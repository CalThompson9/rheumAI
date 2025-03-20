#include "filehandler.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

FileHandler* FileHandler::instance = nullptr;

FileHandler::FileHandler() {
    patientDatabasePath = "Patients";  // Base directory
    QDir().mkpath(patientDatabasePath);  // Ensure it exists
}

FileHandler* FileHandler::getInstance() {
    if (!instance)
        instance = new FileHandler();
    return instance;
}

void FileHandler::setTranscriptFilename(const QString &filepath) {
    transcriptFilename = filepath;
}

void FileHandler::setJsonFilename(const QString &filepath) {
    jsonFilename = filepath;
}

// Read the transcript from the file
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



QString FileHandler::getTranscriptFilename() const {
    return transcriptFilename;
}

QString FileHandler::getJsonFilename() const {
    return jsonFilename;
}


// Read JSON and print it
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
