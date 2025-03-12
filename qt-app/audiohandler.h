#ifndef AUDIO_HANDLER_H
#define AUDIO_HANDLER_H

#include <QJsonArray>
#include <string>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include <QtNetwork/QNetworkAccessManager>
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QAudioSource>
#include <QBuffer>
#include <QMediaDevices>
#include <QStandardPaths>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QDir>
#include <QCoreApplication>
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
    void pauseRecording();
    void resumeRecording();

signals:
    void transcriptionCompleted(const QString &transcribedText);

private:
    const std::string API_KEY = AUDIO_API_KEY;
    const std::string API_URL = "https://speech.googleapis.com/v1/speech:recognize";
    QNetworkAccessManager *networkManager;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);
    std::string sendToGoogleSpeechAPI(const std::string &audioPath);
    time_t getCurrentTime();
    QString outputFilePath;
    QMediaRecorder recorder;
    QMediaCaptureSession captureSession;
    QAudioInput audioInput;
};

#endif // AUDIO_HANDLER_H
