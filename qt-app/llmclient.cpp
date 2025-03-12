#include "llmclient.h"

LLMClient::LLMClient(QObject *parent) 
    : QObject(parent), networkManager(new QNetworkAccessManager(this))
{
    apiKey = "";  // ⚠️ Hardcoded API Key NEEDS TO GO HERE. DO NOT PUSH WITH API KEY. ⚠️

    if (apiKey.isEmpty()) {
        qWarning() << "API Key is empty! Request aborted.";
    }

    connect(networkManager, &QNetworkAccessManager::finished, this, &LLMClient::handleNetworkReply);
}


void LLMClient::sendRequest(const QString &prompt)
{
    if (apiKey.isEmpty()) {
        qWarning() << "API Key is empty! Request aborted.";
        return;
    }

    QUrl url("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + apiKey);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Correct JSON request format for Gemini API
    QJsonObject textPart;
    textPart["text"] = prompt;

    QJsonObject content;
    content["parts"] = QJsonArray{textPart};

    QJsonObject requestBody;
    requestBody["contents"] = QJsonArray{content};

    QJsonDocument jsonDoc(requestBody);
    QByteArray jsonData = jsonDoc.toJson();

    // Send POST request
    networkManager->post(request, jsonData);
}


void LLMClient::handleNetworkReply(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (jsonResponse.isObject()) {
            QJsonObject jsonObj = jsonResponse.object();
            if (jsonObj.contains("candidates") && jsonObj["candidates"].isArray()) {
                QJsonArray candidates = jsonObj["candidates"].toArray();
                if (!candidates.isEmpty() && candidates[0].isObject()) {
                    QJsonObject candidate = candidates[0].toObject();
                    if (candidate.contains("content") && candidate["content"].isObject()) {
                        QJsonObject contentObj = candidate["content"].toObject();
                        if (contentObj.contains("parts") && contentObj["parts"].isArray()) {
                            QJsonArray parts = contentObj["parts"].toArray();
                            if (!parts.isEmpty() && parts[0].isObject()) {
                                QString responseText = parts[0].toObject().value("text").toString();
                                if (!responseText.isEmpty()) {
                                    emit responseReceived(responseText);
                                    reply->deleteLater();
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    reply->deleteLater();
}

