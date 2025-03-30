/**
 * @file llmclient.cpp
 * @brief Definition of LLMClient class
 * 
 * @details Responsible for managing the connection to the Google Gemini API, including 
 * sending requests, and handling responses.
 * 
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Thomas Llamzon (tllamzon@uwo.ca)
 * @date Mar. 4, 2025
 */

#include "llmclient.h"
#include "settings.h"

LLMClient *LLMClient::instance = nullptr;

/**
 * @name LLMClient (constructor)
 * @brief Initializes the LLM client, including the network access manager
 * @details Reads the API key from a file and sets up the network access manager.
 * @param[in] QNetworkManager: Network access manager for handling network requests
 * @author Callum Thompson
 */
LLMClient::LLMClient()
    : QObject(nullptr), networkManager(new QNetworkAccessManager(this))
{
    connect(networkManager, &QNetworkAccessManager::finished, this, &LLMClient::handleNetworkReply);
}

/**
 * @name getInstance
 * @brief Returns the singleton instance of LLMClient
 * @details This function ensures that only one instance of LLMClient exists at a
 * time. It is a singleton pattern implementation.
 * @param[in] parent: Parent widget
 * @return Singleton instance of LLMClient
 * @author Callum Thompson
 */
LLMClient *LLMClient::getInstance()
{
    if (!instance)
    {
        instance = new LLMClient();
    }
    return instance;
}

/**
 * @name sendRequest
 * @brief Combines the initial prompt with an additional prompt and sends as a
 * request to the LLM.
 * @details Initial prompt is read from file, `llmprompt.txt`. 
 * @param[in] prompt: Additional prompt
 * @author Callum Thompson
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
 * @name setApiKey
 * @brief Sets the API key
 * @param[in] key: API key
 */
void LLMClient::setApiKey(const QString& key)
{
    apiKey = key;
}

/**
 * @name sendLLMRequest
 * @brief Parses the input prompt, sets generation configuration, and sends as a
 * request and sends to the LLM.
 * @details This function constructs the JSON request body, sets the necessary headers, 
 * and sends the request to the google gemini API, the version is gemini-1.5-flash-8b.
 * @param[in] inputPrompt: Input prompt
 * @author Callum Thompson
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
 * @details This function checks for errors in the network reply, parses the JSON response,
 * and extracts the relevant information. If the response is valid, it emits a signal with
 * the response text.
 * @param[in] reply: Network reply
 * @author Callum Thompson
 */
void LLMClient::handleNetworkReply(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    reply->deleteLater(); // Clean up the reply object

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Network error:" << reply->errorString();
        emit invalidAPIKey(reply);
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

