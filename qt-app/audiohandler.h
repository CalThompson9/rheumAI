#ifndef AUDIO_HANDLER_H
#define AUDIO_HANDLER_H

#include <string>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include "transcript.h"
#include "api.h"

class AudioHandler : public QObject
{
    Q_OBJECT

private:
    static AudioHandler *instance;
    AudioHandler(); // Private constructor for Singleton

public:
    static AudioHandler *getInstance();
    Transcript transcribe(const std::string &filename);

private:
    const std::string API_KEY = "YOUR_GEMINI_API_KEY";
    const std::string API_URL = "https://api.google.dev/v1/audio:transcribe";
    QNetworkAccessManager *networkManager;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);
    std::string sendToGeminiAPI(const std::string &audioPath);
    time_t getCurrentTime();
};

#endif // AUDIO_HANDLER_H
