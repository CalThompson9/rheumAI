/**
 * @file settings.h
 * @brief Declaration of Settings class
 * 
 * @author Thomas Llamzon (tllamzon@uwo.ca
 * @date Mar. 16, 2025
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QAction>
#include <QMediaDevices>
#include <QtMultimedia/qaudiodevice.h>
#include "llmclient.h"
#include "audiohandler.h"
#include "windowbuilder.h"

/**
 * @class Settings
 * @brief The Settings class provides a dialog for configuring application settings.
 * @details This class allows the user to set API keys for LLM and audio services,
 *          as well as select a summary layout preference. It uses a singleton pattern
 *          to ensure that only one instance of the settings dialog is created.
 * @note The settings are stored in a configuration file for persistence across sessions.
 * @author Thomas Llamzon
 */
class Settings : public QObject {
    Q_OBJECT

    friend class MainWindow;

public:
    static Settings* getInstance(QObject *parent);
    ~Settings() = default;

    void showSettings();

    void setLLMKey(const QString newKey);
    void setGoogleSpeechApiKey(const QString newKey);
    void setOpenAIAudioKey(QString newKey);
    void setSummaryPreference(const QString pref);

    QString getLLMKey() const;
    QString getGoogleSpeechApiKey() const;
    QString getOpenAIAudioKey() const;
    QString getSummaryPreference() const;

private:
    Settings(QObject *parent);

    static QString readKey(const QString& keyPrefix);
    void storeConfig(const QString &config, const QString &value);

    QObject *mainWindow;
    QString llmKey;
    QString googleSpeechApiKey;
    QString openAIAudioKey;
    QString summaryLayoutPreference;

    static QString keyFilename;
    static Settings *instance;
};

#endif // SETTINGS_H
