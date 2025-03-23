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
 * @author Thomas Llamzon (tllamazon@uwo.ca)
 * @date Mar. 1, 2025
 */

#include "windowbuilder.h"
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPixmap>

/**
 * @name setupUI
 * @brief Builds the application's main user interface
 * @details Includes initializing UI elements, layout, and styling
 * @param[in,out] centralWidget: Central widget displaying `mainLayout`
 *  @param[in,out] btnConnectDevice: "Connect Device" button
 *  @param[in,out] btnSettings: "Settings" button
 *  @param[in,out] lblTitle: Window title
 *  @param[in,out] lblPatientName: Patient's name
 *  @param[in,out] comboSelectPatient: Dropdown menu to select patient
 *  @param[in,out] btnRecord: "Record" button
 *  @param[in,out] btnSummarize: "Summarize" button
 *  @param[in,out] selectSummaryLayout: Dropdown menu to select summary layout format
 *  @param[in,out] summarySection: Displays the LLM-generated summary
 *  @param[in,out] mainLayout: Main layout which contains all UI elements
 *  @param[in,out] btnAddPatient: "Add Patient" button
 *  @param[in,out] btnArchivePatient: "Remove Patient" button
 *  @param[in,out] toggleSwitch: Toggle switch to show archived summaries
 */
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

// ==================== Window Constructor ====================
void WindowBuilder::setupUI(QWidget *centralWidget,
                            QPushButton *&btnSettings,
                            QLabel *&lblTitle,
                            QLabel *&lblPatientName,
                            QComboBox *&comboSelectPatient,
                            QPushButton *&btnRecord,
                            QPushButton *&btnSummarize,
                            QPushButton *&selectSummaryLayout,
                            QVBoxLayout *&summarySection,
                            QVBoxLayout *&mainLayout,
                            QPushButton *&btnAddPatient,
                            QPushButton *&btnDeletePatient,
                            QPushButton *&btnArchivePatient,
                            QPushButton *&toggleSwitch) // Add toggleSwitch parameter
{
    // Create UI elements
    btnSettings = new QPushButton("Settings", centralWidget);
    lblTitle = new QLabel(centralWidget);
    lblPatientName = new QLabel("[Select a Patient]", centralWidget);
    comboSelectPatient = new QComboBox(centralWidget);
    btnRecord = new QPushButton("Record", centralWidget);
    btnSummarize = new QPushButton("Summarize", centralWidget);
    btnAddPatient = new QPushButton("Add", centralWidget);
    btnDeletePatient = new QPushButton("Delete", centralWidget);
    btnArchivePatient = new QPushButton("Archive", centralWidget);
    QLabel *summaryTitle = new QLabel("Summary");
    selectSummaryLayout = new QPushButton(centralWidget);
    selectSummaryLayout->setCheckable(true);
    selectSummaryLayout->setText("Select Summary Layout");
    selectSummaryLayout->setFixedWidth(300);

    // Initialize toggle switch
    toggleSwitch = new QPushButton("Show All Archived Patients", centralWidget);
    toggleSwitch->setCheckable(true);
    toggleSwitch->setStyleSheet(greyButtonStyle);

    // Set logo as title without distortion
    QPixmap logoPixmap(":/logo.png");
    lblTitle->setPixmap(logoPixmap.scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    lblPatientName->setAlignment(Qt::AlignCenter);
    lblPatientName->setStyleSheet("font-weight: bold; font-size: 24px; color: #555;");

    btnSettings->setStyleSheet(greyButtonStyle);
    btnSettings->setFixedWidth(150);
    btnSummarize->setStyleSheet(orangeButtonStyle);
    btnAddPatient->setStyleSheet(blueButtonStyle);
    btnDeletePatient->setStyleSheet(redButtonStyle);
    btnArchivePatient->setStyleSheet(orangeButtonStyle);

    btnRecord->setStyleSheet(blueButtonStyle);
    QObject::connect(btnRecord, &QPushButton::clicked, [btnRecord]() {
        static bool isRecording = false;
        isRecording = !isRecording;
        btnRecord->setStyleSheet(isRecording ? redButtonStyle : blueButtonStyle);
        btnRecord->setText(isRecording ? "Stop Recording" : "Record"); });

    // Layouts
    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QHBoxLayout *patientSelectLayout = new QHBoxLayout();
    QHBoxLayout *patientControlsLayout = new QHBoxLayout();
    QHBoxLayout *summaryHeader = new QHBoxLayout();
    QHBoxLayout *recordSummarizeLayout = new QHBoxLayout();
    summarySection = new QVBoxLayout();

    // Top bar layout
    topBarLayout->addStretch();
    topBarLayout->addWidget(lblTitle);
    topBarLayout->addStretch();
    topBarLayout->addWidget(btnSettings);
    topBarLayout->setContentsMargins(175, 0, 0, 0);

    // Patient controls layout
    patientControlsLayout->addWidget(comboSelectPatient);
    patientControlsLayout->addWidget(btnAddPatient);
    patientControlsLayout->addWidget(btnArchivePatient);
    patientControlsLayout->addWidget(btnDeletePatient);
    patientControlsLayout->addWidget(toggleSwitch);
    patientControlsLayout->setSpacing(10);

    // Summary layout header and format selection
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
    mainLayout->addWidget(lblPatientName);
    mainLayout->addLayout(patientSelectLayout);
    mainLayout->addLayout(patientControlsLayout);
    mainLayout->addLayout(summaryHeader);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(recordSummarizeLayout);
    mainLayout->setSpacing(15);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);
}
