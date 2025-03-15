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
    optionDetailedLayout->setEnabled(false);

    // Initialize SummaryGenerator
    summaryGenerator = new SummaryGenerator(this);

    // Connect the signal to process the generated summary when ready
    connect(summaryGenerator, &SummaryGenerator::summaryReady, this, &MainWindow::handleSummaryReady);

    // THIS IS A MOCK FUNCTION CALL JUST FOR TESTING ON PROGRAM START
    handleSummarizeButtonClicked();
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
    displaySummary(summaryGenerator->getSummary());

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
 * @details Starts a new summary generation process via the LLM
 */
void MainWindow::handleSummarizeButtonClicked()
{
    // Create a new transcript for LLM summarization
    QFile file(":/sample_transcript.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open. Request aborted.";
        return;
    }
    QString sample_transcript = QTextStream(&file).readAll().trimmed();
    file.close();

    Transcript* testTranscript = new Transcript(QTime::currentTime(), sample_transcript);

    // Send the request to the LLM
    summaryGenerator->sendRequest(*testTranscript);
}

/**
 * @name handleSummaryReady
 * @brief Processes and displays the structured summary after LLM response
 */
void MainWindow::handleSummaryReady()
{
    // Retrieve structured summary from SummaryGenerator
    Summary summary = summaryGenerator->getSummary();

    // Update the UI with the summary
    displaySummary(summary);
}

/**
 * @name on_addPatientButton_clicked
 * @brief Handles adding a new patient record
 */
void MainWindow::on_addPatientButton_clicked() {
    int patientID = 12345;  // Temporary for testing
    QString firstName = "John";
    QString lastName = "Doe";
    QString dateOfBirth = "1990-01-01";

    PatientRecord newPatient(patientID, firstName, lastName, dateOfBirth);
    FileHandler::getInstance()->savePatientRecord(newPatient);
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
