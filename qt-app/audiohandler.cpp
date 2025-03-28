/**
 * @file audiohandler.cpp
 * @brief Definition of AudioHandler class
 *
 * Handles audio recording and transcription using Google Speech-to-Text API.
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @date Mar. 6, 2025
 * @author Callum Thompson
 * @date Mar. 21, 2025
 */


#include <QMediaDevices>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QCoreApplication>
#include <QHttpMultiPart>
#include <QHttpPart>
#if QT_CONFIG(permissions)
  #include <QPermission>
#endif
#include "audiohandler.h"

AudioHandler *AudioHandler::instance = nullptr;

/**
 * @name AudioHandler (Constructor)
 * @brief Initializes the AudioHandler instance
 */
AudioHandler::AudioHandler() : QObject(nullptr)
{
    qDebug() << "Google API Key:" << apiKey;
    qDebug() << "OpenAI API Key:" << openAIApiKey;

    networkManager = new QNetworkAccessManager(this);
}


/**
 * @name getInstance
 * @brief Returns the singleton instance of AudioHandler
 * @return Singleton instance of AudioHandler
 */
AudioHandler *AudioHandler::getInstance()
{
    if (!instance)
    {
        instance = new AudioHandler();
    }
    return instance;
}

/**
 * @name transcribe
 * @brief Transcribes the audio file using Google Speech-to-Text API
 * @param[in] filename: Path to the audio file
 * @return Transcript object containing the transcribed text
 */
Transcript AudioHandler::transcribe(const QString& filename)
{
    double durationSecs = getAudioDuration(filename);
    int channelCount = getAudioChannelCount(filename);

    qDebug() << "Audio duration:" << durationSecs << "seconds";
    qDebug() << "Channel count:" << channelCount;

    QString response;

    // Use Whisper if longer than 60s or not stereo (2 channels)
    if (durationSecs > 60.0 || channelCount != 2) {
        qDebug() << "Using Whisper (OpenAI)";
        response = sendToWhisperAPI(filename);
    } else {
        qDebug() << "Using Google Speech-to-Text";
        response = sendToGoogleSpeechAPI(filename);
    }

    if (response.isEmpty()) {
        emit transcriptionCompleted("Transcription failed");
        return Transcript(getCurrentTime(), "");
    }

    QString result;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if (!doc.isObject()) {
        emit transcriptionCompleted("Invalid response format");
        return Transcript(getCurrentTime(), "");
    }

    if (durationSecs > 60.0 || channelCount != 2) {
        result = doc.object().value("text").toString();
    } else {
        QJsonArray results = doc.object().value("results").toArray();
        for (const QJsonValue& val : results) {
            QJsonArray alternatives = val.toObject().value("alternatives").toArray();
            if (!alternatives.isEmpty()) {
                result += alternatives[0].toObject().value("transcript").toString();
            }
        }
    }

    emit transcriptionCompleted(result);
    return Transcript(getCurrentTime(), result);
}

/**
 * @name getAudioChannelCount
 * @brief Retrieves the number of audio channels in a WAV file
 * @param[in] audioPath: Path to the audio file
 * @return Number of audio channels
 */
int AudioHandler::getAudioChannelCount(const QString& audioPath) const
{
    QFile file(audioPath);
    if (!file.open(QIODevice::ReadOnly)) return -1;

    file.seek(22); // Byte offset for number of channels in a WAV file header
    char buffer[2];
    file.read(buffer, 2);
    file.close();

    int channels = static_cast<unsigned char>(buffer[0]) |
                   (static_cast<unsigned char>(buffer[1]) << 8);
    return channels;
}


/**
 * @name sendToWhisperAPI
 * @brief Sends the audio file to Google Speech-to-Text API for transcription
 * @param[in] audioPath: Path to the audio file
 * @return Response from the API as a string
 */
QString AudioHandler::sendToWhisperAPI(const QString& audioPath)
{
    qDebug() << "Preparing to send audio to Whisper API...";
    qDebug() << "Audio path:" << audioPath;
    qDebug() << "OpenAI API Key (first 10 chars):" << openAIApiKey.left(10) << "...";

    if (openAIApiKey.isEmpty()) {
        qWarning() << "OpenAI API Key is empty!";
        return "";
    }

    QUrl url("https://api.openai.com/v1/audio/transcriptions");
    QNetworkRequest request(url);

    QString bearerToken = "Bearer " + openAIApiKey;
    request.setRawHeader("Authorization", bearerToken.toUtf8());

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QFile* file = new QFile(audioPath);
    if (!file->open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for Whisper API:" << audioPath;
        delete file;
        return "";
    }

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(audioPath).fileName() + "\""));
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QHttpPart modelPart;
    modelPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"model\""));
    modelPart.setBody("whisper-1");
    multiPart->append(modelPart);

    qDebug() << "Sending POST request to Whisper API...";
    QNetworkReply* reply = networkManager->post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        response = reply->readAll();
        qDebug() << "✅ Whisper API Response:" << response;
    } else {
        qWarning() << "❌ Whisper request failed:" << reply->errorString();
        qDebug() << "Reply HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Raw reply:" << reply->readAll();
    }

    reply->deleteLater();
    return response;
}

/**
 * @name sendToGoogleSpeechAPI
 * @brief Sends the audio file to Google Speech-to-Text API for transcription
 * @param[in] audioPath: Path to the audio file
 * @return Response from the API as a string
 */
QString AudioHandler::sendToGoogleSpeechAPI(const QString& audioPath)
{
    QUrl url("https://speech.googleapis.com/v1/speech:recognize?key=" + apiKey);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QFile file(audioPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open audio file: " << audioPath;
        return "";
    }

    QByteArray audioData = file.readAll();
    file.close();
    QString base64Audio = audioData.toBase64();

    QJsonObject config;
    config["encoding"] = "LINEAR16";
    config["sampleRateHertz"] = 48000;
    config["languageCode"] = "en-CA";
    config["audioChannelCount"] = 2;

    QJsonObject audio;
    audio["content"] = base64Audio;

    QJsonObject root;
    root["config"] = config;
    root["audio"] = audio;

    QJsonDocument doc(root);
    QByteArray jsonData = doc.toJson();

    QNetworkReply* reply = networkManager->post(request, jsonData);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        response = reply->readAll();
    } else {
        qWarning() << "Google request failed:" << reply->errorString();
    }

    reply->deleteLater();
    return response;
}


/**
 * @name startRecording
 * @brief Starts audio recording
 * @param[in] outputFile: Path to the output file
 */
void AudioHandler::startRecording(const QString &outputFile)
{
    requestMicrophonePermission();

    // Get available microphones
    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    if (devices.isEmpty())
    {
        qWarning() << "No microphone detected!";
        return;
    }

    QAudioDevice defaultMic = devices.first();
    qDebug() << "🎤 Using microphone:" << defaultMic.description();

    // Delete previous audio input if it exists
    if (audioInput != nullptr)
    {
        delete audioInput;
        audioInput = nullptr;
    }

    // Create QAudioInput with the validated format
    audioInput = new QAudioInput(defaultMic);
    captureSession.setAudioInput(audioInput);
    captureSession.setRecorder(&recorder);

    // Set up recorder output
    QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    QString filePath = QDir(projectDir).filePath(outputFile);
    recorder.setOutputLocation(QUrl::fromLocalFile(filePath));

    // Set media format to Wave
    QMediaFormat mediaFormat;
    mediaFormat.setFileFormat(QMediaFormat::Wave);
    recorder.setMediaFormat(mediaFormat);

    // Apply required settings to recorder
    recorder.setAudioSampleRate(48000);
    recorder.setAudioChannelCount(2);

    qDebug() << "🎙️ Starting recording. Output file:" << filePath;
    recorder.record();
}



/**
 * @name pauseRecording
 * @brief Pauses audio recording
 */
void AudioHandler::pauseRecording()
{
    recorder.pause();
}

/**
 * @name resumeRecording
 * @brief Resumes audio recording
 */
void AudioHandler::resumeRecording()
{
    recorder.record();
}

/**
 * @name stopRecording
 * @brief Stops audio recording
 */
void AudioHandler::stopRecording()
{
    recorder.stop();
}

/**
 * @name getCurrentTime
 * @brief Retrieves the current time
 * @return Current time
 */
QTime AudioHandler::getCurrentTime() const
{
    return QTime::currentTime();
}

/**
 * @name requestMicrophonePermission
 * @brief Requests permission to use the microphone
 * @details Uses the Qt Permissions API to request microphone access
 */
void AudioHandler::requestMicrophonePermission()
{
#if QT_CONFIG(permissions)
    QMicrophonePermission microphonePermission;
    switch (qApp->checkPermission(microphonePermission))
    {
    case Qt::PermissionStatus::Undetermined:
        qDebug() << "Requesting microphone permission...";
        qApp->requestPermission(microphonePermission, this, &AudioHandler::handlePermissionResponse);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning() << "Microphone permission denied!";
        emit microphonePermissionDenied();
        return;
    case Qt::PermissionStatus::Granted:
        qDebug() << "Microphone permission granted!";
        break;
    }
#else
    qWarning() << "⚠️ Qt permissions API is disabled!";
#endif
}

/**
 * @name handlePermissionResponse
 * @brief Handles the response to the microphone permission request
 * @details Emits signals based on the user's response
 */
void AudioHandler::handlePermissionResponse()
{
    QMicrophonePermission microphonePermission;
    if (qApp->checkPermission(microphonePermission) == Qt::PermissionStatus::Granted)
    {
        qDebug() << "Microphone permission granted!";
        emit microphonePermissionGranted();
    }
    else
    {
        qWarning() << "User denied microphone permission.";
        emit microphonePermissionDenied();
    }
}

/**
 * @name playRecording
 * @brief Plays the recorded audio file
 * @param[in] filePath: Path to the audio file
 */
void AudioHandler::playRecording(const QString &filePath)
{
    if (!QFile::exists(filePath))
    {
        qWarning() << "Recorded file does not exist: " << filePath;
        return;
    }
    qDebug() << "Playing recorded file:" << filePath;
    QString command = "afplay " + filePath; // macOS playback command
    system(command.toUtf8().constData());
}

/**
 * @name setGoogleApiKey
 * @brief Sets the Google Speech-to-Text API key
 * @param[in] key: Googl Speech-to-Text API key
 */
void AudioHandler::setGoogleApiKey(const QString& key)
{
    apiKey = key;
}

/**
 * @name setOpenAIApiKey
 * @brief Sets the OpenAI API key
 * @param[in] key: OpenAI API key
 */
void AudioHandler::setOpenAIApiKey(const QString& key)
{
    openAIApiKey = key;
}

/**
 * @name getAudioDuration
 * @brief Retrieves the duration of an audio file
 * @param[in] audioPath: Path to the audio file
 */
double AudioHandler::getAudioDuration(const QString& audioPath) const
{
    QFile file(audioPath);
    if (!file.open(QIODevice::ReadOnly)) return 0;

    qint64 fileSize = file.size();
    file.close();

    // Assume 16-bit PCM, 2 channels, 48000Hz => 192000 bytes/sec
    return fileSize / 192000.0;
}
