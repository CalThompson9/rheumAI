#include "settings.h"

/**
 * @name Settings
 * @brief Constructor for settings - stores user preferences and configurations.
 * @param parent - MainWindow
 * @param llmClient - LLM Client instance
 */
Settings::Settings(QObject *parent, LLMClient *llmClient) :
    QObject(parent),
    client(llmClient),
    summaryLayout(""),
    connectedPeripherals(false)
{
}

/**
 * @brief Settings::getPeripherals
 * @return
 */
bool Settings::getPeripherals() {
    return connectedPeripherals;
}

/**
 * @brief Settings::setLLMKey
 * @param key
 */
void Settings::setLLMKey(QString newKey) {
    client->apiKey = newKey;
    qDebug() << "=== Gemini Key:" << client->apiKey << "===";
}

/**
 * @brief Settings::setWsprKey
 * @param key
 */
void Settings::setWsprKey(QString newKey) {
    //client->apiKey = newKey;
    qDebug() << "=== Whisper Key:" << newKey << "===";
}
