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
#include "llmclient.h"
#include "audiohandler.h"
#include "windowbuilder.h"

/**
 * @author Thomas Llamzon
 * @brief Declaration file of settings class
 */

class Settings : public QObject {
    Q_OBJECT

    friend class MainWindow;

public:
    static Settings* getInstance(QObject *parent, LLMClient *llm, AudioHandler *audio);
    ~Settings();

    void showSettings();

    void setLLMKey(const QString newKey);
    void setAudioKey(const QString newKey);
    void setSummaryPreference(const QString pref);
    QString getSummaryPreference() const;

signals:
    void okButtonClicked();

private:
    Settings(QObject *parent, LLMClient *llm, AudioHandler *audio);

    void storeConfig(const QString &config, const QString &value);

    QObject *mainWindow;
    LLMClient *llmClient;
    AudioHandler *audioHandlerClient;
    QString llmKey;
    QString audioKey;
    QString summaryLayoutPreference;

    static Settings *instance;
};

#endif // SETTINGS_H
