#ifndef AUDIO_HANDLER_H
#define AUDIO_HANDLER_H

#include <QJsonArray>
#include <string>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
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
    void startRecording(const QString &outputFile);
    void stopRecording();

signals:
    void transcriptionCompleted(const QString &transcribedText);

private:
    const std::string API_KEY = GEMINI_API_KEY;
    const std::string API_URL = "https://speech.googleapis.com/v1/speech:recognize";
    QNetworkAccessManager *networkManager;
    QAudioRecorder *audioRecorder;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);
    std::string sendToGoogleSpeechAPI(const std::string &audioPath);
    time_t getCurrentTime();
};

#endif // AUDIO_HANDLER_H
