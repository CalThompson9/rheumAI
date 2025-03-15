#include "audiohandler.h"

#include <QAudioSource>
#include <QDebug>
#include <QProcess>
#include <Qlabel>

// Replace with your Google Cloud API key and endpoint
const std::string API_URL = "https://speech.googleapis.com/v1/speech:recognize";

AudioHandler *AudioHandler::instance = nullptr;

AudioHandler::AudioHandler() : QObject(nullptr)
{
    networkManager = new QNetworkAccessManager(this);
    captureSession.setAudioInput(&audioInput); // Add this line
    captureSession.setRecorder(&recorder);     // Add this line

    // Connect mediaFormatChanged signal to a slot to print a message
    connect(&recorder, &QMediaRecorder::audioChannelCountChanged, this, []()
            { qDebug() << "Media format has changed. sample rate is "; });
}

AudioHandler *AudioHandler::getInstance()
{
    if (!instance)
    {
        instance = new AudioHandler();
    }
    return instance;
}

Transcript AudioHandler::transcribe(const std::string &filename)
{
    std::string response = sendToGoogleSpeechAPI(filename);
    if (response.empty())
    {
        emit transcriptionCompleted("Transcription failed");
        return Transcript(getCurrentTime(), "");
    }

    // Parse JSON response from Google Speech-to-Text
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(response));
    if (!doc.isObject())
    {
        std::cerr << "Invalid JSON response" << std::endl;
        emit transcriptionCompleted("Invalid response format");
        return Transcript(getCurrentTime(), "");
    }

    QJsonObject jsonObj = doc.object();
    QJsonArray results = jsonObj["results"].toArray();
    std::string transcribedText;
    for (const QJsonValue &result : results)
    {
        QJsonObject resObj = result.toObject();
        QJsonArray alternatives = resObj["alternatives"].toArray();
        if (!alternatives.isEmpty())
        {
            transcribedText += alternatives[0].toObject()["transcript"].toString().toStdString();
        }
    }

    QString qText = QString::fromStdString(transcribedText);
    emit transcriptionCompleted(qText);
    return Transcript(getCurrentTime(), transcribedText);
}

std::string AudioHandler::sendToGoogleSpeechAPI(const std::string &audioPath)
{
    QUrl url(QString::fromStdString(API_URL + "?key=" + AUDIO_API_KEY));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Read audio file
    QFile file(QString::fromStdString(audioPath));
    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Could not open audio file: " << audioPath << std::endl;
        return "";
    }
    QByteArray audioData = file.readAll();
    file.close();

    // Base64 encode the audio data (Google expects this for small files)
    QString base64Audio = audioData.toBase64();

    // Construct JSON payload
    QJsonObject configObj;
    configObj["encoding"] = "LINEAR16"; // Adjust based on your WAV file
    configObj["sampleRateHertz"] = 16000;
    configObj["languageCode"] = "en-CA";

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

    std::string response;
    if (reply->error() == QNetworkReply::NoError)
    {
        response = reply->readAll().toStdString();
        std::cout << "API Response: " << response << std::endl;
    }
    else
    {
        std::cerr << "Request failed: " << reply->errorString().toStdString() << std::endl;
    }

    reply->deleteLater();
    return response;
}

void AudioHandler::recompileResource()
{
    // Create dynamic resource loader
    QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    DynamicResourceLoader loader("resources.qrc", "resources.cpp");

    // Add a resource dynamically
    loader.addResource("output.wav", "");
    loader.loadResources();
}

void AudioHandler::startRecording(const QString &outputFile)
{
    recorder.setQuality(QMediaRecorder::HighQuality);

    QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    QString filePath = QDir(projectDir).filePath(outputFile);
    recorder.setOutputLocation(QUrl::fromLocalFile(filePath));

    QMediaFormat format;
    format.setFileFormat(QMediaFormat::Wave);

    recorder.setMediaFormat(format);
    qDebug() << "Media format has changed. from MEDIA FORMAT ";
    recorder.setAudioSampleRate(16000);
    qDebug() << "Media format has changed. from SAMPLE RATE ";
    recorder.setAudioChannelCount(1);
    qDebug() << "Media format has changed. from AUDIO CHANEL ";

    recorder.record();
    qDebug() << "Media format has changed. from Record " << recorder.audioSampleRate();

    qDebug() << "Recording started with sample rate" << recorder.audioChannelCount();
}

void AudioHandler::pauseRecording()
{
    recorder.pause();
}

void AudioHandler::resumeRecording()
{
    recorder.record();
}

void AudioHandler::stopRecording()
{
    recorder.stop();
    qDebug() << "Recording ended with sample rate" << recorder.audioSampleRate();
    qDebug() << "Recording ended with channel" << recorder.audioChannelCount();
}

time_t AudioHandler::getCurrentTime()
{
    return std::time(nullptr);
}
