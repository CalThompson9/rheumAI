/**
 * @file audiohandler.cpp
 * @brief Definition of AudioHandler class
 *
 * Handles audio recording and transcription using Google Speech-to-Text API.
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @date Mar. 6, 2025
 */

#include "audiohandler.h"
#include <QMediaDevices>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QCoreApplication>
#if QT_CONFIG(permissions)
  #include <QPermission>
#endif

// Replace with your Google Cloud API key and endpoint
const QString API_URL = "https://speech.googleapis.com/v1/speech:recognize";

AudioHandler *AudioHandler::instance = nullptr;

/**
 * @name AudioHandler (Constructor)
 * @brief Initializes the AudioHandler instance
 */
AudioHandler::AudioHandler() : QObject(nullptr)
{
    //apiKey = getAPIKey();
    apiKey = "AIzaSyBV63tBSl7HUCC149tEXKqfTW0j57hS_8g";
    qDebug() << "Audio API Key:" << apiKey;

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
Transcript AudioHandler::transcribe(const QString &filename)
{
    QString response = sendToGoogleSpeechAPI(filename);
    if (response.isEmpty())
    {
        emit transcriptionCompleted("Transcription failed");
        return Transcript(getCurrentTime(), "");
    }

    // Parse JSON response from Google Speech-to-Text
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if (!doc.isObject())
    {
        qDebug() << "Invalid JSON response";
        emit transcriptionCompleted("Invalid response format");
        return Transcript(getCurrentTime(), "");
    }

    QJsonObject jsonObj = doc.object();
    QJsonArray results = jsonObj["results"].toArray();
    QString transcribedText;
    for (const QJsonValue &result : results)
    {
        QJsonObject resObj = result.toObject();
        QJsonArray alternatives = resObj["alternatives"].toArray();
        if (!alternatives.isEmpty())
        {
            transcribedText += alternatives[0].toObject()["transcript"].toString();
        }
    }

    emit transcriptionCompleted(transcribedText);
    return Transcript(getCurrentTime(), transcribedText);
}

/**
 * @name sendToGoogleSpeechAPI
 * @brief Sends the audio file to Google Speech-to-Text API for transcription
 * @param[in] audioPath: Path to the audio file
 * @return Response from the API as a string
 */
QString AudioHandler::sendToGoogleSpeechAPI(const QString &audioPath)
{
    QUrl url(API_URL + QString::fromStdString("?key=") + apiKey);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Read audio file
    QFile file(audioPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open audio file: " << audioPath;
        return "";
    }
    QByteArray audioData = file.readAll();
    file.close();

    // Base64 encode the audio data (Google expects this for small files)
    QString base64Audio = audioData.toBase64();

    // Construct JSON payload
    QJsonObject configObj;
    configObj["encoding"] = "LINEAR16";
    configObj["sampleRateHertz"] = 48000;
    configObj["languageCode"] = "en-CA";
    configObj["audioChannelCount"] = 2;

    QJsonObject audioObj;
    audioObj["content"] = base64Audio;

    QJsonObject requestBody;
    requestBody["config"] = configObj;
    requestBody["audio"] = audioObj;

    QJsonDocument doc(requestBody);
    QByteArray jsonData = doc.toJson();

    // Send POST request
    QNetworkReply *reply = networkManager->post(request, jsonData);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError)
    {
        response = reply->readAll();
        qDebug() << "API Response: " << response;
    }
    else
    {
        qDebug() << "Request failed: " << reply->errorString();
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

    // Get the preferred format and check requirements
    QAudioFormat format = defaultMic.preferredFormat();

    if (format.sampleRate() != 48000 || format.channelCount() != 2)
    {
        qWarning() << "Microphone does not support required format!";
        qWarning() << "   Required: 48kHz, 2 channels";
        qWarning() << "   Found: " << format.sampleRate() << "Hz, " << format.channelCount() << " channels";
        return;
    }

    qDebug() << "Microphone meets requirements: 48kHz, 2 channels";

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
    // qDebug() << "Recording ended with sample rate" << recorder.audioSampleRate();
    // qDebug() << "Recording ended with channel" << recorder.audioChannelCount();
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
 * @author Thomas Llamzon
 * @brief Reads keyFile upon class construction to set apiKey.
 * @return Returns LLM API Key.
*/
QString AudioHandler::getAPIKey() {

    QFile file("keyFile.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "keyFile does not exist yet.";
        return "";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("AUDIO_API_KEY:")) {
            file.close();
            return line.mid(QString("AUDIO_API_KEY:").length()).trimmed();
        }
    }

    return "";
}



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
        emit microphonePermissionDenied();  // Handle UI warning
        return;
    case Qt::PermissionStatus::Granted:
        qDebug() << "Microphone permission granted!";
        break;
    }
#else
    qWarning() << "⚠️ Qt permissions API is disabled!";
#endif
}

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
