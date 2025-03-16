#include "settings.h"

Settings::Settings(QObject *Parent) :
    QObject(Parent),
    APIKey(""),
    summaryLayout(""),
    connectedPeripherals(false)
{
}

Settings* Settings::getInstance() {
    static Settings instance;  // Constructed only once.
    return &instance;
}

/**
 * @brief Settings::getAPIKey
 * @return
 */
QString Settings::getAPIKey() {
    return APIKey;
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
void Settings::setAPIKey(QString key) {
    // Update varaible storing Key
    APIKey = key;

    // Apply it to LLM Client
}

