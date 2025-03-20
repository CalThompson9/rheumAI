#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "patientrecord.h"
#include "summary.h"

class FileHandler {
private:
    static FileHandler* instance;
    QString transcriptFilename;
    QString jsonFilename;
    QString patientDatabasePath;

    FileHandler();  // Private constructor (Singleton pattern)


public:
    static FileHandler* getInstance();  // Singleton access

    void setTranscriptFilename(const QString &filepath);
    void setJsonFilename(const QString &filepath);
    void setDatabasePath(const QString &path);
    void savePatientRecord(const PatientRecord &record);
    void loadPatientJson();  // Read and display JSON

    PatientRecord loadPatientRecord(int patientID);

    QString getTranscriptFilename() const;
    QString getJsonFilename() const;
    QString readTranscript();  // Read raw transcript file
    QString loadSummaryText(int patientID);
    QString loadTranscript(int patientID);

};

#endif // FILEHANDLER_H
