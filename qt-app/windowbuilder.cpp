#include "windowbuilder.h"

void WindowBuilder::setupUI(QWidget *centralWidget,
                            QPushButton *&btnConnectDevice,
                            QPushButton *&btnSettings,
                            QLabel *&lblTitle,
                            QLabel *&lblPatientName,
                            QComboBox *&comboSelectPatient,
                            QPushButton *&btnRecord,
                            QTextEdit *&textTranscription,
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

    // Styling
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-weight: bold; font-size: 18px;");

    lblPatientName->setAlignment(Qt::AlignCenter);
    lblPatientName->setStyleSheet("font-weight: bold; font-size: 14px;");

    // Layouts
    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QHBoxLayout *controlsLayout = new QHBoxLayout();

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

    // Add layouts to main layout
    mainLayout->addLayout(topBarLayout);
    mainLayout->addWidget(lblPatientName);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(textTranscription);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);
}
