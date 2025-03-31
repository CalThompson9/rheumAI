/**
 * @file filehandler.cpp
 * @brief Definition of FileHandler class
 * @details This class handles file operations for the application, including reading
 * and writing patient records, transcripts, and summaries. It uses the singleton design
 * pattern to ensure that only one instance of the class exists throughout the application.
 * The class provides methods to save and load patient records, transcripts, and summaries
 * from JSON files. It also manages the archiving and unarchiving of patient records.
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Andres Pedreros (apedrero@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 16, 2025
 */

#include <QTextStream>
#include <QDebug>
#include "filehandler.h"

// Create an instance of the FileHandler class since it is a singleton
// This instance will be used to access the methods of the class
FileHandler *FileHandler::instance = nullptr;

/**
 * @name FileHandler (constructor)
 * @brief Initializes the FileHandler instance
 * @details Sets up the paths for the patient database and archived database.
 * Creates the directories if they do not exist.
 * @author Kalundi Segumaga
 */
FileHandler::FileHandler() : patientDatabasePath("Patients"),
                             archivedDatabasePath("Archived")
{
    QDir().mkpath(patientDatabasePath);
    QDir().mkpath(archivedDatabasePath);
}

/**
 * @name getInstance
 * @brief Returns the singleton instance of FileHandler
 * @details If the instance does not exist, it creates a new one.
 * This ensures that only one instance of FileHandler is created and used
 * throughout the application.
 * @return Singleton instance of FileHandler
 * @author Kalundi Serumaga
 */
FileHandler *FileHandler::getInstance()
{
    if (!instance)
        instance = new FileHandler();
    return instance;
}

/**
 * @name setTranscriptFilename
 * @brief Sets the filepath to the transcript file
 * @details This method is used to specify the file where the transcript data will be saved.
 * It is important to set this filepath before attempting to read or write transcript data.
 * @param[in] filepath: Filepath to transcript file
 * @author Kalundi Serumaga
 */
void FileHandler::setTranscriptFilename(const QString &filepath)
{
    transcriptFilename = filepath;
}

/**
 * @name setJsonFilename
 * @brief Sets the filepath to the patient information JSON file
 * @details This method is used to specify the file where the patient information
 * will be saved. It is important to set this filepath before attempting to read or
 * write patient information.
 * @param[in] filepath: Filepath to the patient information JSON file
 * @author Kalundi Serumaga
 */
void FileHandler::setJsonFilename(const QString &filepath)
{
    jsonFilename = filepath;
}

/**
 * @name readTranscript
 * @name Read transcript from file
 * @details Transcript filepath must first be set using `setTranscriptFilename`
 * @return Transcript data, or an empty string if the file could not be read
 * @author Kalundi Serumaga
 */
QString FileHandler::readTranscript()
{
    // If no transcript filename is set, return empty string
    if (transcriptFilename.isEmpty())
    {
        return "";
    }

    // Attempt to open the transcript file for reading
    QFile file(transcriptFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";// Return empty string if file can't be opened
    }

    // Read the entire contents of the file
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return content;// Return the loaded transcript content

}



/**
 * @name saveOrAppendRawTranscript
 * @brief Appends a timestamped transcript to the patient's daily raw file
 * @details Also updates `transcript_raw.txt` with full daily content for summarization
 * @param patientID The ID of the patient
 * @param transcript The transcript object to save
 * @author Kalundi Serumaga
 */

void FileHandler::saveOrAppendRawTranscript(int patientID, const Transcript &transcript) {
    QString folderPath = "Patients/" + QString::number(patientID);
    QDir().mkpath(folderPath);  // Ensure folder exists

    QString currentDate = QDate::currentDate().toString("yyyyMMdd");
    QString filePath = folderPath + "/raw_transcript_" + currentDate + ".txt";

    // Read existing file if it exists
    QString fileContents;
    QFile file(filePath);
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContents = in.readAll();
            file.close();
        }
    }

    // Append new content
    fileContents += "\n\nTimestamp: " + transcript.getTimestamp().toString("hh:mm:ss") + "\n\n";
    fileContents += transcript.getContent();

    // Write back the full updated content
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << fileContents;
        file.close();
    } else {
        qInfo() << "Failed to open file for writing: " << filePath;
    }

    // 🔁 Update transcript_raw.txt with full daily content for summary
    QString latestTranscriptPath = folderPath + "/transcript_raw.txt";
    QFile latestFile(latestTranscriptPath);
    if (latestFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream latestOut(&latestFile);
        latestOut << fileContents;
        latestFile.close();
    } else {
        qInfo() << "Failed to update transcript_raw.txt!";
    }
}

/**
 * @name loadSummaryText
 * @brief FileHandler::loadSummaryText
 * @param patientID The ID of the patient
 * @details The summary is stored in a file named `summary.txt` in the patient's folder.
 * @return The summary text for the patient
 * @author Kalundi Serumaga
 */
QString FileHandler::loadSummaryText(int patientID)
{
    // Build the path to the patient's summary file
    QString summaryPath = patientDatabasePath + "/" + QString::number(patientID) + "/summary.txt";
    QFile file(summaryPath);

     // Attempt to open the file for reading
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";// Return empty string if file can't be opened
    }

    // Read the entire contents of the summary file
    QTextStream in(&file);
    QString summaryContent = in.readAll();
    file.close();

    return summaryContent;// Return the loaded summary
}

/**
 * @name loadTranscript
 * @brief Loads the most recent transcript for a patient
 * @details Reads from `transcript_raw.txt` in the patient's folder
 * @param patientID The ID of the patient
 * @return The full transcript content, or an empty string if not found
 * @author Kalundi Serumaga
 */
QString FileHandler::loadTranscript(int patientID)
{
    // Build the path to the patient's raw transcript file
    QString transcriptPath = patientDatabasePath + "/" + QString::number(patientID) + "/transcript_raw.txt";
    QFile file(transcriptPath);

    // Attempt to open the file for reading
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }

    // Read the entire contents of the transcript file
    QTextStream in(&file);
    QString transcriptContent = in.readAll();
    file.close();

    return transcriptContent;// Return the loaded transcript
}

/**
 * @name savePatientRecord
 * @brief Saves the patient record to file
 * @details File will be named according to the patient ID stored in the patient
 * record, and located in the directory configured in the constructor.
 * @warning If the file already exists, it will be overwritten
 * @param[in] record: Patient record to save
 * @author Callum Thompson
 * @author Kalundi Serumaga
 */
void FileHandler::savePatientRecord(const PatientRecord &record)
{
    // Build the path to the patient's folder using their unique ID
    QString patientPath = patientDatabasePath + "/" + QString::number(record.getID());
    QDir().mkpath(patientPath); // Ensure patient folder exists

    // Open the file for writing patient info as JSON
    QFile file(patientPath + "/patient_info.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;// Exit if the file can't be opened
    }

    // Convert the PatientRecord to a JSON document and write it to file
    QJsonDocument doc(record.toJson());
    file.write(doc.toJson());
    file.close();
}

/**
 * @name saveTranscript
 * @brief Save the given transcript text to transcript_raw.txt for a specific patient
 * @details This method will overwrite the existing transcript file.
 * The file will be located in the patient's folder, which is determined by the patient ID.
 * @warning If the file already exists, it will be overwritten.
 * @param patientID ID of the patient whose transcript is being saved
 * @param transcript The full text of the transcript to write
 * @author Kalundi Serumaga
 * @author Callum Thompson
 */
void FileHandler::saveTranscript(int patientID, const QString &transcript)
{
    // Build the path to the patient's transcript file
    QString transcriptPath = patientDatabasePath + "/" + QString::number(patientID) + "/transcript_raw.txt";
    QFile file(transcriptPath);

    // Attempt to open the file for writing (overwrites existing content)
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;// Exit silently if file cannot be opened
    }

    file.resize(0);// Clear any existing contents before writing new data

    // Write the transcript text to the file
    QTextStream out(&file);
    out << transcript;
    file.close();
}

/**
 * @name loadPatientRecord
 * @brief Reads a patient record from file
 * @details File will be named according to the patient ID stored in the patient
 * record, and located in the directory configured in the constructor.
 * @param[in] patientID: Patient ID of record to read
 * @return Read patient record, or an empty record if the file could not be read
 * @author Callum Thompson
 * @author Kalundi Serumaga
 */
PatientRecord FileHandler::loadPatientRecord(int patientID)
{
    // Build the path to the patient's JSON file in the active database
    QString filePath = patientDatabasePath + "/" + QString::number(patientID) + "/patient_info.json";
    QFile file(filePath);

     // Try opening the patient file in the active database
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        // If that fails, try loading from the archive instead
        QString archivePath = archivedDatabasePath + "/" + QString::number(patientID) + "/patient_info.json";
        QFile archive(archivePath);
        if (!archive.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            return PatientRecord(); // Return an empty record if both fail
        }

        file.setFileName(archivePath);                    // Update file to point to the archive path
        file.open(QIODevice::ReadOnly | QIODevice::Text); // Open the archive file
    }

    // Read the contents of the file and parse it into a JSON document
    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);

    // Construct and return a PatientRecord object from the JSON data
    return PatientRecord::fromJson(doc.object());
}

/**
 * @name archivePatientRecord
 * @brief Moves a patient record to the archive folder
 * @details The patient record is moved from the 'Patients' folder to the 'Archived' folder.
 * The original patient folder is deleted after the move.
 * @param[in] patientID: Patient ID of record to archive
 * @return Patient record that was archived
 * @author Andres Pedreros
 * @author Kalundi Serumaga
 */
PatientRecord FileHandler::archivePatientRecord(int patientID)
{
    // Build paths for the active patient folder and destination archive folder
    QString patientPath = patientDatabasePath + "/" + QString::number(patientID);
    QString archivePath = archivedDatabasePath + "/" + QString::number(patientID);
    QDir().mkpath(archivePath); // Ensure archive folder exists

    QDir patientDir(patientPath);
    if (!patientDir.exists())
    {
        // If the patient folder doesn't exist, return an empty PatientRecord
        return PatientRecord();
    }

    // Move all files from the patient folder to the archive folder
    for (const QString &fileName : patientDir.entryList(QDir::Files))
    {
        QString srcFilePath = patientPath + "/" + fileName;
        QString destFilePath = archivePath + "/" + fileName;

        // If a file fails to move, log the error
        if (!QFile::rename(srcFilePath, destFilePath))
        {
            qInfo() << "Failed to move file:" << srcFilePath << "to" << destFilePath;
        }
    }

    // Remove the old patient folder
    if (!QDir(patientPath).removeRecursively())
    {
        qInfo() << "Failed to remove old patient folder:" << patientPath;
    }

    // Load and return the now-archived patient record
    return loadPatientRecord(patientID);
}

/**
 * @name unarchivePatientRecord
 * @brief Moves a patient record to the 'Patients' folder
 * @details The patient record is moved from the 'Archived' folder to the 'Patients' folder.
 * The original archive folder is deleted after the move.
 * @param[in] patientID: Patient ID of record to unarchive
 * @return Patient record that was unarchived
 * @author Andres Pedreros
 * @author Kalundi Serumaga
 */
PatientRecord FileHandler::unarchivePatientRecord(int patientID)
{
    // Build paths for the archived patient and the destination active patient directory
    QString archivePath = archivedDatabasePath + "/" + QString::number(patientID);
    QString patientPath = patientDatabasePath + "/" + QString::number(patientID);
    QDir().mkpath(patientPath); // Ensure archive folder exists

    QDir archiveDir(archivePath);
    if (!archiveDir.exists())
    {
        // If the archive folder doesn't exist, return an empty PatientRecord
        return PatientRecord();
    }

    // Move all files from the archived folder to the patients folder
    for (const QString &fileName : archiveDir.entryList(QDir::Files))
    {
        QString srcFilePath = archivePath + "/" + fileName;
        QString destFilePath = patientPath + "/" + fileName;

        // If a file fails to move, log the error
        if (!QFile::rename(srcFilePath, destFilePath))
        {
            qInfo() << "Failed to move file:" << srcFilePath << "to" << destFilePath;
        }
    }

    // Remove the old patient folder
    if (!QDir(archivePath).removeRecursively())
    {
        qInfo() << "Failed to remove old archive folder:" << archivePath;
    }

    // Load and return the reactivated patient record
    return loadPatientRecord(patientID);
}

/**
 * @name saveTranscriptToJson
 * @brief Convert the currently stored transcript text into JSON format and save it
 * @details Transcript filepath must first be set using `setTranscriptFilename`. 
 * The JSON file will be saved in the same directory as the transcript file.
 * @author Kalundi Serumaga
 * @author Callum Thompson
 */
void FileHandler::saveTranscriptToJson()
{
    // Get transcript content from the raw transcript file
    QString transcriptText = readTranscript();
    if (transcriptText.isEmpty())
        return;// Skip saving if transcript is empty


         // Create a JSON object to hold the transcript data
        QJsonObject jsonObj;
        jsonObj["patientID"] = 12345;   // Mock Patient ID to be replaced with actual ID
        jsonObj["date"] = "2024-03-07"; // Mock Date to be replaced with actual date
        jsonObj["transcript"] = transcriptText;
    
        QJsonDocument jsonDoc(jsonObj);

    // Attempt to open the JSON file for writing
    QFile jsonFile(jsonFilename);
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qInfo() << "Could not open JSON file for writing";
        return;
    }

    // Write JSON data to file and close
    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();
}

/**
 * @name getTranscriptFilename
 * @brief Get the filepath to the transcript file
 * @details This method returns the filepath to the transcript file
 * that was previously set using `setTranscriptFilename`.
 * @return Filepath to the transcript file
 * @author Kalundi Serumaga
 */
QString FileHandler::getTranscriptFilename() const
{
    return transcriptFilename;
}

/**
 * @name getJsonFilename
 * @brief Gets the filepath to the patient information JSON file
 * @details This method returns the filepath to the patient information
 * JSON file that was previously set using `setJsonFilename`.
 * @return Filepath to the patient information JSON file
 * @author Kalundi Serumaga
 */
QString FileHandler::getJsonFilename() const
{
    return jsonFilename;
}

/**
 * @name loadPatientJson
 * @brief Read the raw text stored in the patient information JSON file and
 * print it
 * @details This method reads the JSON file and prints the contents to the console.
 * It is useful for debugging purposes to ensure that the JSON data is being
 * correctly loaded and parsed.
 * @details Patient information filepath must first be set using `setJsonFilename`
 * @author Kalundi Serumaga
 */
void FileHandler::loadPatientJson()
{
    QFile jsonFile(jsonFilename);

    // Attempt to open the JSON file in read-only text mode
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return; // Exit if file can't be opened
    }

    // Read the entire file content into a QByteArray
    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    // Parse the raw JSON data into a QJsonDocument and extract the root object
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObj = jsonDoc.object();
}

