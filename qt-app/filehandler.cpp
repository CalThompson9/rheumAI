/**
 * @file filehandler.cpp
 * @brief Definition of FileHandler class
 * 
 * Handles reading and writing patient files.
 * 
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @date Mar. 16, 2025
 */

#include "filehandler.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

FileHandler* FileHandler::instance = nullptr;

/**
 * @name FileHandler (constructor)
 * @brief Initializes the FileHandler instance
 */
FileHandler::FileHandler() {
    patientDatabasePath = "Patients";  // Base directory
    QDir().mkpath(patientDatabasePath);  // Ensure it exists
}

/**
 * @name getInstance
 * @brief Returns the singleton instance of FileHandler
 * @return Singleton instance of FileHandler
 */
FileHandler* FileHandler::getInstance() {
    if (!instance)
        instance = new FileHandler();
    return instance;
}

/**
 * @name setTranscriptFilename
 * @brief Sets the filepath to the transcript file
 * @param[in] filepath: Filepath to transcript file
 */
void FileHandler::setTranscriptFilename(const QString &filepath) {
    transcriptFilename = filepath;
}

/**
 * @name setJsonFilename
 * @brief Sets the filepath to the patient information JSON file
 * @param[in] filepath: Filepath to the patient information JSON file
 */
void FileHandler::setJsonFilename(const QString &filepath) {
    jsonFilename = filepath;
}

/**
 * @name readTranscript
 * @name Read transcript from file
 * @details Transcript filepath must first be set using `setTranscriptFilename`
 * @return Transcript data, or an empty string if the file could not be read
 */
QString FileHandler::readTranscript() {
    if (transcriptFilename.isEmpty()) {
        qDebug() << "Transcript file is not set!";
        return "";
    }

    QFile file(transcriptFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open transcript file for reading";
        return "";
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    qDebug() << "Transcript Read:\n" << content;
    return content;
}


QString FileHandler::loadSummaryText(int patientID) {
    QString summaryPath = patientDatabasePath + "/" + QString::number(patientID) + "/summary.txt";
    QFile file(summaryPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "❌ No summary found for patient:" << patientID;
        return "";
    }

    QTextStream in(&file);
    QString summaryContent = in.readAll();
    file.close();

    qDebug() << "✅ Loaded summary from file (before returning):\n" << summaryContent;
    return summaryContent;
}


QString FileHandler::loadTranscript(int patientID) {
    QString transcriptPath = patientDatabasePath + "/" + QString::number(patientID) + "/transcript_raw.txt";
    QFile file(transcriptPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No transcript found for patient:" << patientID;
        return "";
    }

    QTextStream in(&file);
    QString transcriptContent = in.readAll();
    file.close();

    return transcriptContent;
}


/**
 * @name savePatientRecord
 * @brief Saves the patient record to file
 * @details File will be named according to the patient ID stored in the patient
 * record, and located in the directory configured in the constructor.
 * @warning If the file already exists, it will be overwritten
 * @param[in] record: Patient record to save
 */
void FileHandler::savePatientRecord(const PatientRecord &record) {
    QString patientPath = patientDatabasePath + "/" + QString::number(record.getID());
    QDir().mkpath(patientPath);  // Ensure patient folder exists

    QFile file(patientPath + "/patient_info.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not save patient record!";
        return;
    }

    QJsonDocument doc(record.toJson());
    file.write(doc.toJson());
    file.close();
    qDebug() << "Patient record saved to:" << file.fileName();
}

void FileHandler::saveTranscript(int patientID, const QString &transcript) {
    QString transcriptPath = patientDatabasePath + "/" + QString::number(patientID) + "/transcript_raw.txt";
    QFile file(transcriptPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not save transcript!";
        return;
    }

    file.resize(0);

    QTextStream out(&file);
    out << transcript;
    file.close();
    qDebug() << "Transcript saved to:" << transcriptPath;
}


/**
 * @name loadPatientRecord
 * @brief Reads a patient record from file
 * @param[in] patientID: Patient ID of record to read
 * @return Read patient record, or an empty record if the file could not be read
 */
PatientRecord FileHandler::loadPatientRecord(int patientID) {
    QString filePath = patientDatabasePath + "/" + QString::number(patientID) + "/patient_info.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not load patient record!";
        return PatientRecord();
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    qDebug() << "Patient record loaded for ID:" << patientID;
    return PatientRecord::fromJson(doc.object());
}


/**
 * @name saveTranscriptToJson
 * @brief Convert the currently stored transcript text into JSON format and save it 
 * @details Transcript filepath must first be set using `setTranscriptFilename`.
 */
void FileHandler::saveTranscriptToJson() {
    QString transcriptText = readTranscript();
    if (transcriptText.isEmpty()) return;

    // Convert to JSON format
    QJsonObject jsonObj;
    jsonObj["patientID"] = 12345;  // Mock Patient ID
    jsonObj["date"] = "2024-03-07";  // Mock Date
    jsonObj["transcript"] = transcriptText;

    QJsonDocument jsonDoc(jsonObj);

    QFile jsonFile(jsonFilename);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open JSON file for writing";
        return;
    }

    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();
    qDebug() << "Transcript saved as JSON to:" << jsonFilename;
}

/**
 * @name getTranscriptFilename
 * @brief Get the filepath to the transcript file
 * @return Filepath to the transcript file
 */
QString FileHandler::getTranscriptFilename() const {
    return transcriptFilename;
}

/**
 * @name getJsonFilename
 * @brief Gets the filepath to the patient information JSON file
 * @return Filepath to the patient information JSON file
 */
QString FileHandler::getJsonFilename() const {
    return jsonFilename;
}

/**
 * @name loadPatientJson
 * @brief Read the raw text stored in the patient information JSON file and
 * print it
 * @details Patient information filepath must first be set using `setJsonFilename`
 */
void FileHandler::loadPatientJson() {
    QFile jsonFile(jsonFilename);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open JSON file for reading";
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObj = jsonDoc.object();
    qDebug() << "Loaded JSON data:" << jsonObj;
}
