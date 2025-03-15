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

    QString initialPrompt;

    // Read the initial prompt from llmprompt.txt
    QFile file(":/llmprompt.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open llmprompt.txt. Request aborted.";
        return;
    }
    initialPrompt = QTextStream(&file).readAll().trimmed();
    file.close();

    if (initialPrompt.isEmpty()) {
        qWarning() << "Initial prompt is empty! Request aborted.";
        return;
    }

    // Combine initial prompt and user prompt
    QString fullPrompt = initialPrompt + "\n\n" + prompt;

    userPrompt = fullPrompt;  // Store user prompt including initial prompt

    sendLLMRequest(fullPrompt);
}


void LLMClient::sendLLMRequest(const QString &inputPrompt)
{
    QUrl url("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-8b:generateContent?key=" + apiKey);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Constructing the JSON request body
    QJsonObject textPart;
    textPart["text"] = inputPrompt;

    QJsonObject content;
    content["parts"] = QJsonArray{textPart};

    QJsonObject requestBody;
    requestBody["contents"] = QJsonArray{content};

    // Add parameters for the request
    QJsonObject generationConfig;
    generationConfig["temperature"] = 0;
    generationConfig["top_p"] = 1.0;
    generationConfig["top_k"] = 40;
    requestBody["generationConfig"] = generationConfig;  

    QJsonDocument jsonDoc(requestBody);
    QByteArray jsonData = jsonDoc.toJson();


    // Send POST request
    networkManager->post(request, jsonData);
}


void LLMClient::handleNetworkReply(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    reply->deleteLater(); // Clean up the reply object

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error:" << reply->errorString();
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (!jsonResponse.isObject()) {
        qWarning() << "Invalid JSON response.";
        return;
    }

    QJsonObject jsonObj = jsonResponse.object();
    if (!jsonObj.contains("candidates") || !jsonObj["candidates"].isArray()) {
        qWarning() << "No candidates found in response.";
        return;
    }

    QJsonArray candidates = jsonObj["candidates"].toArray();
    if (candidates.isEmpty() || !candidates[0].isObject()) {
        qWarning() << "Empty candidates list.";
        return;
    }

    QJsonObject candidate = candidates[0].toObject();
    if (!candidate.contains("content") || !candidate["content"].isObject()) {
        qWarning() << "No content in response.";
        return;
    }

    QJsonObject contentObj = candidate["content"].toObject();
    if (!contentObj.contains("parts") || !contentObj["parts"].isArray()) {
        qWarning() << "No parts in content.";
        return;
    }

    QJsonArray parts = contentObj["parts"].toArray();
    if (parts.isEmpty() || !parts[0].isObject()) {
        qWarning() << "No valid text response found.";
        return;
    }

    QString responseText = parts[0].toObject().value("text").toString();
    if (responseText.isEmpty()) {
        qWarning() << "Response text is empty.";
        return;
    }


    // Emit the final response
    emit responseReceived(responseText);
}
