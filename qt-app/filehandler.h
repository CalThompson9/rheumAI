/**
 * @file filehandler.h
 * @brief Declaration of FileHandler class
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Andres Pedreros (apedrero@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 16, 2025
 */

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include "patientrecord.h"
#include "summary.h"
#include "transcript.h"

/**
 * @class FileHandler
 * @brief Handles file operations for patient records and transcripts
 *
 * @details This class provides methods to save, load, and manage patient records,
 * transcripts, and JSON files. It follows the Singleton design pattern to
 * ensure only one instance of the class exists throughout the application.
 * @author Kalundi Serumaga
 * @author Joelene Hales
 * @author Callum Thompson
 * @author Andres Pedreros
 */
class FileHandler
{
private:
    static FileHandler *instance;
    QString transcriptFilename;
    QString jsonFilename;
    QString patientDatabasePath;
    QString archivedDatabasePath;

    FileHandler(); // Private constructor (Singleton pattern)

public:
    static FileHandler *getInstance(); // Singleton access

    void setTranscriptFilename(const QString &filepath);
    void setJsonFilename(const QString &filepath);
    void savePatientRecord(const PatientRecord &record);
    void loadPatientJson(); // Read and display JSON
    void saveTranscript(int patientID, const QString &transcript);
    void saveTranscriptToJson(); // Convert transcript to JSON
    void saveOrAppendRawTranscript(int patientID, const Transcript &transcript);


    PatientRecord loadPatientRecord(int patientID);
    PatientRecord archivePatientRecord(int patientID);
    PatientRecord unarchivePatientRecord(int patientID);

    QString getTranscriptFilename() const;
    QString getJsonFilename() const;
    QString readTranscript(); // Read raw transcript file
    QString loadSummaryText(int patientID);
    QString loadTranscript(int patientID);
};

#endif // FILEHANDLER_H
