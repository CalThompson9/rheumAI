#include "audiohandler.h"
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>

AudioHandler *AudioHandler::instance = nullptr;

AudioHandler::AudioHandler()
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
    std::string response = sendToGeminiAPI(filename);
    return Transcript(getCurrentTime(), response);
}

std::string AudioHandler::sendToGeminiAPI(const std::string &audioPath)
{
    QUrl url(QString::fromStdString(API_URL + "?key=" + GEMINI_API_KEY));
    QNetworkRequest request(url);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart audioPart;
    audioPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    audioPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + QString::fromStdString(audioPath) + "\""));

    QFile *file = new QFile(QString::fromStdString(audioPath));
    file->open(QIODevice::ReadOnly);
    audioPart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(audioPart);

    QNetworkReply *reply = networkManager->post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    std::string response;
    if (reply->error() == QNetworkReply::NoError)
    {
        response = reply->readAll().toStdString();
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
