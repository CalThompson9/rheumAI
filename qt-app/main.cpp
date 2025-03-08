#include "mainwindow.h"
#include <QApplication>
#include "filehandler.h"
#include "patientrecord.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>  // Include this for debugging

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Initialize and show the main window
    MainWindow w;
    w.show();

    // TEST: Convert transcript to JSON (Temporary for Debugging)
    FileHandler* fileHandler = FileHandler::getInstance();
    fileHandler->setTranscriptFilename(":/sample_transcript.txt");
    fileHandler->setJsonFilename("patient_12345.json");

    // DEBUG: Print expected file paths
    qDebug() << "Expected Transcript Path: " << fileHandler->getTranscriptFilename();
    qDebug() << "Expected JSON Path: " << fileHandler->getJsonFilename();

    fileHandler->saveTranscriptToJson();
    fileHandler->loadPatientJson();

    return a.exec();
}
