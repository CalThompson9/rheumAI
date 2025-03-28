/**
 * @file audiohandler.h
 * @brief Declaration of AudioHandler class
 *
 * @author Andres Pedreros (apedrero@uwo.ca)
 * @date Mar. 6, 2025
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 20, 2025
 */
#ifndef AUDIO_HANDLER_H
#define AUDIO_HANDLER_H

#include <string>
#include <iostream>
#include <QJsonArray>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QDir>
#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include "transcript.h"

/**
 * @class AudioHandler
 * @brief Singleton class for handling audio recording and transcription.
 * @details This class provides methods to record audio, transcribe it using Google Speech API,
 *          and manage microphone permissions. It uses QMediaRecorder for recording and QNetworkAccessManager
 *          for sending audio data to the API. The class is designed as a singleton to ensure
 *          that only one instance exists throughout the application.
 * @note The class is not thread-safe and should be used in a single-threaded context.
 */
class AudioHandler : public QObject
{
    Q_OBJECT

private:
    static AudioHandler *instance; // Singleton instance
    AudioHandler();                // Private constructor for Singleton

public:
    static AudioHandler *getInstance();             // Get the singleton instance
    Transcript transcribe(const QString &filename); // Transcribe audio file
    void startRecording(const QString &outputFile); // Start audio recording
    void stopRecording();                           // Stop audio recording
    void pauseRecording();                          // Pause audio recording
    void resumeRecording();                         // Resume audio recording
    void handlePermissionResponse();
    void playRecording(const QString &filePath);

    QString getAPIKey(const QString& prefix);
    void setOpenAIApiKey(const QString& key);
    double getAudioDuration(const QString& path) const;

    friend class Settings;

signals:
    void transcriptionCompleted(const QString &transcribedText); // Signal for transcription completion
    void microphonePermissionDenied();
    void microphonePermissionGranted();

private:
    QString googleSpeechApiKey;
    QString openAIApiKey;
    QNetworkAccessManager *networkManager;

    QString sendToWhisperAPI(const QString &audioPath); // Send audio to Google Speech API
    QString sendToGoogleSpeechAPI(const QString &audioPath);
    QTime getCurrentTime() const;                            // Get current time
    int getAudioChannelCount(const QString &audioPath) const; // Get audio channel count
    QString outputFilePath;                                  // Output file path for recording
    QMediaRecorder recorder;                                 // Media recorder for audio
    QMediaCaptureSession captureSession;                     // Media capture session
    QAudioInput *audioInput = nullptr; 

    void requestMicrophonePermission(); // Request microphone permission

};

#endif // AUDIO_HANDLER_H
