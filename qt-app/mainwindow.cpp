/**
 * @file mainwindow.cpp
 * @brief Definition of MainWindow class
 *
 * @details Main window owns and manages the lifetime UI elements, processes button
 * press signals, and manages summary generation.
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Thomas Llamzon (tllamzon@uwo.ca)
 * @date Mar. 1, 2025
 */

#include "mainwindow.h"

/**
 * @name MainWindow (constructor)
 * @brief Initializes a MainWindow instance
 * @details Sets up the UI elements and connects signals to slots. 
 * Initializes the SummaryGenerator and AudioHandler instances.
 * Sets up the layout and connects buttons to their respective handlers.
 * @param[in] parent: Parent widget
 * @author Callum Thompson
 * @author Andres Pedreros Castro
 * @author Joelene Hales
 * @author Thomas Llamzon
 * @author Kalundi Serumaga
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), archiveMode(false)
{
    setGeometry(0, 0, 1200, 800);

    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    WindowBuilder::setupUI(centralWidget, btnSettings,
                           lblTitle, lblPatientName, comboSelectPatient,
                           btnRecord, btnSummarize,
                           selectSummaryLayout, summarySection, summaryTitle,
                           mainLayout, btnAddPatient, btnEditPatient, btnDeletePatient, btnArchivePatient,
                           toggleSwitch); // Pass toggleSwitch to WindowBuilder
    checkDropdownEmpty(); // Disable all relevant actions on startup if the user has no patients

    // Connect archive mode button
    connect(toggleSwitch, &QPushButton::clicked, this, &MainWindow::handleArchiveToggled);

    // Initialize SummaryGenerator
    summaryGenerator = new SummaryGenerator(this);

    // Connect the signal to process the generated summary when ready
    connect(summaryGenerator, &SummaryGenerator::summaryReady, this, &MainWindow::handleSummaryReady);

    // Configure API handlers
    AudioHandler *audioHandler = AudioHandler::getInstance();
    llmClient = LLMClient::getInstance();
    connect(audioHandler, &AudioHandler::badRequest, this, &MainWindow::endLoading);
    connect(llmClient, &LLMClient::invalidAPIKey, this, &MainWindow::endLoading);

    // Initialize settings
    settings = Settings::getInstance(this);
    connect(btnSettings, &QPushButton::clicked, settings, &Settings::showSettings);

    // Add summary layout options
    summaryLayoutOptions = new QMenu(this);
    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Summary");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Summary");
    QAction *optionPlainLayout = summaryLayoutOptions->addAction("Plain Text Transcript");

    selectSummaryLayout->setMenu(summaryLayoutOptions);

    // Connect selection of each option to update summary layout format
    connect(optionDetailedLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(new DetailedSummaryFormatter); });
    connect(optionConciseLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(new ConciseSummaryFormatter); });
    connect(optionPlainLayout, &QAction::triggered, this, [=]()
            { handleSummaryLayoutChanged(nullptr); });

    // Initialize summary layout formatter from settings default summary layout configuration
    QString defaultSummaryLayout = settings->getSummaryPreference();
    selectSummaryLayout->setText(defaultSummaryLayout);
    if (defaultSummaryLayout.contains("Detailed Summary"))
    {
        summaryFormatter = new DetailedSummaryFormatter;
        optionDetailedLayout->setEnabled(false);
    }
    else if (defaultSummaryLayout.contains("Concise Summary"))
    {
        summaryFormatter = new ConciseSummaryFormatter;
        optionConciseLayout->setEnabled(false);
    }
    else // Default the default (if not set) to detailed format
    {
        summaryFormatter = new DetailedSummaryFormatter;
        optionDetailedLayout->setEnabled(false);
    }

    // Connect "Record" button to start and stop recording
    connect(btnRecord, &QPushButton::clicked, this, [audioHandler, this]()
    {
        static bool isRecording = false;
        if (isRecording) {  // If currently recording and button is pressed, stop recording
            audioHandler->stopRecording();

            QString projectDir = QDir(QCoreApplication::applicationDirPath()).absolutePath();
            QString filePath = QDir(projectDir).filePath("output.wav");

            // If a patient is not selected to record for, do not let the user do this
            QVariant patientData = comboSelectPatient->currentData();
            if (!patientData.isValid()) {
                QMessageBox::warning(this, "No Patient Selected", "Please select a patient before recording.");
                return;
            }
            int selectedPatientID = patientData.toInt();
            patientID = selectedPatientID;

            // Display non-modal loading dialog
            loadingDialog->setModal(false);  // Temporarily change loading dialog to be non-modal
            loadingDialog->show();

            Transcript currentTranscription = audioHandler->transcribe(filePath);

            //  Overwrite 'transcript_raw.txt' for summarizer
            FileHandler::getInstance()->saveTranscript(selectedPatientID, currentTranscription.getContent());

            //  Append to timestamped raw log
            FileHandler::getInstance()->saveOrAppendRawTranscript(selectedPatientID, currentTranscription);

            btnRecord->setText("Start Recording");

            // Re-enable ADD, DELETE, ARCHIVE, SUMMARIZE, VIEW ARCHIVED
            // PATIENTS, SETTINGS, SELECT PATIENT buttons
            btnAddPatient->setEnabled(true);
            btnDeletePatient->setEnabled(false);
            btnArchivePatient->setEnabled(false);
            btnSummarize->setEnabled(true);
            toggleSwitch->setEnabled(true);
            btnSettings->setEnabled(true);
            comboSelectPatient->setEnabled(true);
            btnAddPatient->setStyleSheet(WindowBuilder::blueButtonStyle);
            btnDeletePatient->setStyleSheet(WindowBuilder::redButtonStyle);
            btnArchivePatient->setStyleSheet(WindowBuilder::orangeButtonStyle);
            btnSummarize->setStyleSheet(WindowBuilder::orangeButtonStyle);
            toggleSwitch->setStyleSheet(WindowBuilder::blueButtonStyle);

            loadingDialog->hide(); // Hide loading dialog after recording is finished
            loadingDialog->setModal(true);  // Set loading dialog as modal for next use
        }
        else { // If not recording and button is pressed, start recording
            audioHandler->startRecording("output.wav");
            btnRecord->setText("Stop Recording");

            // Disable ADD, DELETE, ARCHIVE, SUMMARIZE, VIEW ARCHIVED
            // PATIENTS, SETTINGS, SELECT PATIENT buttons
            btnAddPatient->setEnabled(false);
            btnDeletePatient->setEnabled(false);
            btnArchivePatient->setEnabled(false);
            btnSummarize->setEnabled(false);
            toggleSwitch->setEnabled(false);
            btnSettings->setEnabled(false);
            comboSelectPatient->setEnabled(false);
            btnAddPatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
            btnDeletePatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
            btnArchivePatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
            btnSummarize->setStyleSheet(WindowBuilder::disabledButtonStyle);
            toggleSwitch->setStyleSheet(WindowBuilder::disabledButtonStyle);
        }
        isRecording = !isRecording; // Toggle recording mode
    });

    // Connect mainWindow functions to their associated actions
    connect(audioHandler, &AudioHandler::transcriptionCompleted, this, &MainWindow::handleSummarizeButtonClicked);
    connect(llmClient, &LLMClient::responseReceived, this, &MainWindow::handleLLMResponse);

    // Connect mainWindow buttons to their associated actions
    connect(btnAddPatient, &QPushButton::clicked, this, &MainWindow::on_addPatientButton_clicked);
    connect(btnEditPatient, &::QPushButton::clicked, this, &MainWindow::on_editPatientButton_clicked);
    connect(btnDeletePatient, &QPushButton::clicked, this, &MainWindow::on_removePatientButton_clicked);
    connect(btnArchivePatient, &QPushButton::clicked, this, &MainWindow::on_archivePatientButton_clicked);
    connect(comboSelectPatient, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_patientSelected);

    // Configure loading window that indicates to user than a process is happening
    loadingDialog = new QDialog(this);
    loadingDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    loadingDialog->setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(loadingDialog);
    QLabel *spinner = new QLabel(loadingDialog);
    QMovie *movie = new QMovie(":/spinner.gif");

    spinner->setMovie(movie);
    movie->start();

    loadingLabel = new QLabel("Loading, please wait...", loadingDialog);
    loadingLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(spinner);
    layout->addWidget(loadingLabel);

    loadingDialog->setLayout(layout);
    loadingDialog->resize(200, 150);

    // Load existing patients on startup
    loadPatientsIntoDropdown();
    viewPatient();
    if (comboSelectPatient->count() > 0)
    {
        QTimer::singleShot(100, this, [this]()
                           {
            comboSelectPatient->setCurrentIndex(0);
            on_patientSelected(0); });
    }

    // Connect "Summarize" button to summarize transcripts and update window
    connect(btnSummarize, &QPushButton::clicked, this, &MainWindow::handleSummarizeButtonClicked);
}

/**
 * @name endLoading
 * @brief Handler function for when any of the the user's API keys are invalid.
 * @param[in] Cause of error thrown by client; used to produce a descriptive error message.
 * @author Thomas Llamzon
 */
void MainWindow::endLoading(QNetworkReply *reply) {
    loadingDialog->hide();

    QString errorMessage = reply->errorString(); // Parse error message from reply generated by attempt to contact API

    // Make presentable error messsage according to which client's API gave the error
    if (errorMessage.contains("openai")) {
        errorMessage = "Transcriber Error!\n\nPlease ensure your transcriber API Key is properly configured in Settings.";
    } else if (errorMessage.contains("googleapis")) {
        errorMessage = "Summarizer Error!\n\nPlease ensure your sumarizer API Key is properly configured in Settings.";
    } else {
        errorMessage = "Please ensure your API keys are properly configured in Settings.";
    }

    // Display error message
    QMessageBox::warning(this, "BAD API KEY", errorMessage);
}

/**
 * @name handleLLMResponse
 * @brief Handler function called when the LLM returns a response
 * @details Sets the plan text transcription displayed in the main window and
 * saves the transcript to a file.
 * @param[in] response: Response returned by the LLM
 * @author Callum Thompson
 */
void MainWindow::handleLLMResponse(const QString &response)
{
    // Get selected patient ID
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid())
    {
        qWarning() << "No patient selected, cannot save transcript!";
        return;
    }
    int patientID = patientData.toInt(); // Get patient ID

    // Ensure the patient's folder exists
    QDir patientDir("Patients/" + QString::number(patientID));
    if (!patientDir.exists())
    {
        patientDir.mkpath(".");
    }

    // Save the transcript to file
    QString transcriptPath = patientDir.filePath("summary.txt");
    QFile file(transcriptPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << response;
        file.close();
    }
    else
    {
        qInfo() << "Failed to save transcript!";
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
 * @author Joelene Hales
 * @author Callum Thompson
 */
void MainWindow::handleSummaryLayoutChanged(SummaryFormatter *summaryFormatter)
{
    QAction *selectedOption = qobject_cast<QAction *>(sender());

    // Clear existing summary display
    QLayoutItem *child;
    while ((child = summarySection->takeAt(0)) != nullptr)
    {
        delete child->widget(); // Ensure previous widget is deleted
        delete child;
    }

    if (selectedOption->text() == "Plain Text Transcript")
    {
        summaryTitle->setText("Transcript"); // Change section header
        
        // Ensure the transcript is actually loaded
        if (currentTranscriptText.isEmpty())
        {
            qInfo() << "No transcript available.";
            return;
        }

        // Create a QLabel inside the summary section (INSTEAD OF ADDING ANYTHING ELSE)
        QLabel *transcriptLabel = new QLabel(currentTranscriptText);
        transcriptLabel->setWordWrap(true);                          // Ensure proper wrapping
        transcriptLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Positioning fix

        // Add it inside the summary layout
        summarySection->addWidget(transcriptLabel);
    }
    else
    {
        // Display summary with the selected format
        summaryTitle->setText("Summary");
        setSummaryFormatter(summaryFormatter);
        displaySummary(summaryGenerator->getSummary());
    }

    // Update menu UI
    for (QAction *layoutAction : summaryLayoutOptions->actions())
    {
        layoutAction->setEnabled(layoutAction != selectedOption);
        selectSummaryLayout->setText(selectedOption->text());
    }
}

/**
 * @name handleSummarizeButtonClicked
 * @brief Handler function called when summarize button is clicked
 * @details Starts a new summary generation process via the LLM client.
 * @author Callum Thompson
 */
void MainWindow::handleSummarizeButtonClicked()
{
    // Get selected patient ID
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid())
    {
        QMessageBox::warning(this, "No Patient Selected", "Please select a patient before summarizing.");
        return;
    }
    int patientID = patientData.toInt();

    // Construct file path for the transcript
    QString transcriptPath = "Patients/" + QString::number(patientID) + "/transcript_raw.txt";

    // Open and read the transcript
    QFile file(transcriptPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open transcript. Request aborted.";
        return;
    }

    currentTranscriptText = QTextStream(&file).readAll().trimmed();
    file.close();

    // Create a transcript object
    Transcript *transcript = new Transcript(QTime::currentTime(), currentTranscriptText);

    loadingDialog->show();

    // Send transcript to the LLM
    summaryGenerator->sendRequest(*transcript);

}

/**
 * @name handleSummaryReady
 * @brief Processes and displays the structured summary after LLM response
 * @details Hides the loading dialog, retrieves the summary from the
 * SummaryGenerator, and saves it to a file.
 * @author Callum Thompson
 * @author Kalundi Serumaga
 */
void MainWindow::handleSummaryReady()
{
    loadingDialog->hide(); // Hide loading dialog once summary is ready

    // Retrieve structured summary from SummaryGenerator
    Summary summary = summaryGenerator->getSummary();

    // Get selected patient ID
    QVariant patientData = comboSelectPatient->currentData();
    if (!patientData.isValid())
    {
        qWarning() << "No patient selected, cannot save summary!";
        return;
    }
    int patientID = patientData.toInt();

    // Construct summary file path
    QString summaryPath = "Patients/" + QString::number(patientID) + "/summary.txt";

    // Save summary to file
    QFile file(summaryPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << summary.getText();
        file.close();
    }
    else
    {
        qInfo() << "Failed to save summary!";
    }

    // Update the UI with the summary
    displaySummary(summary);
    btnSummarize->setText("Regenerate Summary");

    //  Ensure transcript_raw.txt is restored with actual transcript (not summary)

}

/**
 * @name loadPatientsIntoDropdown
 * @brief Handles adding a new patient record
 * @details Loads all patients from the Patients directory into the dropdown
 * @return Returns false if the Patients directory is empty
 * @author Kalundi Serumaga
 */
bool MainWindow::loadPatientsIntoDropdown()
{
    comboSelectPatient->clear(); // Clear dropdown before loading
    QDir patientsDir("Patients");

    if (!patientsDir.exists()) // Ensure patient directory exists before attempting to get information from it
    {
        return false;
    }

    bool empty = true; // Used to indicate to toher functions if there are patients in the Patients directory
    QStringList patientFolders = patientsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folderName : patientFolders)
    { // Load information into dropdown
        empty = false;
        int patientID = folderName.toInt(); // Convert folder name to int
        PatientRecord patient = FileHandler::getInstance()->loadPatientRecord(patientID);
        QString displayName = patient.getFirstName() + " " + patient.getLastName() + " [" + QString::number(patientID) + "]";
        comboSelectPatient->addItem(displayName, patientID);
    }
    return !empty;
}

/**
 * @name loadArchivedPatientsIntoDropdown
 * @brief Handles loading archived patients into the dropdown
 * @details Loads all archived patients from the Archived directory into the dropdown. 
 * If the directory does not exist, it returns true.
 * @return Returns false if the archived patients directory is empty.
 * @author Andres Pedreros Castro
 * @author Kalundi Serumaga
 * @author Thomas Llamzon
 * @returns boolean - true if the directory does not exist, false otherwise.
 */
bool MainWindow::loadArchivedPatientsIntoDropdown()
{
    comboSelectPatient->clear(); // Clear dropdown before loading
    QDir archivedDir("Archived");

    if (!archivedDir.exists()) // Ensure archived directory exists before attempting to get information from it
    {
        return true;
    }

    bool empty = true; // Used to indicate if any patients have been archived or if there are any archived patients
    QStringList patientFolders = archivedDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folderName : patientFolders)
    { // Load patient information into dropdown
        empty = false;
        int patientID = folderName.toInt();                                               // Convert folder name to int
        PatientRecord patient = FileHandler::getInstance()->loadPatientRecord(patientID); // Load archived record
        QString displayName = patient.getFirstName() + " " + patient.getLastName() + " [" + QString::number(patientID) + "]";
        comboSelectPatient->addItem(displayName, patientID);
    }

    return !empty;
}

/**
 * @name setSummaryFormatter
 * @brief Set the formatter used to create the summary
 * @details Deletes the old formatter and sets the new one
 * @param[in] summaryFormatter: Summary layout formatter
 * @author Callum Thompson
 * @author Joelene Hales
 */
void MainWindow::setSummaryFormatter(SummaryFormatter *newSummaryFormatter)
{
    delete summaryFormatter;
    summaryFormatter = newSummaryFormatter;
}

/**
 * @name displaySummary
 * @brief Display summary using the configured layout
 * @details Uses the summaryFormatter to generate the layout for the summary. 
 * @param[in] summary: Summary to display
 * @author Joelene Hales
 * @author Callum Thompson
 */
void MainWindow::displaySummary(const Summary &summary)
{
    if (!summaryFormatter)
    {
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
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 * @author Thomas Llamzon
 */
void MainWindow::on_addPatientButton_clicked()
{
    AddPatientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) // Get information entered in the add patient form
    {
        QString firstName = dialog.getFirstName();
        QString lastName = dialog.getLastName();
        QString dateOfBirth = dialog.getDateOfBirth();
        QString healthCard = dialog.getHealthCard();
        QString email = dialog.getEmail();
        QString phoneNumber = dialog.getPhoneNumber();
        QString address = dialog.getAddress();
        QString postalCode = dialog.getPostalCode();
        QString province = dialog.getProvince();
        QString country = dialog.getCountry();
        QString baseName = firstName + " " + lastName; // No DOB in dropdown

        int patientID = QDateTime::currentMSecsSinceEpoch() % 100000;

        // Check if a patient with the same name already exists
        int duplicateCount = 0;
        for (int i = 0; i < comboSelectPatient->count(); ++i)
        {
            QVariant storedID = comboSelectPatient->itemData(i);
            PatientRecord existingPatient = FileHandler::getInstance()->loadPatientRecord(storedID.toInt());

            if (existingPatient.getFirstName() == firstName &&
                existingPatient.getLastName() == lastName &&
                existingPatient.getDateOfBirth() == dateOfBirth)
            {
                QMessageBox::warning(this, "Duplicate Patient",
                                     "A patient with this name and birthdate already exists!");
                return;
            }

            // If the same name exists, increase counter
            if (comboSelectPatient->itemText(i).startsWith(baseName))
            {
                duplicateCount++;
            }
        }

        // If duplicate, add a counter [1], [2], etc.
        QString displayName = baseName;
        if (duplicateCount > 0)
        {
            displayName += " [" + QString::number(duplicateCount) + "]";
        }

        // Create and save new patient
        PatientRecord newPatient(
            patientID,
            healthCard,
            firstName,
            lastName,
            dateOfBirth,
            email,
            phoneNumber,
            address,
            postalCode,
            province,
            country);
        FileHandler::getInstance()->savePatientRecord(newPatient);

        // Update user interface to show new patient in dropdown
        comboSelectPatient->addItem(displayName, patientID);

        // Refresh UI
        checkDropdownEmpty();
        viewPatient();
    }
}

/**
 * @name on_editPatientButton_clicked
 * @brief Handler function called when the "Edit Patient" button is pressed
 * @details Displays a dialog window that prompts the user to edit the
 * patient's details. Once the user submits the form, the file of the existing selected
 * patient is updated with the changed fields.
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 * @author Thomas Llamzon
 */
void MainWindow::on_editPatientButton_clicked()
{
    int patientID = comboSelectPatient->currentData().toInt();
    if (patientID == 0)
        return;

    // Get existing patient by patientID
    PatientRecord existing = FileHandler::getInstance()->loadPatientRecord(patientID);

    // Change that existing patient's information according to edit patient form
    EditPatientInfo dialog(this);
    dialog.setFirstName(existing.getFirstName());
    dialog.setLastName(existing.getLastName());
    dialog.setDateOfBirth(existing.getDateOfBirth());
    dialog.setHealthCard(existing.getHealthCard());
    dialog.setEmail(existing.getEmail());
    dialog.setPhoneNumber(existing.getPhoneNumber());
    dialog.setAddress(existing.getAddress());
    dialog.setPostalCode(existing.getPostalCode());
    dialog.setProvince(existing.getProvince());
    dialog.setCountry(existing.getCountry());

    if (dialog.exec() == QDialog::Accepted)
    {
        existing.setFirstName(dialog.getFirstName());
        existing.setLastName(dialog.getLastName());
        existing.setDateOfBirth(dialog.getDateOfBirth());
        existing.setHealthCard(dialog.getHealthCard());
        existing.setEmail(dialog.getEmail());
        existing.setPhoneNumber(dialog.getPhoneNumber());
        existing.setAddress(dialog.getAddress());
        existing.setPostalCode(dialog.getPostalCode());
        existing.setProvince(dialog.getProvince());
        existing.setCountry(dialog.getCountry());

        FileHandler::getInstance()->savePatientRecord(existing);

        loadPatientsIntoDropdown(); // Refresh display names if changed
        viewPatient();
    }
}

/**
 * @name on_removePatientButton_clicked
 * @brief Handle deletion of the selected patient from the system
 * @details Deletes the selected patient's folder and all associated files from either
 *          the active or archived directory. Updates the dropdown and UI accordingly.
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 * @author Thomas Llamzon
 */
void MainWindow::on_removePatientButton_clicked()
{
    // Prevent erroneous behaviour; user accidentally clicks the "delete patient" button - prevent accidental deletion
    if (QMessageBox::question(this, "Delete Confirmation", "Are you sure you want to delete this patient?", QMessageBox::Yes | QMessageBox::Cancel)
        == QMessageBox::Cancel)
    return;

    int index = comboSelectPatient->currentIndex();
    if (index == -1)
        return; // No patient selected

    int selectedID = comboSelectPatient->currentData().toInt();
    QString baseDir = archiveMode ? "Archived/" : "Patients/"; // Find directory patient is stored in based off whether the user is in arhcive mode
    QString patientDirPath = baseDir + QString::number(selectedID);

    QDir dir(patientDirPath);
    if (dir.exists())
    {
        if (dir.removeRecursively()) // Delete all files related to that patient
        {
            qInfo() << "Patient folder deleted successfully:" << patientDirPath;
        }
        else
        {
            QMessageBox::warning(this, "Delete Failed", "Could not delete patient folder.");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "Delete Failed", "Patient folder does not exist.");
        return;
    }

    // Refresh UI
    comboSelectPatient->removeItem(index);
    checkDropdownEmpty();
}

/**
 * @brief MainWindow::on_archivePatientButton_clicked
 * @brief Handler function called when the "Archive Patient" button is pressed
 * @details Returns patient to active patients directory.
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 * @author Thomas Llamzon
 */
void MainWindow::on_archivePatientButton_clicked()
{

    int index = comboSelectPatient->currentIndex();
    if (index == -1)
        return;

    if (archiveMode)
    {                                                                                                  // ARCHIVE MODE --> Handle UNARCHIVING
        FileHandler::getInstance()->unarchivePatientRecord(comboSelectPatient->currentData().toInt()); // Unarchive Patient
    }
    else
    {                                                                                                // ACTIVE MODE --> Handle ARCHIVING
        FileHandler::getInstance()->archivePatientRecord(comboSelectPatient->currentData().toInt()); // Archive Patient
    }

    // Refresh UI
    comboSelectPatient->removeItem(index);
    checkDropdownEmpty();
}

/**
 * @name handleArchiveToggled
 * @brief Handler function called when the "Archive" toggle button is pressed
 * @details Toggles between archive and active mode. Updates the UI accordingly.
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 */
void MainWindow::handleArchiveToggled()
{
    archiveMode = !archiveMode; // Toggle archive mode

    if (archiveMode) // IN ARCHIVE MODE
    {
        toggleSwitch->setText("Show All Active Patients");
        btnArchivePatient->setText("Unarchive Patient");

        viewPatient(); // Update Patient Info Label

        // Disable ADD, EDIT, RECORD, SUMMARIZE buttons
        btnAddPatient->setEnabled(false);
        btnEditPatient->setEnabled(false);
        btnRecord->setEnabled(false);
        btnSummarize->setEnabled(false);
        btnAddPatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
        btnEditPatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
        btnRecord->setStyleSheet(WindowBuilder::disabledButtonStyle);
        btnSummarize->setStyleSheet(WindowBuilder::disabledButtonStyle);
        toggleSwitch->setStyleSheet(WindowBuilder::blueButtonStyle);
    }
    else // IN ACTIVE MODE
    {
        toggleSwitch->setText("Show All Archived Patients");
        btnArchivePatient->setText("Archive Patient");

        viewPatient();

        // Re-enable ADD, EDIT, RECORD, SUMMARIZE buttons
        btnAddPatient->setEnabled(true);
        btnEditPatient->setEnabled(true);
        btnRecord->setEnabled(true);
        btnSummarize->setEnabled(true);
        btnAddPatient->setStyleSheet(WindowBuilder::blueButtonStyle);
        btnEditPatient->setStyleSheet(WindowBuilder::blueButtonStyle);
        btnRecord->setStyleSheet(WindowBuilder::blueButtonStyle);
        btnSummarize->setStyleSheet(WindowBuilder::orangeButtonStyle);
        toggleSwitch->setStyleSheet(WindowBuilder::greyButtonStyle);
    }

    // Refresh UI
    checkDropdownEmpty();
}

/**
 * @name checkDropdownEmpty
 * @brief Check if the dropdown is empty
 * @details Checks if Archive or Patient directories no longer have patients to update UI accordingly.
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 */
void MainWindow::checkDropdownEmpty()
{

    bool notEmpty; // check whichever directory is empty based on whether the user is in arhcive mode
    if (archiveMode)
        notEmpty = loadArchivedPatientsIntoDropdown();
    else
        notEmpty = loadPatientsIntoDropdown();

    if (!notEmpty) // Dropdown has been emptied
    {
        lblPatientName->setText("Name: \nDOB: \nPhone: \nEmail: \nAddress: \nProvince: \nCountry: ");

        // Disable all patient actions except for ADD
        btnDeletePatient->setEnabled(false);
        btnArchivePatient->setEnabled(false);
        // Update button style to indicate disabled
        btnDeletePatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
        btnArchivePatient->setStyleSheet(WindowBuilder::disabledButtonStyle);

        // Only need to disable EDIT, RECORD, SUMMARIZE in active mode (avoiding redundancy)
        if (!archiveMode)
        { // Disable button functionality
            btnEditPatient->setEnabled(false);
            btnRecord->setEnabled(false);
            btnSummarize->setEnabled(false);
            // Update button style to indicate disabled
            btnEditPatient->setStyleSheet(WindowBuilder::disabledButtonStyle);
            btnRecord->setStyleSheet(WindowBuilder::disabledButtonStyle);
            btnSummarize->setStyleSheet(WindowBuilder::disabledButtonStyle);
        }
    }
    else // Dropdown no longer empty
    {
        btnDeletePatient->setEnabled(true);
        btnArchivePatient->setEnabled(true);
        // Update button style to indicate re-enabled
        btnDeletePatient->setStyleSheet(WindowBuilder::redButtonStyle);
        btnArchivePatient->setStyleSheet(WindowBuilder::orangeButtonStyle);

        // Only re-enable EDIT, RECORD, SUMMARIZE in active mode
        if (!archiveMode)
        { // Re-enable button functionality
            btnEditPatient->setEnabled(true);
            btnRecord->setEnabled(true);
            btnSummarize->setEnabled(true);
            // Update button style to indicate enabled
            btnEditPatient->setStyleSheet(WindowBuilder::orangeButtonStyle);
            btnRecord->setStyleSheet(WindowBuilder::blueButtonStyle);
            btnSummarize->setStyleSheet(WindowBuilder::orangeButtonStyle);
        }
    }
}

/**
 * @name on_patientSelected
 * @brief Handles the signal that a new patient has been selected
 * @details Updates the UI to display the select patient's ID
 * @param[in] index: Index of option selected in the dropdown
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 */
void MainWindow::on_patientSelected(int index)
{
    if (index == -1)
        return;

    patientID = comboSelectPatient->currentData().toInt();

    viewPatient(); // Update current patient information section

    // Load the transcript
    QString savedTranscript = FileHandler::getInstance()->loadTranscript(patientID);
    if (!savedTranscript.isEmpty())
    {
        currentTranscriptText = savedTranscript; // Store transcript
    }
    else
    {
        currentTranscriptText.clear();
    }

    // Load the structured summary according to default summary layout preference
    QString savedSummaryText = FileHandler::getInstance()->loadSummaryText(patientID);

    QString defaultLayout = settings->getSummaryPreference();

    // Clear existing summary layout
    QLayoutItem *child;
    while ((child = summarySection->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // Revert summary layout according to user summary layout preference.
    if (defaultLayout.contains("Detailed Summary"))
    {
        summaryFormatter = new DetailedSummaryFormatter;
        selectSummaryLayout->setText("Detailed Summary");

    }
    else if (defaultLayout.contains("Concise Summary"))
    {
        summaryFormatter = new ConciseSummaryFormatter;
        selectSummaryLayout->setText("Concise Summary");
    }
    else
    {
        summaryFormatter = new DetailedSummaryFormatter;
        selectSummaryLayout->setText("Detailed Summary");
    }

    // Refresh summary layout dropdown
    QString currentLayout = selectSummaryLayout->text();
    for (QAction *layoutAction : summaryLayoutOptions->actions())
    {
        layoutAction->setEnabled(layoutAction->text() != currentLayout);
    }

    if (!savedSummaryText.isEmpty())
    {
        summaryGenerator->setSummaryText(savedSummaryText);

        Summary summary = summaryGenerator->getSummary();

        displaySummary(summary);
        btnSummarize->setText("Regenerate Summary");
    }
    else
    {
        // Clear the summary UI
        QLayoutItem *child;
        while ((child = summarySection->takeAt(0)) != nullptr)
        {
            if (child->widget())
            {
                child->widget()->deleteLater(); // Safely delete the widget
            }
            delete child; // Free the layout item
        }
        btnSummarize->setText("Summarize");
    }
}

/**
 * @name ~MainWindow
 * @brief Deconstructor for MainWindow
 * @details Does nothing
 * @author Callum Thompson
 */
MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}

/**
 * @name viewPatient
 * @brief When switched to a patient, display their information in the top corner
 * @details This function is called when a patient is selected from the dropdown
 * @author Kalundi Serumaga
 * @author Andres Pedreros Castro
 */
void MainWindow::viewPatient()
{
    QVariant patientData = comboSelectPatient->currentData(); // Get patient data based off user's current selected patient
    QString info;

    if (!patientData.isValid()) // If there is no patient selected to update patient info section with
    {
        qWarning() << "No patient selected, cannot view patient!";
        return;
    }

    int patientID = patientData.toInt();
    PatientRecord patient = FileHandler::getInstance()->loadPatientRecord(patientID);
    // Update information based off current selected patient
    info =
        "Name: " + patient.getFirstName() + " " + patient.getLastName() + "\n" +
        "DOB: " + patient.getDateOfBirth() + "\n" +
        "Health Card: " + patient.getHealthCard() + "\n" +
        "Phone: " + patient.getPhoneNumber() + "\n" +
        "Email: " + patient.getEmail() + "\n" +
        "Address: " + patient.getAddress() + "\n" +
        "Province: " + patient.getProvince() + "\n" +
        "Postal Code: " + patient.getPostalCode() + "\n" +
        "Country: " + patient.getCountry();

    lblPatientName->setText(info); // update the object associated with the current patient information section
}
