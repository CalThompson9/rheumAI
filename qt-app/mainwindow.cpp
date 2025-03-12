#include "mainwindow.h"
#include "llmclient.h"
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


    // Connect "Record" button to LLM API request
    connect(btnRecord, &QPushButton::clicked, this, [this]() {
        QFile file(":/sample_transcript.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open sample_transcript.txt. Request aborted.";
            return;
        }
        QString prompt = QTextStream(&file).readAll().trimmed();
        llmClient->sendRequest(prompt);
        file.close();
    });
}

void MainWindow::handleLLMResponse(const QString &response)
{
    textTranscription->setPlainText(response);
}

void MainWindow::on_addPatientButton_clicked() {
    int patientID = 12345;  // Temporary for testing
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
