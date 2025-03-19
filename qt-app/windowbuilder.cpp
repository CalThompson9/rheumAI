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
 *  @param[in,out] btnRemovePatient: "Remove Patient" button
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
                            QVBoxLayout *&mainLayout,
                            QPushButton *&btnAddPatient,
                            QPushButton *&btnRemovePatient)
{
    // Create UI elements
    //btnConnectDevice = new QPushButton("Connect Device", centralWidget);
    btnSettings = new QPushButton("Settings", centralWidget);
    lblTitle = new QLabel("RheumAI Dashboard", centralWidget);
    lblPatientName = new QLabel("[Patient Name]", centralWidget);
    comboSelectPatient = new QComboBox(centralWidget);
    btnRecord = new QPushButton("Record", centralWidget);
    btnSummarize = new QPushButton("Summarize", centralWidget);
    btnAddPatient = new QPushButton("Add Patient", centralWidget);
    btnRemovePatient = new QPushButton("Remove Patient", centralWidget);
    QLabel* summaryTitle = new QLabel("Summary");
    selectSummaryLayout = new QPushButton(centralWidget);
    selectSummaryLayout->setCheckable(true);
    selectSummaryLayout->setText("Select Summary Layout");

    // Style
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-weight: bold; font-size: 20px; color: #333;");

    lblPatientName->setAlignment(Qt::AlignCenter);
    lblPatientName->setStyleSheet("font-weight: bold; font-size: 16px; color: #555;");

    QString blueButtonStyle = "QPushButton {"
                               "background-color: #5371ff;"
                               "border-radius: 8px;"
                               "color: white;"
                               "padding: 10px;"
                               "font-size: 14px;"
                               "} "
                               "QPushButton:hover {"
                               "background-color: #425BD0;"
                               "}";
    QString orangeButtonStyle = "QPushButton {"
                                "background-color: #FF914D;"
                                "border-radius: 8px;"
                                "color: white;"
                                "padding: 10px;"
                                "font-size: 14px;"
                                "} "
                                "QPushButton:hover {"
                                "background-color: #C56E39;"
                                "}";

    //btnConnectDevice->setStyleSheet(blueButtonStyle);
    btnSettings->setStyleSheet(blueButtonStyle);
    btnRecord->setStyleSheet(orangeButtonStyle);
    btnSummarize->setStyleSheet(orangeButtonStyle);
    btnAddPatient->setStyleSheet(blueButtonStyle);

    summaryTitle->setAlignment(Qt::AlignLeft);
    summaryTitle->setStyleSheet("font-weight: bold; font-size: 14px;");

    // Layouts
    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    QHBoxLayout *summaryHeader = new QHBoxLayout();
    summarySection = new QVBoxLayout();

    // Top bar layout
    //topBarLayout->addWidget(btnConnectDevice);
    topBarLayout->addStretch();
    topBarLayout->addWidget(lblTitle);
    topBarLayout->addStretch();
    topBarLayout->addWidget(btnSettings);

    // Controls layout
    controlsLayout->addWidget(comboSelectPatient);
    controlsLayout->addWidget(btnRecord);
    controlsLayout->addWidget(btnSummarize);
    controlsLayout->addWidget(btnAddPatient);
    controlsLayout->addWidget(btnRemovePatient);

    // Summary layout header and format selection
    summaryHeader->addWidget(summaryTitle);
    summaryHeader->addWidget(selectSummaryLayout);

    // Create Horizontal dividers
    QFrame *line1 = new QFrame(centralWidget);
    QFrame *line2 = new QFrame(centralWidget);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    // Create a scroll area for the summary section
    QScrollArea *scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    summarySection = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(summarySection);
    scrollArea->setWidget(scrollWidget);

    // Add layouts to main layout
    mainLayout->addLayout(topBarLayout);
    mainLayout->addWidget(line1);
    mainLayout->addWidget(lblPatientName);
    mainLayout->addWidget(line2);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(summaryHeader);
    mainLayout->addWidget(scrollArea);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);
}
