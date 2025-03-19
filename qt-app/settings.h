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

public:
    Settings(QObject *parent = nullptr, LLMClient *llm = nullptr, AudioHandler *audio = nullptr);
    void setLLMKey(const QString newKey);
    void setAudioKey(const QString newKey);
    void writeAPIKey(const QString &keyClient, const QString &key);

private:
    LLMClient *llmClient;
    AudioHandler *audioHandlerClient;
    QString userPath;
    QString keyFilePath = "../../../../../keyFile.txt";
    QString summaryLayout = "";
};

#endif // SETTINGS_H
