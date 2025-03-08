#include <QMenu>

#include "mainwindow.h"
#include "llmclient.h"
#include "detailedsummaryformatter.h"
#include "concisesummaryformatter.h"
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
                           btnRecord, textTranscription, selectSummaryLayout, 
                           summarySection, mainLayout, btnAddPatient);

    // FIXME: For testing only, remove once actual functionality is implemented 
    /////////////////////////////////////////////////////////////     
    testSummary.setSymptoms("Symptoms...");
    testSummary.setMedicalHistory("Medical history....");
    testSummary.setTreatmentPlans("Treatment plans....");
    testSummary.setDiagnoses("Diagnoses....");
    displaySummary(testSummary);
    /////////////////////////////////////////////////////////////

    // Add layout options
    QMenu* summaryLayoutOptions = new QMenu(this);
    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Layout");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Layout");

    selectSummaryLayout->setMenu(summaryLayoutOptions);

    // Connect actions to slot
    connect(optionDetailedLayout, &QAction::triggered, this, [=]() {
        setSummaryFormatter(new DetailedSummaryFormatter);
        displaySummary(testSummary);
        for (QAction* layoutAction : summaryLayoutOptions->actions())
        {
            layoutAction->setEnabled(layoutAction != optionDetailedLayout);
        }
    });
    connect(optionConciseLayout, &QAction::triggered, this, [=]() {
        setSummaryFormatter(new ConciseSummaryFormatter);
        displaySummary(testSummary);
        for (QAction* layoutAction : summaryLayoutOptions->actions())
        {
            layoutAction->setEnabled(layoutAction != optionConciseLayout);
        }
    });

    // Initialize summary layout formatter
    summaryFormatter = new DetailedSummaryFormatter;
    optionDetailedLayout->setEnabled(false);

    // Initialize LLM client
    llmClient = new LLMClient(this);
    connect(llmClient, &LLMClient::responseReceived, this, &MainWindow::handleLLMResponse);
    connect(btnAddPatient, &QPushButton::clicked, this, &MainWindow::on_addPatientButton_clicked);

    // Connect "Record" button to LLM API request
    connect(btnRecord, &QPushButton::clicked, this, [this]() {
        llmClient->sendRequest("Hello, AI! How are you?");
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

/**
 * @name setSummaryLayout
 * @brief Set the formatter used to create the summary
 * @param[in] summaryFormatter: Summary layout formatter
 */
void MainWindow::setSummaryFormatter(SummaryFormatter* newSummaryFormatter)
{
    delete summaryFormatter;
    summaryFormatter = newSummaryFormatter;
}

/**
 * @name displaySummary
 * @brief Display summary using the configured layout
 * @param[in] summary: Summary to display
 */
void MainWindow::displaySummary(const Summary& summary)
{
    summaryFormatter->generateLayout(summary, summarySection);
}

MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}
