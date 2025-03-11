#include "audiohandler.h"

// Replace with your Google Cloud API key and endpoint
const std::string API_URL = "https://speech.googleapis.com/v1/speech:recognize";

AudioHandler *AudioHandler::instance = nullptr;

AudioHandler::AudioHandler() : QObject(nullptr)
{
    networkManager = new QNetworkAccessManager(this);
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
    configObj["languageCode"] = "en-US";

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

time_t AudioHandler::getCurrentTime()
{
    return std::time(nullptr);
}
