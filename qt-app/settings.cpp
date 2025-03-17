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
 * @brief Settings::setAPIKey
 * @param key
 */
void Settings::setAPIKey(QString newKey) {
    client->apiKey = newKey;
}
