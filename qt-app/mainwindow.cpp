#include "mainwindow.h"
#include "llmclient.h"
#include "audiohandler.h"
#include "detailedsummaryformatter.h"
#include "concisesummaryformatter.h"
#include "filehandler.h"
#include "patientrecord.h"
#include "transcript.h"
#include "summarygenerator.h"
#include "addpatientdialog.h"
#include <QMessageBox> 


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(0, 0, 1200, 800);

    // Create central widget

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    WindowBuilder::setupUI(centralWidget, btnConnectDevice, btnSettings,
                           lblTitle, lblPatientName, comboSelectPatient,
                           btnRecord, btnSummarize,
                           selectSummaryLayout, summarySection,
                           mainLayout, btnAddPatient, btnRemovePatient);

    // Add summary layout options
    summaryLayoutOptions = new QMenu(this);
    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Layout");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Layout");

    selectSummaryLayout->setMenu(summaryLayoutOptions);

    // Connect selection of each option to update summary layout format
    connect(optionDetailedLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(new DetailedSummaryFormatter); });
    connect(optionConciseLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(new ConciseSummaryFormatter); });

    // Initialize summary layout formatter
    summaryFormatter = new DetailedSummaryFormatter;
    optionDetailedLayout->setEnabled(false);


    // Initialize SummaryGenerator
    summaryGenerator = new SummaryGenerator(this);

    // Connect the signal to process the generated summary when ready
    connect(summaryGenerator, &SummaryGenerator::summaryReady, this, &MainWindow::handleSummaryReady);
    // Initialize AudioHandler and connect transcription signal to LLMClient
    AudioHandler *audioHandler = AudioHandler::getInstance();

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

            audioHandler->transcribe(filePath);
            btnRecord->setText("Start Recording");
        }
        else
        {
            audioHandler->startRecording("output.wav");
            btnRecord->setText("Stop Recording");
        }
        isRecording = !isRecording; });

    connect(audioHandler, &AudioHandler::transcriptionCompleted, this, &MainWindow::handleSummarizeButtonClicked);

    // THIS IS A MOCK FUNCTION CALL JUST FOR TESTING ON PROGRAM START


    handleSummarizeButtonClicked();

    llmClient = new LLMClient(this);
    connect(llmClient, &LLMClient::responseReceived, this, &MainWindow::handleLLMResponse);
    connect(btnAddPatient, &QPushButton::clicked, this, &MainWindow::on_addPatientButton_clicked);
    connect(btnRemovePatient, &QPushButton::clicked, this, &MainWindow::on_removePatientButton_clicked);
    connect(comboSelectPatient, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_patientSelected);


    // Connect "Record" button to LLM API request

    // NEW: Load existing patients on startup**
     loadPatientsIntoDropdown();


    connect(btnRecord, &QPushButton::clicked, this, [this]() {
        //llmClient->sendRequest("Hello, AI! How are you?");

         // Get selected patient ID
         QVariant patientData = comboSelectPatient->currentData();
         if (!patientData.isValid()) {
             QMessageBox::warning(this, "No Patient Selected", "Please select a patient before recording.");
             return;
         }
         int patientID = patientData.toInt();

         // Create a test transcript file
         QString filePath = "Patients/" + QString::number(patientID) + "/transcript_raw.txt";
         QFile file(filePath);
         if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QTextStream out(&file);
             out << "This is a test transcript for patient " << patientID << "\n";
             file.close();
             qDebug() << "Test transcript saved to: " << filePath;
         } else {
             qDebug() << "Failed to save transcript!";
         }
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
void MainWindow::handleSummaryLayoutChanged(SummaryFormatter *summaryFormatter)
{
    // Display summary with selected layout format
    setSummaryFormatter(summaryFormatter);
    displaySummary(summaryGenerator->getSummary());

    // Update options menu
    QAction *selectedOption = qobject_cast<QAction *>(sender());
    for (QAction *layoutAction : summaryLayoutOptions->actions())
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
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open. Request aborted.";
        return;
    }
    QString sample_transcript = QTextStream(&file).readAll().trimmed();
    file.close();

    Transcript *testTranscript = new Transcript(QTime::currentTime(), sample_transcript);

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


void MainWindow::loadPatientsIntoDropdown() {
    comboSelectPatient->clear();  // Clear dropdown before loading
    qDebug() << "Loading patients from Patients folder...";
    QDir patientsDir("Patients");

    if (!patientsDir.exists()) {
        qDebug() << "Patients directory does not exist!";
        return;
    }

    QStringList patientFolders = patientsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folderName : patientFolders) {
        int patientID = folderName.toInt();  // Convert folder name to int
        PatientRecord patient = FileHandler::getInstance()->loadPatientRecord(patientID);
        QString displayName = patient.getFirstName() + " " + patient.getLastName() + " [" + QString::number(patientID) + "]";
        comboSelectPatient->addItem(displayName, patientID);
    }

    qDebug() << "Loaded patients into dropdown.";

}

/**
 * @name setSummaryLayout
 * @brief Set the formatter used to create the summary
 * @param[in] summaryFormatter: Summary layout formatter
 */
void MainWindow::setSummaryFormatter(SummaryFormatter *newSummaryFormatter)
{
    delete summaryFormatter;
    summaryFormatter = newSummaryFormatter;
}

/**
 * @name displaySummary
 * @brief Display summary using the configured layout
 * @param[in] summary: Summary to display
 */
void MainWindow::displaySummary(const Summary &summary)
{
    summaryFormatter->generateLayout(summary, summarySection);
}

void MainWindow::on_addPatientButton_clicked() {
    AddPatientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString firstName = dialog.getFirstName();
        QString lastName = dialog.getLastName();
        QString dateOfBirth = dialog.getDateOfBirth();
        QString baseName = firstName + " " + lastName;  // No DOB in dropdown

        int patientID = QDateTime::currentMSecsSinceEpoch() % 100000;

        // 🔹 Check if a patient with the same name already exists
        int duplicateCount = 0;
        for (int i = 0; i < comboSelectPatient->count(); ++i) {
            QVariant storedID = comboSelectPatient->itemData(i);
            PatientRecord existingPatient = FileHandler::getInstance()->loadPatientRecord(storedID.toInt());

            if (existingPatient.getFirstName() == firstName &&
                existingPatient.getLastName() == lastName &&
                existingPatient.getDateOfBirth() == dateOfBirth) {
                QMessageBox::warning(this, "Duplicate Patient",
                                     "A patient with this name and birthdate already exists!");
                return;
            }

            // If the same name exists, increase counter
            if (comboSelectPatient->itemText(i).startsWith(baseName)) {
                duplicateCount++;
            }
        }

        // 🔹 If duplicate, add a counter [1], [2], etc.
        QString displayName = baseName;
        if (duplicateCount > 0) {
            displayName += " [" + QString::number(duplicateCount) + "]";
        }

        // 🔹 Create and save new patient
        PatientRecord newPatient(patientID, firstName, lastName, dateOfBirth);
        FileHandler::getInstance()->savePatientRecord(newPatient);

        comboSelectPatient->addItem(displayName, patientID);
        qDebug() << "New patient added: " << patientID << " - " << displayName;
    }
}



void MainWindow::on_removePatientButton_clicked() {
    int index = comboSelectPatient->currentIndex();
    if (index == -1) return;  // No patient selected

    int patientID = comboSelectPatient->currentData().toInt();
    QString patientFolder = "Patients/" + QString::number(patientID);

    // Remove patient folder
    if (QDir(patientFolder).removeRecursively()) {
        qDebug() << "Patient record deleted: " << patientID;
    } else {
        qDebug() << "Failed to delete patient record!";
    }

    // Remove from dropdown
    comboSelectPatient->removeItem(index);
}

void MainWindow::on_patientSelected(int index) {
    if (index == -1) return;

    int patientID = comboSelectPatient->currentData().toInt();
    qDebug() << "Selected patient: " << patientID;

    lblPatientName->setText("Patient ID: " + QString::number(patientID));
}



MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}
