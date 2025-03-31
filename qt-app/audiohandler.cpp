/**
 * @file audiohandler.cpp
 * @brief Definition of AudioHandler class
 *
 * Handles audio recording and transcription using Google Speech-to-Text API.
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @date Mar. 6, 2025
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 21, 2025
 */

// Libraries
#include <QMediaDevices>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QCoreApplication>
#include <QHttpMultiPart>
#include <QHttpPart>
#if QT_CONFIG(permissions)
#include <QPermission>
#endif
#include "audiohandler.h"

// Since this is a singleton, we need to declare the static instance
AudioHandler *AudioHandler::instance = nullptr;

/**
 * @name AudioHandler (Constructor)
 * @brief Initializes the AudioHandler instance
 * @details Sets up the network manager. The API keys are initialized by
 * Settings.
 * @see Settings
 * @author Andres Pedreros Castro
 */
AudioHandler::AudioHandler() : QObject(nullptr)
{
    networkManager = new QNetworkAccessManager(this);
}

/**
 * @name getInstance
 * @brief Returns the singleton instance of AudioHandler
 * @details If the instance does not exist, it creates a new one.
 * This ensures that only one instance of AudioHandler exists throughout the application.
 * @note This is a singleton pattern implementation.
 * @author Joelene Hales
 * @return Singleton instance of AudioHandler
 */
AudioHandler *AudioHandler::getInstance()
{
    // Create the singleton instance if it doesn't already exist
    if (!instance)
    {
        instance = new AudioHandler();
    }

    // Return the shared AudioHandler instance
    return instance;
}

/**
 * @name transcribe
 * @brief Transcribes the audio file using Google Speech-to-Text API
 * @details This function checks the duration and channel count of the audio file.
 * If the duration is longer than 60 seconds or if the audio is not stereo (2 channels),
 * it uses the Whisper API for transcription. Otherwise, it uses Google Speech-to-Text API.
 * It returns a Transcript object containing the transcribed text.
 * @note The function emits a signal when the transcription is completed.
 * @see getAudioDuration
 * @param[in] filename: Path to the audio file
 * @return Transcript object containing the transcribed text
 * @author Andres Pedreros Castro
 * @author Callum Thompson
 */
Transcript AudioHandler::transcribe(const QString &filename)
{
    // Determine audio properties
    double durationSecs = getAudioDuration(filename);
    int channelCount = getAudioChannelCount(filename);

    QString response;

    // Use Whisper if longer than 60s or not stereo (2 channels)
    if (durationSecs > 60.0 || channelCount != 2)
    {
        qInfo() << "Using Whisper (OpenAI)";
        response = sendToWhisperAPI(filename);
    }
    else
    {
        qInfo() << "Using Google Speech-to-Text";
        response = sendToGoogleSpeechAPI(filename);
    }

    // Handle failure to get a valid response
    if (response.isEmpty())
    {
        emit transcriptionCompleted("Transcription failed");
        return Transcript(getCurrentTime(), "");
    }

     // Parse the JSON response
    QString result;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if (!doc.isObject())
    {
        emit transcriptionCompleted("Invalid response format");
        return Transcript(getCurrentTime(), "");
    }

    // Extract transcribed text depending on which service was used
    if (durationSecs > 60.0 || channelCount != 2)
    {
        // Whisper returns a flat "text" field
        result = doc.object().value("text").toString();
    }
    else
    {
         // Google Speech returns a nested array of results and alternatives
        QJsonArray results = doc.object().value("results").toArray();
        for (const QJsonValue &val : results)
        {
            QJsonArray alternatives = val.toObject().value("alternatives").toArray();
            if (!alternatives.isEmpty())
            {
                result += alternatives[0].toObject().value("transcript").toString();
            }
        }
    }

    // Emit signal and return the transcript with timestamp
    emit transcriptionCompleted(result);
    return Transcript(getCurrentTime(), result);
}

/**
 * @name getAudioChannelCount
 * @brief Retrieves the number of audio channels in a WAV file
 * @details This function reads the WAV file header to determine the number of channels.
 * It assumes the file is in WAV format and uses a specific byte offset to read the channel count.
 * @note This function is specific to WAV files and may not work for other audio formats.
 * @see getAudioDuration
 * @param[in] audioPath: Path to the audio file
 * @return Number of audio channels
 * @author Andres Pedreros Castro
 */
int AudioHandler::getAudioChannelCount(const QString &audioPath) const
{
    QFile file(audioPath);

    // Try to open the audio file in read-only mode
    if (!file.open(QIODevice::ReadOnly))
        return -1;

    file.seek(22); // Byte offset for number of channels in a WAV file header
    char buffer[2];
    file.read(buffer, 2);
    file.close();

    // Combine the two bytes into a single 16-bit integer (little-endian)
    int channels = static_cast<unsigned char>(buffer[0]) |
                   (static_cast<unsigned char>(buffer[1]) << 8);
    return channels;
}

/**
 * @name sendToWhisperAPI
 * @brief Sends the audio file to Google Speech-to-Text API for transcription
 * @details This function prepares the audio file and sends it to the Whisper API for transcription.
 * It sets the necessary headers and handles the response.
 * It uses QNetworkAccessManager to send the request and receive the response.
 * @note The function emits a signal when the transcription is completed.
 * @param[in] audioPath: Path to the audio file
 * @return Response from the API as a string
 * @author Callum Thompson
 */
QString AudioHandler::sendToWhisperAPI(const QString &audioPath)
{
    // Abort if OpenAI API key is missing
    if (openAIApiKey.isEmpty())
    {
        qWarning() << "OpenAI API Key is empty!";
        return "";
    }

    // Set up the OpenAI Whisper endpoint and request
    QUrl url("https://api.openai.com/v1/audio/transcriptions");
    QNetworkRequest request(url);

    // Add authorization header using bearer token
    QString bearerToken = "Bearer " + openAIApiKey;
    request.setRawHeader("Authorization", bearerToken.toUtf8());

    // Create multipart form data for the POST request
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Prepare the audio file for upload
    QFile *file = new QFile(audioPath);
    if (!file->open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open file for Whisper API:" << audioPath;
        delete file;
        return "";
    }

    // Add the audio file to the multipart body
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(audioPath).fileName() + "\""));
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // Ensure file is cleaned up with multipart
    multiPart->append(filePart);

    // Specify the model name ("whisper-1")
    QHttpPart modelPart;
    modelPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"model\""));
    modelPart.setBody("whisper-1");
    multiPart->append(modelPart);

    // Send the POST request
    QNetworkReply *reply = networkManager->post(request, multiPart);
    multiPart->setParent(reply); // Cleanup when reply is finished

    // Block execution until the request is done (synchronous wait)
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Handle the response
    QString response;
    if (reply->error() == QNetworkReply::NoError)
    {
        response = reply->readAll();
    }
    else
    {
        emit badRequest(reply); // Signal UI or logger about the failed request
        qWarning() << "Whisper request failed:" << reply->errorString();
    }

    reply->deleteLater();
    return response;
}


/**
 * @name sendToGoogleSpeechAPI
 * @brief Sends the audio file to Google Speech-to-Text API for transcription
 * @details This function prepares the audio file and sends it to the Google Speech-to-Text API for transcription.
 * It sets the necessary headers and handles the response.
 * It uses QNetworkAccessManager to send the request and receive the response.
 * @note The function emits a signal when the transcription is completed.
 * @param[in] audioPath: Path to the audio file
 * @return Response from the API as a string
 * @author Andres Pedreros Castro
 */
QString AudioHandler::sendToGoogleSpeechAPI(const QString &audioPath)
{
    // Construct the Google Speech-to-Text API URL with your API key
    QUrl url("https://speech.googleapis.com/v1/speech:recognize?key=" + googleSpeechApiKey);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Load the audio file
    QFile file(audioPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Could not open audio file: " << audioPath;
        return "";
    }

    // Read and encode audio file as base64
    QByteArray audioData = file.readAll();
    file.close();
    QString base64Audio = audioData.toBase64();

    // Configure audio settings for Google's STT API
    QJsonObject config;
    config["encoding"] = "LINEAR16";            // WAV file format
    config["sampleRateHertz"] = 48000;          // Sample rate used by your mic or recorder
    config["languageCode"] = "en-CA";           // Canadian English
    config["audioChannelCount"] = 2;            // Stereo audio

    // Insert the encoded audio into the request
    QJsonObject audio;
    audio["content"] = base64Audio;

    // Combine configuration and audio into final JSON body
    QJsonObject root;
    root["config"] = config;
    root["audio"] = audio;

    // Serialize the JSON payload
    QJsonDocument doc(root);
    QByteArray jsonData = doc.toJson();

    // Send the POST request
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // Wait synchronously for the reply to complete
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Read and return the API response
    QString response;
    if (reply->error() == QNetworkReply::NoError)
    {
        response = reply->readAll();
    }
    else
    {
        emit badRequest(reply); // Notify system about failed request
        qWarning() << "Google request failed:" << reply->errorString();
    }

    reply->deleteLater();
    return response;
}


/**
 * @name startRecording
 * @brief Starts audio recording
 * @details This function initializes the audio input and recorder,
 * sets the output file, and starts recording.
 * It also requests microphone permission if not already granted.
 * @note The function emits a signal when the recording starts.
 * @see requestMicrophonePermission
 * @see QAudioInput
 * @param[in] outputFile: Path to the output file
 * @return void
 * @author Andres Pedreros Castro
 */
void AudioHandler::startRecording(const QString &outputFile)
{
    requestMicrophonePermission();

    // Get available microphones
    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    if (devices.isEmpty())
    {
        qWarning() << "No microphone detected!";
        return;
    }

    QAudioDevice defaultMic = devices.first();
    qInfo() << "🎤 Using microphone:" << defaultMic.description();

    // Delete previous audio input if it exists
    if (audioInput != nullptr)
    {
        delete audioInput;
        audioInput = nullptr;
    }

    // Create QAudioInput with the validated format
    audioInput = new QAudioInput(defaultMic);
    captureSession.setAudioInput(audioInput);
    captureSession.setRecorder(&recorder);

    // Set up recorder output
    QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();
    QString filePath = QDir(projectDir).filePath(outputFile);
    recorder.setOutputLocation(QUrl::fromLocalFile(filePath));

    // Set media format to Wave
    QMediaFormat mediaFormat;
    mediaFormat.setFileFormat(QMediaFormat::Wave);
    recorder.setMediaFormat(mediaFormat);

    // Apply required settings to recorder
    recorder.setAudioSampleRate(48000);
    recorder.setAudioChannelCount(2);

    recorder.record();
}

/**
 * @name pauseRecording
 * @brief Pauses audio recording
 * @details This function pauses the audio recording.
 * It can be resumed later using the resumeRecording() function.
 * @author Andres Pedreros Castro
 * @return void
 */
void AudioHandler::pauseRecording()
{
    recorder.pause();
}

/**
 * @name resumeRecording
 * @brief Resumes audio recording
 * @details This function resumes the audio recording after it has been paused.
 * It continues recording from the point where it was paused.
 * @author Andres Pedreros Castro
 * @return void
 */
void AudioHandler::resumeRecording()
{
    recorder.record();
}

/**
 * @name stopRecording
 * @brief Stops audio recording
 * @details This function stops the audio recording and finalizes the output file.
 * It can be called after recording is complete or if the user wants to stop recording.
 * @author Andres Pedreros Castro
 * @return void
 */
void AudioHandler::stopRecording()
{
    recorder.stop();
}

/**
 * @name getCurrentTime
 * @brief Retrieves the current time
 * @details This function returns the current time using QTime.
 * It can be used to timestamp the recorded audio or for other purposes.
 * @note This function does not require any parameters.
 * @see QTime
 * @return Current time
 * @author Andres Pedreros Castro
 */
QTime AudioHandler::getCurrentTime() const
{
    return QTime::currentTime();
}

/**
 * @name requestMicrophonePermission
 * @brief Requests permission to use the microphone
 * @details This function checks if the microphone permission is granted.
 * If the permission is not granted, it requests permission from the user.
 * It emits a signal when the permission is granted or denied.
 * @details Uses the Qt Permissions API to request microphone access
 * @author Callum Thompson
 * @return void
 */
void AudioHandler::requestMicrophonePermission()
{
#if QT_CONFIG(permissions)
    // Create a permission object specific to microphone access
    QMicrophonePermission microphonePermission;

    // Check the current status of microphone permission
    switch (qApp->checkPermission(microphonePermission))
    {
    case Qt::PermissionStatus::Undetermined:
        // Permission hasn't been asked yet — request it
        qApp->requestPermission(microphonePermission, this, &AudioHandler::handlePermissionResponse);
        return;
    case Qt::PermissionStatus::Denied:
        // Permission was explicitly denied by the user
        qWarning() << "Microphone permission denied!";
        emit microphonePermissionDenied();// Notify other parts of the app
        return;
    case Qt::PermissionStatus::Granted:
        // Permission already granted
        qInfo() << "Microphone permission granted!";
        break;
    }
#else
    // This block runs if the Qt permissions module is not available in the current build
    qWarning() << "⚠️ Qt permissions API is disabled!";
#endif
}

/**
 * @name handlePermissionResponse
 * @brief Handles the response to the microphone permission request
 * @details Emits signals based on the user's response to the permission request.
 * If the permission is granted, it emits microphonePermissionGranted().
 * If the permission is denied, it emits microphonePermissionDenied().
 * @see requestMicrophonePermission
 * @author Callum Thompson
 * @return void
 */
void AudioHandler::handlePermissionResponse()
{
    // Check the result of the microphone permission request
    QMicrophonePermission microphonePermission;
    if (qApp->checkPermission(microphonePermission) == Qt::PermissionStatus::Granted)
    {
        // User accepted the microphone permission request
        qInfo() << "User granted microphone permission.";
        emit microphonePermissionGranted();
    }
    else
    {
        // User denied the microphone permission request
        qInfo() << "User denied microphone permission.";
        emit microphonePermissionDenied();
    }
}

/**
 * @name playRecording
 * @brief Plays the recorded audio file
 * @details This function uses the system command to play the recorded audio file.
 * It uses the `afplay` command on macOS to play the audio file.
 * @note This function assumes the audio file is in a format supported by the system player.
 * @param[in] filePath: Path to the audio file
 * @return void
 * @author Andres Pedreros Castro
 */
void AudioHandler::playRecording(const QString &filePath)
{
    // Check if the specified audio file exists
    if (!QFile::exists(filePath))
    {
        qWarning() << "Recorded file does not exist: " << filePath;
        return;
    }
    // Use the macOS 'afplay' command to play the audio file
    QString command = "afplay " + filePath; // macOS playback command
    system(command.toUtf8().constData());// Execute the command via system shell
}

/**
 * @name setGoogleAIApiKey
 * @brief Sets the Google Text-to-Speech API key
 * @details This function sets the Google Text-to-Speech API key for the
 * AudioHandler instance.  It can be used to update the API key if needed.
 * @param[in] key: Google Text-to-Speech API key
 * @author Joelene Hales
 */
void AudioHandler::setGoogleApiKey(const QString &key)
{
    googleSpeechApiKey = key;
}

/**
 * @name setOpenAIApiKey
 * @brief Sets the OpenAI API key
 * @details This function sets the OpenAI API key for the AudioHandler instance.
 * It can be used to update the API key if needed.
 * @param[in] key: OpenAI API key
 * @author Andres Pedreros Castro
 */
void AudioHandler::setOpenAIApiKey(const QString &key)
{
    openAIApiKey = key;
}

/**
 * @name getAudioDuration
 * @brief Retrieves the duration of an audio file
 * @details This function reads the audio file and calculates its duration
 * based on the file size and audio format.
 * It assumes a specific format (16-bit PCM, 2 channels, 48000Hz) for the calculation.
 * @note This function is specific to WAV files and may not work for other audio formats.
 * @param[in] audioPath: Path to the audio file
 * @return Duration of the audio file in seconds
 * @author Andres Pedreros Castro
 */
double AudioHandler::getAudioDuration(const QString &audioPath) const
{
    QFile file(audioPath);

    // Attempt to open the audio file
    if (!file.open(QIODevice::ReadOnly))
        return 0;// Return 0 on failure

    // Get the total size of the file in bytes
    qint64 fileSize = file.size();
    file.close();

    // Assume 16-bit PCM, 2 channels, 48000Hz => 192000 bytes/sec
    return fileSize / 192000.0;
}
