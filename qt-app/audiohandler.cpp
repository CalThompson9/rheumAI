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

// Replace with your Google Cloud API key and endpoint
const QString API_URL = "https://speech.googleapis.com/v1/speech:recognize";

AudioHandler *AudioHandler::instance = nullptr;

/**
 * @name AudioHandler (Constructor)
 * @brief Initializes the AudioHandler instance
 */
AudioHandler::AudioHandler() : QObject(nullptr)
{
    apiKey = getAPIKey();
    qDebug() << "=== Audio API Key:" << apiKey;

    networkManager = new QNetworkAccessManager(this);
    captureSession.setAudioInput(&audioInput); // Add this line
    captureSession.setRecorder(&recorder);     // Add this line

    // Connect mediaFormatChanged signal to a slot to print a message
    // connect(&recorder, &QMediaRecorder::audioChannelCountChanged, this, []()
    //         { qDebug() << "Media format has changed. sample rate is "; });
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
    configObj["encoding"] = "LINEAR16"; // Adjust based on your WAV file
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
    recorder.setQuality(QMediaRecorder::HighQuality);

    QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    QString filePath = QDir(projectDir).filePath(outputFile);
    recorder.setOutputLocation(QUrl::fromLocalFile(filePath));

    QMediaFormat format;
    format.setFileFormat(QMediaFormat::Wave);
    recorder.setMediaFormat(format);
    recorder.record();
    // qDebug() << "Media format has changed. from Record " << recorder.audioSampleRate();

    // qDebug() << "Recording started with sample rate" << recorder.audioChannelCount();
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
        qWarning() << "Could not open keyFile.txt for reading:" << file.errorString();
        return QString();
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
