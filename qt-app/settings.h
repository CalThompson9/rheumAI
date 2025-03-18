#ifndef SETTINGS_H
#define SETTINGS_H

#include "llmclient.h"
#include <QObject>
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

/**
 * @author Thomas Llamzon
 * @name Settings
 * @brief The Settings class acts as a storage class, storing all of the user's preferences and configurations.
 */
class Settings : public QObject {
    Q_OBJECT

public:
    /**
     * @name Settings
     * @brief Constructor for the static settings class
     * @param parent
     */
    Settings(QObject *parent = nullptr, LLMClient *client = nullptr);

    /**
     * @name getPeripherals
     * @brief Returns the peripherals connected to the application. *** TYPE UNKNOWN AS OF NOW ***
     * @return ^
     */
    bool getPeripherals();

    /**
     * @name setAPIKey
     * @brief Allows LLM Client API Key to be set from settings menu.
     * @param key - API Key.
     */
    void setLLMKey(const QString newKey);

    /**
     * @name setWsprKey
     * @brief Allows Whisper Client API Key to be set from settings menu.
     * @param key - API Key.
     */
    void setWsprKey(const QString newKey);


private:
    LLMClient *client;
    QString summaryLayout = "";
    bool connectedPeripherals = false;
};

#endif // SETTINGS_H
