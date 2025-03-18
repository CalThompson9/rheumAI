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

    // Settings
    settings = new Settings(this, summaryGenerator->llmClient);
    connect(btnSettings, &QPushButton::clicked, this, &MainWindow::showSettings);
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

/**
 * @name showSettings
 * @brief Handles constructing and handling the settings pop-up menu.
 * @todo [OPTIONAL] Move function to Settings class.
 */
void MainWindow::showSettings()
{
    // ========== Layout ==========
    QDialog *settingsWindow = new QDialog(this);
    settingsWindow->setWindowTitle("Settings");
    settingsWindow->setGeometry(0, 0, 500, 250);

    QVBoxLayout *mainLayout = new QVBoxLayout(settingsWindow);

    // ========== Connected Peripherals ==========
    QVBoxLayout *peripheralsLayout = new QVBoxLayout();
    QLabel *cpLabel = new QLabel("Connected Peripherals:", settingsWindow);
    peripheralsLayout->addWidget(cpLabel);

    // Create a read-only text field to display a structured list of peripherals.
    // Here we use a QTextEdit styled with a white background and gray border.
    QTextEdit *peripheralsField = new QTextEdit(settingsWindow);
    peripheralsField->setReadOnly(true);
    peripheralsField->setFixedHeight(60); // Adjust height as needed
    peripheralsField->setStyleSheet("background-color: white; border: 1px solid gray;");

    // Example: Set the text for the microphone connection status.
    // Replace 'true' with the actual check for your hardware.
    if (true)
        peripheralsField->setText("Microphone ✅");
    else
        peripheralsField->setText("Microphone ❌");

    peripheralsLayout->addWidget(peripheralsField);
    mainLayout->addLayout(peripheralsLayout);

    // ========== LLM API Key ==========
    QHBoxLayout *llmLayout = new QHBoxLayout();
    QLabel *llmLabel = new QLabel("LLM API Key:", settingsWindow);
    QLineEdit *llmKeyField = new QLineEdit(settingsWindow);
    QDialogButtonBox *llmButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    llmLayout->addWidget(llmLabel);
    llmLayout->addWidget(llmKeyField);
    llmLayout->addWidget(llmButtonBox);
    mainLayout->addLayout(llmLayout);

    // When OK is clicked, set the key if non-empty
    connect(llmButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!llmKeyField->text().isEmpty()) {
            settings->setLLMKey(llmKeyField->text());
        } else {
            qWarning() << "This field cannot be empty.";
        }
    });
    // When Cancel is clicked, just clear the field
    connect(llmButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        llmKeyField->clear();
    });

    // ========== Whisper API Key ==========
    QHBoxLayout *wsprLayout = new QHBoxLayout();
    QLabel *wsprLabel = new QLabel("Whisper API Key:", settingsWindow);
    QLineEdit *wsprKeyField = new QLineEdit(settingsWindow);
    QDialogButtonBox *wsprButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    wsprLayout->addWidget(wsprLabel);
    wsprLayout->addWidget(wsprKeyField);
    wsprLayout->addWidget(wsprButtonBox);
    mainLayout->addLayout(wsprLayout);

    connect(wsprButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!wsprKeyField->text().isEmpty()) {
            settings->setWsprKey(wsprKeyField->text());
        } else {
            qWarning() << "This field cannot be empty.";
        }
    });
    connect(wsprButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        wsprKeyField->clear();
    });

    // ========== Save & Close ==========
    QPushButton *saveCloseButton = new QPushButton("Save and close", settingsWindow);
    mainLayout->addWidget(saveCloseButton);

    connect(saveCloseButton, &QPushButton::clicked, settingsWindow, &QDialog::close);

    // ========== Button sty ==========
    const QString blueStyle = R"(
        QPushButton {
            background-color: #5371ff;
            color: white;
            border-radius: 8px;
            padding: 10px;
            margin: 0 4px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #425BD0;
        }
        QPushButton:pressed {
            background-color: #006ae6;
        }
    )";
    const QString cancelStyle = R"(
        QPushButton {
            background-color: #AAAAAA;
            color: black;
            border-radius: 8px;
            padding: 10px;
            margin: 0 4px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #949494;
        }
        QPushButton:pressed {
            background-color: #5F5F5F;
        }
    )";

    QPushButton *llmOkButton = llmButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *llmCancelButton = llmButtonBox->button(QDialogButtonBox::Cancel);
    QPushButton *wsprOkButton = wsprButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *wsprCancelButton= wsprButtonBox->button(QDialogButtonBox::Cancel);

    // Disable default style
    llmOkButton->setDefault(false);
    llmOkButton->setAutoDefault(false);
    wsprOkButton->setDefault(false);
    wsprOkButton->setAutoDefault(false);

    llmOkButton->setStyleSheet(blueStyle);
    llmCancelButton->setStyleSheet(cancelStyle);
    wsprOkButton->setStyleSheet(blueStyle);
    wsprCancelButton->setStyleSheet(cancelStyle);
    saveCloseButton->setStyleSheet(blueStyle);

    settingsWindow->exec();
    delete settingsWindow;
}

MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}
