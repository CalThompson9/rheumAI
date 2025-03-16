#ifndef SETTINGS_H
#define SETTINGS_H

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
     * @name getInstance
     * @brief Returns the static instance of the settings configuration
     * @return
     */
    static Settings* getInstance();

    /**
     * @name getAPIKey
     * @brief Returns the LLM Client's API Key
     * @return ^
     */
    QString getAPIKey();

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
    void setAPIKey(const QString key);


private:
    /**
     * @name Settings
     * @brief Constructor for the static settings class
     * @param parent
     */
    explicit Settings(QObject *parent = nullptr);

    // Disable copy-construction and assignment.
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    // Settings Variables
    QString APIKey = "";
    QString summaryLayout = "";
    bool connectedPeripherals = false;
};

#endif // SETTINGS_H
