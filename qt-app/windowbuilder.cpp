/**
 * @file windowbuilder.cpp
 * @brief Definition of WindowBuilder class
 *
 * Provides methods to build the application's main user interface (UI), including
 * initializing UI elements, layout, and styling.
 *
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Thomas Llamzon (tllamzon@uwo.ca)
 * @date Mar. 1, 2025
 */

#include "windowbuilder.h"

// ==================== Define button styles ====================
const QString WindowBuilder::blueButtonStyle =
    "QPushButton {"
    "background-color: #5371ff;"
    "border-radius: 8px;"
    "color: white;"
    "padding: 6px;"
    "font-size: 12px;"
    "} "
    "QPushButton:hover {"
    "background-color: #425BD0;"
    "}";

const QString WindowBuilder::orangeButtonStyle =
    "QPushButton {"
    "background-color: #FF914D;"
    "border-radius: 8px;"
    "color: white;"
    "padding: 6px;"
    "font-size: 12px;"
    "} "
    "QPushButton:hover {"
    "background-color: #C56E39;"
    "}";

const QString WindowBuilder::redButtonStyle =
    "QPushButton {"
    "background-color: #b30000;"
    "border-radius: 8px;"
    "color: white;"
    "padding: 6px;"
    "font-size: 12px;"
    "} "
    "QPushButton:hover {"
    "background-color: darkred;"
    "}";

const QString WindowBuilder::settingsBlueButtonStyle =
    "QPushButton {"
    "background-color: #5371ff;"
    "color: white;"
    "border-radius: 8px;"
    "padding: 6px;"
    "font-size: 12px;"
    "} "
    "QPushButton:hover {;"
    "background-color: #425BD0;"
    "} "
    "QPushButton:pressed {"
    "background-color: #006ae6;"
    "} ";

const QString WindowBuilder::greyButtonStyle =
    "QPushButton {"
    "background-color: #AAAAAA;"
    "color: black;"
    "border-radius: 8px;"
    "padding: 6px;"
    "font-size: 12px;"
    "} "
    "QPushButton:hover {"
    "background-color: #949494;"
    "} "
    "QPushButton:pressed {"
    "background-color: #5F5F5F;"
    "} ";

const QString WindowBuilder::disabledButtonStyle =
    "QPushButton {"
    "background-color: #454545;"
    "color: #5e5e5e;"
    "border-radius: 8px;"
    "padding: 6px;"
    "font-size: 12px;"
    "} ";

/**
 * @name setupUI
 * @brief Builds the application's main user interface
 * @details Includes initializing UI elements, layout, and styling
 * @param[in,out] centralWidget: Central widget displaying `mainLayout`
 *  @param[in,out] btnSettings: "Settings" button
 *  @param[in,out] lblTitle: Window title
 *  @param[in,out] lblPatientName: Patient's name
 *  @param[in,out] comboSelectPatient: Dropdown menu to select patient
 *  @param[in,out] btnRecord: "Record" button
 *  @param[in,out] btnSummarize: "Summarize" button
 *  @param[in,out] selectSummaryLayout: Dropdown menu to select summary layout format
 *  @param[in,out] summarySection: Displays the LLM-generated summary
 *  @param[in,out] summaryTitle: Summary section title
 *  @param[in,out] mainLayout: Main layout which contains all UI elements
 *  @param[in,out] btnAddPatient: "Add Patient" button
 *  @param[in,out] btnEditPatient: "Edit Patient" button
 *  @param[in,out] btnDeletePatient: "Delete Patient" button
 *  @param[in,out] btnArchivePatient: "Archive Patient" button
 *  @param[in,out] toggleSwitch: Toggle switch to show archived summaries
 * @author Callum Thompson
 * @author Andres Pedreros Castro
 * @author Joelene Hales
 * @author Kalundi Serumaga
 * @author Thomas Llamzon
 */
void WindowBuilder::setupUI(QWidget *centralWidget,
                            QPushButton *&btnSettings,
                            QLabel *&lblTitle,
                            QLabel *&lblPatientName,
                            QComboBox *&comboSelectPatient,
                            QPushButton *&btnRecord,
                            QPushButton *&btnSummarize,
                            QPushButton *&selectSummaryLayout,
                            QVBoxLayout *&summarySection,
                            QLabel *&summaryTitle,
                            QVBoxLayout *&mainLayout,
                            QPushButton *&btnAddPatient,
                            QPushButton *&btnEditPatient,
                            QPushButton *&btnDeletePatient,
                            QPushButton *&btnArchivePatient,
                            QPushButton *&toggleSwitch)
{
    // Create UI elements
    btnSettings = new QPushButton("Settings", centralWidget);
    lblTitle = new QLabel(centralWidget);
    lblPatientName = new QLabel("[Select a Patient]", centralWidget);
    comboSelectPatient = new QComboBox(centralWidget);
    btnRecord = new QPushButton("Record", centralWidget);
    btnSummarize = new QPushButton("Summarize", centralWidget);
    btnAddPatient = new QPushButton("Add Patient", centralWidget);
    btnEditPatient = new QPushButton("Edit Patient", centralWidget);
    btnDeletePatient = new QPushButton("Delete Patient", centralWidget);
    btnArchivePatient = new QPushButton("Archive Patient", centralWidget);
    summaryTitle = new QLabel("Summary");
    selectSummaryLayout = new QPushButton(centralWidget);
    selectSummaryLayout->setCheckable(true);
    selectSummaryLayout->setText("Select Summary Layout");
    selectSummaryLayout->setFixedWidth(300);

    // Initialize Archive toggle
    toggleSwitch = new QPushButton("Show All Archived Patients", centralWidget);
    toggleSwitch->setCheckable(true);
    toggleSwitch->setStyleSheet(greyButtonStyle);

    // Set logo as title without distortion
    QPixmap logoPixmap(":/logo.png");
    lblTitle->setPixmap(logoPixmap.scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Configure patient information structure on the window
    lblPatientName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lblPatientName->setContentsMargins(15, 0, 0, 0); // Optional: padding from the left
    lblPatientName->setStyleSheet("font-weight: bold; font-size: 16px; color: #555;");
    lblPatientName->setFixedWidth(250); // or use setMaximumWidth if you want flexibility
    lblPatientName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    lblPatientName->setTextInteractionFlags(Qt::TextSelectableByMouse); // Optional, allows copying
    lblPatientName->setToolTip(lblPatientName->text());                 // Show full info on hover

    // Colouring the buttons
    btnSettings->setStyleSheet(greyButtonStyle);
    btnSettings->setFixedWidth(150);
    btnSummarize->setStyleSheet(orangeButtonStyle);
    btnAddPatient->setStyleSheet(blueButtonStyle);
    btnDeletePatient->setStyleSheet(redButtonStyle);
    btnArchivePatient->setStyleSheet(orangeButtonStyle);
    btnEditPatient->setStyleSheet(orangeButtonStyle);
    btnRecord->setStyleSheet(blueButtonStyle);

    // Connect record button clicking event to button style change
    QObject::connect(btnRecord, &QPushButton::clicked, [btnRecord]() {
        static bool isRecording = false;
        isRecording = !isRecording;
        btnRecord->setStyleSheet(isRecording ? redButtonStyle : blueButtonStyle);
        btnRecord->setText(isRecording ? "Stop Recording" : "Record");
    });

    // Create app structure with layouts
    // Main Layout is the layout which all other sections of the app adhere to
    mainLayout = new QVBoxLayout(centralWidget);

    // Subsections of the main layout
    QGridLayout *topBarLayout = new QGridLayout();
    QHBoxLayout *patientControlsLayout = new QHBoxLayout();
    QHBoxLayout *summaryHeader = new QHBoxLayout();
    QHBoxLayout *recordSummarizeLayout = new QHBoxLayout();
    summarySection = new QVBoxLayout();

    // Top bar layout with patient info, logo, and settings
    topBarLayout->addWidget(lblPatientName, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    topBarLayout->addWidget(lblTitle, 0, 1, Qt::AlignHCenter); // Center column
    topBarLayout->addWidget(btnSettings, 0, 2, Qt::AlignRight | Qt::AlignVCenter);
    topBarLayout->setColumnStretch(0, 1);
    topBarLayout->setColumnStretch(1, 0);
    topBarLayout->setColumnStretch(2, 1);
    topBarLayout->setContentsMargins(0, 0, 0, 0);

    // Patient Controls Layout contains all buttons associated with patient actions
    patientControlsLayout->addWidget(comboSelectPatient);
    patientControlsLayout->addWidget(btnAddPatient);
    patientControlsLayout->addWidget(btnEditPatient);
    patientControlsLayout->addWidget(btnArchivePatient);
    patientControlsLayout->addWidget(btnDeletePatient);
    patientControlsLayout->addWidget(toggleSwitch);
    patientControlsLayout->setSpacing(10);

    // Summary layout header and format section
    summaryHeader->addWidget(summaryTitle);
    summaryTitle->setStyleSheet("font-weight: bold; font-size: 20px; color: #555;");
    summaryHeader->addWidget(selectSummaryLayout);

    // Create scrollable summary section
    QScrollArea *scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    summarySection = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(summarySection);
    scrollArea->setWidget(scrollWidget);

    // Record & Summarize layout
    recordSummarizeLayout->addWidget(btnRecord);
    recordSummarizeLayout->addWidget(btnSummarize);
    recordSummarizeLayout->setSpacing(10);

    // Add layouts to main layout
    mainLayout->addLayout(topBarLayout);
    mainLayout->addLayout(patientControlsLayout);
    mainLayout->addLayout(summaryHeader);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(recordSummarizeLayout);
    mainLayout->setSpacing(15);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);
}
