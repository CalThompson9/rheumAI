/**
 * @file llmclient.cpp
 * @brief Definition of LLMClient class
 * 
 * Responsible for managing the connection to the Google Gemini API, including 
 * sending requests, and handling responses.
 * 
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Thomas Llamazon (tllamazon@uwo.ca)
 * @date Mar. 4, 2025
 */

#include "llmclient.h"


/**
 * @name LLMClient (constructor)
 * @brief Initializes the LLM client, including the network access manager
 * @param[in] parent: Parent widget
 */

LLMClient::LLMClient(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this))
{

    //apiKey = getAPIKey();

    qDebug() << "LLM API Key: " << apiKey;
    apiKey = "AIzaSyC9Yvpr7fSvGERQw4FwEAgoDnYYC7ji-Qo";  // ⚠️ Hardcoded API Key NEEDS TO GO HERE. DO NOT PUSH WITH API KEY. ⚠️


    if (apiKey.isEmpty()) {
        qWarning() << "API Key is empty! Request aborted.";
    }

    connect(networkManager, &QNetworkAccessManager::finished, this, &LLMClient::handleNetworkReply);
}

/**
 * @name sendRequest
 * @brief Combines the initial prompt with an additional prompt and sends as a
 * request to the LLM.
 * @details Initial prompt is read from file, `llmprompt.txt`
 * @param[in] prompt: Additional prompt
 */
void LLMClient::sendRequest(const QString &prompt)
{
    if (apiKey.isEmpty())
    {
        qWarning() << "API Key is empty! Request aborted.";
        return;
    }

    QString initialPrompt;

    // Read the initial prompt from llmprompt.txt
    QFile file(":/llmprompt.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open llmprompt.txt. Request aborted.";
        return;
    }
    initialPrompt = QTextStream(&file).readAll().trimmed();
    file.close();

    if (initialPrompt.isEmpty())
    {
        qWarning() << "Initial prompt is empty! Request aborted.";
        return;
    }

    // Combine initial prompt and user prompt
    QString fullPrompt = initialPrompt + "\n\n" + prompt;

    userPrompt = fullPrompt; // Store user prompt including initial prompt

    sendLLMRequest(fullPrompt);
}

/**
 * @name sendLLMRequest
 * @brief Parses the input prompt, sets generation configuration, and sends as a
 * request and sends to the LLM.
 * @param[in] inputPrompt: Input prompt
 */
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

/**
 * @name handleNetworkReply
 * @brief Handle network reply by parsing the response data, converting it to a
 * JSON format, and emit a signal that a response has been recieved.
 * @param[in] reply: Network reply
 */
void LLMClient::handleNetworkReply(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    reply->deleteLater(); // Clean up the reply object

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Network error:" << reply->errorString();
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (!jsonResponse.isObject())
    {
        qWarning() << "Invalid JSON response.";
        return;
    }

    QJsonObject jsonObj = jsonResponse.object();
    if (!jsonObj.contains("candidates") || !jsonObj["candidates"].isArray())
    {
        qWarning() << "No candidates found in response.";
        return;
    }

    QJsonArray candidates = jsonObj["candidates"].toArray();
    if (candidates.isEmpty() || !candidates[0].isObject())
    {
        qWarning() << "Empty candidates list.";
        return;
    }

    QJsonObject candidate = candidates[0].toObject();
    if (!candidate.contains("content") || !candidate["content"].isObject())
    {
        qWarning() << "No content in response.";
        return;
    }

    QJsonObject contentObj = candidate["content"].toObject();
    if (!contentObj.contains("parts") || !contentObj["parts"].isArray())
    {
        qWarning() << "No parts in content.";
        return;
    }

    QJsonArray parts = contentObj["parts"].toArray();
    if (parts.isEmpty() || !parts[0].isObject())
    {
        qWarning() << "No valid text response found.";
        return;
    }

    QString responseText = parts[0].toObject().value("text").toString();
    if (responseText.isEmpty())
    {
        qWarning() << "Response text is empty.";
        return;
    }

    // Emit the final response
    emit responseReceived(responseText);
}

/**
 * @author Thomas Llamzon
 * @brief Reads keyFile upon class construction to set apiKey.
 * @return Returns LLM API Key.
 */
QString LLMClient::getAPIKey() {

    QFile file("keyFile.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "keyFile does not exist yet.";
        return "";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("GEMINI_API_KEY:")) {
            file.close();
            return line.mid(QString("GEMINI_API_KEY:").length()).trimmed();
        }
    }

    return "";
}
