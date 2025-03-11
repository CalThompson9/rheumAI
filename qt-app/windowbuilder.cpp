#include "windowbuilder.h"
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

void WindowBuilder::setupUI(QWidget *centralWidget,
                            QPushButton *&btnConnectDevice,
                            QPushButton *&btnSettings,
                            QLabel *&lblTitle,
                            QLabel *&lblPatientName,
                            QComboBox *&comboSelectPatient,
                            QPushButton *&btnRecord,
                            QPushButton *&btnSummarize,
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
    btnSummarize = new QPushButton("Summarize", centralWidget);
    textTranscription = new QTextEdit(centralWidget);
    btnAddPatient = new QPushButton("Add Patient", centralWidget);
    textTranscription->setReadOnly(true);
    QLabel* summaryTitle = new QLabel("Summary");
    selectSummaryLayout = new QPushButton(centralWidget);
    selectSummaryLayout->setCheckable(true);  // Makes it toggle between states
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

    btnConnectDevice->setStyleSheet(blueButtonStyle);
    btnSettings->setStyleSheet(blueButtonStyle);
    btnRecord->setStyleSheet(orangeButtonStyle);
    btnSummarize->setStyleSheet(orangeButtonStyle);
    btnAddPatient->setStyleSheet(blueButtonStyle);

    textTranscription->setStyleSheet("background-color: #f5f5f5; padding: 10px; border-radius: 5px;");

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
    controlsLayout->addWidget(btnSummarize);
    controlsLayout->addWidget(btnAddPatient);

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

    // Add layouts to main layout
    mainLayout->addLayout(topBarLayout);
    mainLayout->addWidget(line1);
    mainLayout->addWidget(lblPatientName);
    mainLayout->addWidget(line2);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(textTranscription);
    mainLayout->addLayout(summaryHeader);
    mainLayout->addLayout(summarySection);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);

    // 🎛 Connect settings button to pop-up
    QObject::connect(btnSettings, &QPushButton::clicked, [centralWidget]() {
        QDialog *settingsDialog = new QDialog(centralWidget);
        settingsDialog->setWindowTitle("Settings");

        QVBoxLayout *layout = new QVBoxLayout(settingsDialog);
        QLabel *label = new QLabel("Settings options will go here.", settingsDialog);
        layout->addWidget(label);

        settingsDialog->setLayout(layout);
        settingsDialog->exec();
    });
}
