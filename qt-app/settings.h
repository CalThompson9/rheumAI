/**
 * @file settings.h
 * @brief Declaration of Settings class
 * 
 * @author Thomas Llamazon (tllamazon@uwo.ca
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

class Settings : public QObject {
    Q_OBJECT

    friend class MainWindow;

public:
    static Settings* getInstance(QObject *parent);
    ~Settings() = default;

    void showSettings();

    void setLLMKey(const QString newKey);
    void setAudioKey(const QString newKey);
    void setOpenAIAudioKey(QString newKey);
    void setSummaryPreference(const QString pref);

    QString getLLMKey() const;
    QString getAudioKey() const;
    QString getOpenAIAudioKey() const;
    QString getSummaryPreference() const;

signals:
    void okButtonClicked();

private:
    Settings(QObject *parent);

    static QString readKey(const QString& keyPrefix);
    void storeConfig(const QString &config, const QString &value);

    QObject *mainWindow;
    QString llmKey;
    QString audioKey;
    QString openAIAudioKey;
    QString summaryLayoutPreference;

    static Settings *instance;
};

#endif // SETTINGS_H
