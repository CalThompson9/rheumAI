#include "mainwindow.h"
#include "llmclient.h"
#include "audiohandler.h"
#include "filehandler.h"
#include "patientrecord.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set screen bounds
    setGeometry(0, 0, 1200, 800);

    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Use WindowBuilder to set up UI
    WindowBuilder::setupUI(centralWidget, btnConnectDevice, btnSettings,
                           lblTitle, lblPatientName, comboSelectPatient,
                           btnRecord, textTranscription, mainLayout, btnAddPatient);

    // Initialize LLM client
    llmClient = new LLMClient(this);
    connect(llmClient, &LLMClient::responseReceived, this, &MainWindow::handleLLMResponse);
    connect(btnAddPatient, &QPushButton::clicked, this, &MainWindow::on_addPatientButton_clicked);

    // Initialize AudioHandler and connect transcription signal to LLMClient
    AudioHandler *audioHandler = AudioHandler::getInstance();
    connect(audioHandler, &AudioHandler::transcriptionCompleted, llmClient, &LLMClient::sendRequest);

    // Connect "Record" button to start and stop recording
    connect(btnRecord, &QPushButton::clicked, this, [audioHandler, this]()
            {
        static bool isRecording = false;
        if (isRecording)
        {
            audioHandler->stopRecording();
            QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();


            // Construct absolute path to output.wav
            QString filePath = QDir(projectDir).filePath("output.wav");

            audioHandler->transcribe(filePath.toStdString());
            btnRecord->setText("Start Recording");
        }
        else
        {
            audioHandler->startRecording("output2.wav");
            btnRecord->setText("Stop Recording");
        }
        isRecording = !isRecording; });
}

void MainWindow::handleLLMResponse(const QString &response)
{
    textTranscription->setPlainText(response);
}

void MainWindow::on_addPatientButton_clicked()
{
    int patientID = 12345; // Temporary for testing
    QString firstName = "John";
    QString lastName = "Doe";
    QString dateOfBirth = "1990-01-01";

    PatientRecord newPatient(patientID, firstName, lastName, dateOfBirth);
    FileHandler::getInstance()->savePatientRecord(newPatient);

    qDebug() << "New patient added!";
}

MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}
