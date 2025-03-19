#include "settings.h"

/**
 * @todo Default summary layout, API key change automatic restart, API key field default value
 */

/**
 * @name Settings
 * @brief Constructor for settings - stores user preferences and configurations.
 * @param parent - MainWindow
 * @param llmClient - LLM Client instance
 */
Settings::Settings(QObject *parent, LLMClient *llm, AudioHandler *audio)
    : QObject(parent),
    llmClient(llm),
    audioHandlerClient(audio)
{
}

/**
 *  * @name setLLMKey
 * @brief Sets LLM API key and modifies key storage file for continual use.
 * @param key
 */
void Settings::setLLMKey(QString newKey) {

    // Set key in LLMClient class
    llmClient->apiKey = newKey;

    // Change API Key in keyFile
    writeAPIKey("LLM", newKey);
}

/**
 * @name setAudioKey
 * @brief Sets audio transcriber API key and modifies key storage file for continual use.
 * @param key
 */
void Settings::setAudioKey(QString newKey) {

    // Set key in AudioHandler class
    audioHandlerClient->apiKey = newKey;

    // Change API Key in keyFile
    writeAPIKey("AUDIO", newKey);
}

/**
 * @name writeAPIKey
 * @brief Writes API key to hidden key file storing API keys for continual use.
 * @param keyClient - Client to set API key for
 * @param key - User API key
 */
void Settings::writeAPIKey(const QString &keyClient, const QString &key)
{
    QString prefix;
    if (keyClient == "LLM") {
        prefix = "GEMINI_API_KEY:";
    } else if (keyClient == "AUDIO") {
        prefix = "AUDIO_API_KEY:";
    } else {
        qWarning() << "Unknown keyClient:" << keyClient;
        return;
    }

    QFile file("keyFile.txt");
    QStringList lines;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines << in.readLine();
        }
        file.close();
    } else {
        if (file.error() != QFile::FileError::OpenError) {
            qWarning() << "Could not open file for reading:" << file.errorString();
            return;
        }
    }

    // Search for an existing line with the prefix and update it
    bool foundLine = false;
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].startsWith(prefix)) {
            lines[i] = prefix + " " + key; // Replace everything after prefix
            foundLine = true;
            break;
        }
    }

    if (!foundLine) {
        lines << prefix + " " + key;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (const QString &line : lines) {
        out << line << "\n";
    }

    file.close();
}
