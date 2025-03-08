#include "windowbuilder.h"

void WindowBuilder::setupUI(QWidget *centralWidget,
                            QPushButton *&btnConnectDevice,
                            QPushButton *&btnSettings,
                            QLabel *&lblTitle,
                            QLabel *&lblPatientName,
                            QComboBox *&comboSelectPatient,
                            QPushButton *&btnRecord,
                            QTextEdit *&textTranscription,
                            QPushButton *&selectSummaryLayout,
                            QVBoxLayout *&summarySection,
                            QVBoxLayout *&mainLayout,
                            QPushButton *&btnAddPatient)
{
    // Create UI elements
    btnConnectDevice = new QPushButton("Connect Device", centralWidget);
    btnSettings = new QPushButton("Settings", centralWidget);
    lblTitle = new QLabel("RheumAI Dashboard", centralWidget);
    lblPatientName = new QLabel("[Patient Name]", centralWidget);
    comboSelectPatient = new QComboBox(centralWidget);
    btnRecord = new QPushButton("Record", centralWidget);
    textTranscription = new QTextEdit(centralWidget);
    btnAddPatient = new QPushButton("Add Patient", centralWidget);
    textTranscription->setReadOnly(true);
    QLabel* summaryTitle = new QLabel("Summary");
    selectSummaryLayout = new QPushButton(centralWidget);
    selectSummaryLayout->setCheckable(true);  // Makes it toggle between states
    selectSummaryLayout->setText("Select Summary Layout");

    // Styling
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-weight: bold; font-size: 18px;");

    lblPatientName->setAlignment(Qt::AlignCenter);
    lblPatientName->setStyleSheet("font-weight: bold; font-size: 14px;");

    summaryTitle->setAlignment(Qt::AlignLeft);
    summaryTitle->setStyleSheet("font-weight: bold; font-size: 14px;");

    // Layouts
    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    QHBoxLayout *summaryHeader = new QHBoxLayout();
    summarySection = new QVBoxLayout();

    // Top bar layout
    topBarLayout->addWidget(btnConnectDevice);
    topBarLayout->addStretch();
    topBarLayout->addWidget(lblTitle);
    topBarLayout->addStretch();
    topBarLayout->addWidget(btnSettings);

    // Controls layout
    controlsLayout->addWidget(comboSelectPatient);
    controlsLayout->addWidget(btnRecord);
    controlsLayout->addWidget(btnAddPatient);

    // Summary layout header and format selection
    summaryHeader->addWidget(summaryTitle);
    summaryHeader->addWidget(selectSummaryLayout);

    // Add layouts to main layout
    mainLayout->addLayout(topBarLayout);
    mainLayout->addWidget(lblPatientName);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(textTranscription);
    mainLayout->addLayout(summaryHeader);
    mainLayout->addLayout(summarySection);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);
}
