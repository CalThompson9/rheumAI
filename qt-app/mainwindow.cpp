/**
 * @file mainwindow.cpp
 * @brief Definition of MainWindow class
 * 
 * Main window owns and manages the lifetime UI elements, processes button
 * press signals, and manages summary generation.
 * 
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Thomas Llamzon (tllamazon@uwo.ca)
 * @date Mar. 1, 2025
 */

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
#include <QMediaDevices>
#include <QAudioDevice>
#include <QTimer>



/**
 * @name MainWindow (constructor)
 * @brief Initializes a MainWindow instance
 * @param[in] parent: Parent widget
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(0, 0, 1200, 800);


    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    WindowBuilder::setupUI(centralWidget, btnSettings,
                           lblTitle, lblPatientName, comboSelectPatient,
                           btnRecord, btnSummarize,
                           selectSummaryLayout, summarySection,
                           mainLayout, btnAddPatient, btnRemovePatient);

    // Add summary layout options
    summaryLayoutOptions = new QMenu(this);
    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Layout");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Layout");
    QAction *optionPlainLayout = summaryLayoutOptions->addAction("Plain Text");

    selectSummaryLayout->setMenu(summaryLayoutOptions);
    selectSummaryLayout->setText("Detailed Layout");

    // Connect selection of each option to update summary layout format
    connect(optionDetailedLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(new DetailedSummaryFormatter); });
    connect(optionConciseLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(new ConciseSummaryFormatter); });
    connect(optionPlainLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(nullptr); });

    // Initialize SummaryGenerator
    summaryGenerator = new SummaryGenerator(this);

    // Connect the signal to process the generated summary when ready
    connect(summaryGenerator, &SummaryGenerator::summaryReady, this, &MainWindow::handleSummaryReady);

    // Initialize AudioHandler and connect transcription signal to LLMClient
    AudioHandler *audioHandler = AudioHandler::getInstance();

    // Initialize settings
    settings = Settings::getInstance(this, summaryGenerator->llmClient, audioHandler);
    connect(btnSettings, &QPushButton::clicked, settings, &Settings::showSettings);
    connect(settings, &Settings::okButtonClicked, this, &MainWindow::handleSummarizeButtonClicked);

    // TODO: need to set the

    // Initialize summary layout formatter from settings
    QString defaultSummaryLayout = settings->getSummaryPreference();
    selectSummaryLayout->setText(defaultSummaryLayout);
    if (defaultSummaryLayout.contains("Detailed Layout"))
    {
        qDebug() << "Setting default summary format to: Detailed Layout";
        summaryFormatter = new DetailedSummaryFormatter;
        optionDetailedLayout->setEnabled(false);
    }
    else if (defaultSummaryLayout.contains("Concise Layout"))
    {
        qDebug() << "Setting default summary format to: Concise Layout";
        summaryFormatter = new ConciseSummaryFormatter;
        optionConciseLayout->setEnabled(false);
    }
    else
    {
        qDebug() << "Unrecognized summary format in settings: " <<  defaultSummaryLayout << ". Using detailed layout instead...";
        summaryFormatter = new DetailedSummaryFormatter;
        optionDetailedLayout->setEnabled(false);
    }

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

            // Getting trancription and saving it to file
            Transcript currentTranscription = audioHandler->transcribe(filePath);
            qDebug() << "Transcription: " << currentTranscription.getContent();
            FileHandler::getInstance()->saveTranscript(patientID, currentTranscription.getContent());

            btnRecord->setText("Start Recording");
        }
        else
        {
            audioHandler->startRecording("output.wav");
            btnRecord->setText("Stop Recording");
        }
        isRecording = !isRecording; });

    connect(audioHandler, &AudioHandler::transcriptionCompleted, this, &MainWindow::handleSummarizeButtonClicked);

    llmClient = new LLMClient(this);
    connect(llmClient, &LLMClient::responseReceived, this, &MainWindow::handleLLMResponse);
    connect(btnAddPatient, &QPushButton::clicked, this, &MainWindow::on_addPatientButton_clicked);
    connect(btnRemovePatient, &QPushButton::clicked, this, &MainWindow::on_removePatientButton_clicked);
    connect(comboSelectPatient, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_patientSelected);

    // Connect "Record" button to LLM API request

    // NEW: Load existing patients on startup**
    loadPatientsIntoDropdown();
        if (comboSelectPatient->count() > 0) {
        QTimer::singleShot(100, this, [this]() {
            comboSelectPatient->setCurrentIndex(0);
            on_patientSelected(0);
        });
    }

    // Connect "Summarize" button to summarize transcripts and update window
    connect(btnSummarize, &QPushButton::clicked, this, &MainWindow::handleSummarizeButtonClicked);
}

/**
 * @name handleLLMResponse
 * @brief Handler function called when the LLM returns a response
 * @details Sets the plan text transcription displayed in the main window
 * @param[in] response: Response returned by the LLM
 */
void MainWindow::handleLLMResponse(const QString &response)
{
    textTranscription->setPlainText(response);

    // Get selected patient ID
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid()) {
        qWarning() << "No patient selected, cannot save transcript!";
        return;
    }
    int patientID = patientData.toInt();

    // Ensure the patient's folder exists
    QDir patientDir("Patients/" + QString::number(patientID));
    if (!patientDir.exists()) {
        patientDir.mkpath(".");
    }

    // Save the transcript to file
    QString transcriptPath = patientDir.filePath("transcript_raw.txt");
    QFile file(transcriptPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << response;
        file.close();
        qDebug() << "Transcript saved to: " << transcriptPath;
    } else {
        qDebug() << "Failed to save transcript!";
    }
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
    QAction *selectedOption = qobject_cast<QAction *>(sender());

    // Clear existing summary display
    QLayoutItem *child;
    while ((child = summarySection->takeAt(0)) != nullptr) {
        delete child->widget();  // Ensure previous widget is deleted
        delete child;
    }

    if (selectedOption->text() == "Plain Text") {
        qDebug() << "Switching to plain text (transcript) mode.";

        // Ensure the transcript is actually loaded
        if (currentTranscriptText.isEmpty()) {
            qDebug() << "No transcript available.";
            return;
        }

        // Create a QLabel inside the summary section (INSTEAD OF ADDING ANYTHING ELSE)
        QLabel *transcriptLabel = new QLabel(currentTranscriptText);
        transcriptLabel->setWordWrap(true);  // Ensure proper wrapping
        transcriptLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);  // Positioning fix

        // Add it inside the summary layout
        summarySection->addWidget(transcriptLabel);

        qDebug() << "Transcript displayed inside summary section.";
    }
    else {
        // Display summary with the selected format
        setSummaryFormatter(summaryFormatter);
        displaySummary(summaryGenerator->getSummary());
    }

    // Update menu UI
    for (QAction *layoutAction : summaryLayoutOptions->actions()) {
        layoutAction->setEnabled(layoutAction != selectedOption);
        selectSummaryLayout->setText(selectedOption->text());
    }
}



/**
 * @name handleSummarizeButtonClicked
 * @brief Handler function called when summarize button is clicked
 * @details Starts a new summary generation process via the LLM
 */
void MainWindow::handleSummarizeButtonClicked()
{
    // Get selected patient ID
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid()) {
        QMessageBox::warning(this, "No Patient Selected", "Please select a patient before summarizing.");
        return;
    }
    int patientID = patientData.toInt();
    
    // Construct file path for the transcript
    QString transcriptPath = "Patients/" + QString::number(patientID) + "/transcript_raw.txt";
    
    // Open and read the transcript
    QFile file(transcriptPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open transcript. Request aborted.";
        return;
    }

    currentTranscriptText = QTextStream(&file).readAll().trimmed();
    file.close();

    // Create a transcript object
    Transcript *transcript = new Transcript(QTime::currentTime(), currentTranscriptText);

    // Send transcript to the LLM
    summaryGenerator->sendRequest(*transcript);
}


/**
 * @name handleSummaryReady
 * @brief Processes and displays the structured summary after LLM response
 */
void MainWindow::handleSummaryReady()
{
    // Retrieve structured summary from SummaryGenerator
    Summary summary = summaryGenerator->getSummary();

    // Get selected patient ID
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid()) {
        qWarning() << "No patient selected, cannot save summary!";
        return;
    }
    int patientID = patientData.toInt();

    // Construct summary file path
    QString summaryPath = "Patients/" + QString::number(patientID) + "/summary.txt";
    
    // Save summary to file
    QFile file(summaryPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << summary.getText();
        file.close();
        qDebug() << "Summary saved to: " << summaryPath;
    } else {
        qDebug() << "Failed to save summary!";
    }

    // Update the UI with the summary
    displaySummary(summary);
    btnSummarize->setText("Regenerate Summary");
}

/**
 * @name loadPatientsIntoDropdown
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
    if (!summaryFormatter) {
        qDebug() << "Warning: summaryFormatter is null, cannot display summary.";
        return;
    }

    summaryFormatter->generateLayout(summary, summarySection);
}

/**
 * @name on_addPatientButton_clicked
 * @brief Handler function called when the "Add Patient" button is pressed
 * @details Displays a dialog window that prompts the user to enter the
 * patient's details. Once the user submits the form, a new record file is created
 * with the input data, and the user interface is updated.
 */
void MainWindow::on_addPatientButton_clicked() {
    AddPatientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString firstName = dialog.getFirstName();
        QString lastName = dialog.getLastName();
        QString dateOfBirth = dialog.getDateOfBirth();
        QString baseName = firstName + " " + lastName;  // No DOB in dropdown

        int patientID = QDateTime::currentMSecsSinceEpoch() % 100000;

        // Check if a patient with the same name already exists
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

        // If duplicate, add a counter [1], [2], etc.
        QString displayName = baseName;
        if (duplicateCount > 0) {
            displayName += " [" + QString::number(duplicateCount) + "]";
        }

        // Create and save new patient
        PatientRecord newPatient(patientID, firstName, lastName, dateOfBirth);
        FileHandler::getInstance()->savePatientRecord(newPatient);

        // Update user interface to show new patient in dropdown
        comboSelectPatient->addItem(displayName, patientID);
        qDebug() << "New patient added: " << patientID << " - " << displayName;

        // Refresh dropdown
        loadPatientsIntoDropdown();
    }
}


/**
 * @name on_removePatientButton_clicked
 * @brief Handler function called when the "Remove Patient" button is pressed
 * @details Removes the record file for the selected patient, and updates the
 * user interface.
 */
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

    // Update user interface to remove patient from dropdown
    comboSelectPatient->removeItem(index);
}

/**
 * @name on_patientSelected
 * @brief Handles the signal that a new patient has been selected
 * @details Updates the UI to display the select patient's ID
 * @param[in] index: Index of option selected in the dropdown
 */
void MainWindow::on_patientSelected(int index) {
    if (index == -1) return;

    patientID = comboSelectPatient->currentData().toInt();
    qDebug() << "Selected patient: " << patientID;

    viewPatient();

    // Load the transcript
    QString savedTranscript = FileHandler::getInstance()->loadTranscript(patientID);
    if (!savedTranscript.isEmpty()) {
        qDebug() << "Transcript found, storing in currentTranscriptText.";
        currentTranscriptText = savedTranscript;  // Store transcript
    } else {
        qDebug() << "No transcript found, clearing UI.";
        currentTranscriptText.clear();
    }

    // Load the structured summary
    qDebug() << "Attempting to load summary for patient ID: " << patientID;
    QString savedSummaryText = FileHandler::getInstance()->loadSummaryText(patientID);

    if (!savedSummaryText.isEmpty()) {
        qDebug() << "Summary found, sending to SummaryGenerator...";
        summaryGenerator->setSummaryText(savedSummaryText);

        Summary summary = summaryGenerator->getSummary();
        qDebug() << "Summary successfully retrieved, attempting to display...";
        
        displaySummary(summary);
        btnSummarize->setText("Regenerate Summary");
        qDebug() << "Summary display completed.";
    } else {
        qDebug() << "No saved summary found.";
        // Clear the summary UI
        QLayoutItem *child;
        while ((child = summarySection->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();  // Safely delete the widget
            }
            delete child;  // Free the layout item
        }
        btnSummarize->setText("Summarize");

    }
}

/**
 * @name viewPatient
 * @brief When switched to a patient, display their information in the top corner
 * @details This function is called when a patient is selected from the dropdown
 */
void MainWindow::viewPatient() {
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid()) {
        qWarning() << "No patient selected, cannot view patient!";
        return;
    }

    int patientID = patientData.toInt();
    PatientRecord patient = FileHandler::getInstance()->loadPatientRecord(patientID);

    QString info = "Name: " + patient.getFirstName() + " " + patient.getLastName() +
                   " \nDOB: " + patient.getDateOfBirth() +
                   " \nID: " + QString::number(patientID);
    lblPatientName->setText(info);
}


MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}
