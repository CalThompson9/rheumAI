#ifndef SETTINGS_H
#define SETTINGS_H

#include "llmclient.h"
#include "audiohandler.h"
#include <QObject>
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFile>
#include <QTextStream>

/**
 * @author Thomas Llamzon
 * @name Settings
 * @brief The Settings class acts as a storage class, handling & storing all of the user's preferences and configurations.
 */
class Settings : public QObject {
    Q_OBJECT

    friend class MainWindow;

public:
    Settings(QObject *parent = nullptr, LLMClient *llm = nullptr, AudioHandler *audio = nullptr);
    void setLLMKey(const QString newKey);
    void setAudioKey(const QString newKey);
    void writeAPIKey(const QString &keyClient, const QString &key);

private:
    LLMClient *llmClient;
    AudioHandler *audioHandlerClient;
    QString llmKey;
    QString audioKey;
};

#endif // SETTINGS_H
