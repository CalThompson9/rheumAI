#include "mainwindow.h"
#include "llmclient.h"
#include "detailedsummaryformatter.h"
#include "concisesummaryformatter.h"
#include "filehandler.h"
#include "patientrecord.h"
#include "transcript.h"
#include "summarygenerator.h"

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
                           btnRecord, btnSummarize, textTranscription, 
                           selectSummaryLayout, summarySection,
                           mainLayout, btnAddPatient);

    // Add summary layout options
    summaryLayoutOptions = new QMenu(this);
    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Layout");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Layout");

    selectSummaryLayout->setMenu(summaryLayoutOptions);

    // Connect selection of each option to update summary layout format
    connect(optionDetailedLayout, &QAction::triggered, this, [=]() {
        handleSummaryLayoutChanged(new DetailedSummaryFormatter);
    });
    connect(optionConciseLayout, &QAction::triggered, this, [=]() {
        handleSummaryLayoutChanged(new ConciseSummaryFormatter);
    });

    // Initialize summary layout formatter
    summaryFormatter = new DetailedSummaryFormatter;
    displaySummary(testSummary);
    optionDetailedLayout->setEnabled(false);

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

    // Connect "Summarize" button to summarize transcripts and update window
    connect(btnSummarize, &QPushButton::clicked, this, &MainWindow::handleSummarizeButtonClicked);
}

void MainWindow::handleLLMResponse(const QString &response)
{
    textTranscription->setPlainText(response);
}

/**
 * @name handleSummaryLayoutChanged
 * @brief Handler function called when a new summary layout format is selected
 * @details Updates the window to display the summary with the selected format.
 * Does not regenerate the summary. Updates the layout selection menu to disable
 * the currently selected option.
 * @param[in] summaryFormatter: Summary formatter corresponding to the selected
 * layout style
 */
void MainWindow::handleSummaryLayoutChanged(SummaryFormatter* summaryFormatter)
{
    // Display summary with selected layout format
    setSummaryFormatter(summaryFormatter);
    displaySummary(testSummary);

    // Update options menu
    QAction* selectedOption = qobject_cast<QAction*>(sender());
    for (QAction* layoutAction : summaryLayoutOptions->actions())
    {
        layoutAction->setEnabled(layoutAction != selectedOption);
    }
}

/**
 * @name handleSummarizeButtonClicked
 * @brief Handler function called when summarize button is clicked
 * @details Regenerates the summary and displays in the window
 */
void MainWindow::handleSummarizeButtonClicked()
{
    // Regenerate summary
    Transcript testTranscript(QTime::currentTime(), QString("Test Transcript"));  // FIXME: Replace this dummy transcript with list of actual transcripts, in patient file
    SummaryGenerator summaryGenerator(testTranscript);

    testSummary = summaryGenerator.summarizeSymptoms().summarizeDiagnoses().summarizeMedicalHistory().summarizeTreatmentPlans().getSummary();

    displaySummary(testSummary);  // Update window
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
