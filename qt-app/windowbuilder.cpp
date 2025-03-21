#include "windowbuilder.h"
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPixmap>

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
    btnSettings = new QPushButton("Settings", centralWidget);
    lblTitle = new QLabel(centralWidget);
    lblPatientName = new QLabel("[Select a Patient]", centralWidget);
    comboSelectPatient = new QComboBox(centralWidget);
    btnRecord = new QPushButton("Record", centralWidget);
    btnSummarize = new QPushButton("Summarize", centralWidget);
    btnAddPatient = new QPushButton("Add", centralWidget);
    btnRemovePatient = new QPushButton("Remove", centralWidget);
    QLabel* summaryTitle = new QLabel("Summary");
    selectSummaryLayout = new QPushButton(centralWidget);
    selectSummaryLayout->setCheckable(true);
    selectSummaryLayout->setText("Select Summary Layout");
    selectSummaryLayout->setFixedWidth(300);

    // Set logo as title without distortion
    QPixmap logoPixmap(":/logo.png");
    lblTitle->setPixmap(logoPixmap.scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    lblPatientName->setAlignment(Qt::AlignCenter);
    lblPatientName->setStyleSheet("font-weight: bold; font-size: 16px; color: #555;");

    // Define button styles
    QString blueButtonStyle = "QPushButton {"
                               "background-color: #5371ff;"
                               "border-radius: 6px;"
                               "color: white;"
                               "padding: 6px;"
                               "font-size: 12px;"
                               "} "
                               "QPushButton:hover {"
                               "background-color: #425BD0;"
                               "}";

    QString orangeButtonStyle = "QPushButton {"
                                "background-color: #FF914D;"
                                "border-radius: 6px;"
                                "color: white;"
                                "padding: 6px;"
                                "font-size: 12px;"
                                "} "
                                "QPushButton:hover {"
                                "background-color: #C56E39;"
                                "}";

    btnSettings->setStyleSheet(blueButtonStyle);
    btnSettings->setFixedWidth(100);
    btnSummarize->setStyleSheet(orangeButtonStyle);
    btnAddPatient->setStyleSheet(blueButtonStyle);
    btnRemovePatient->setStyleSheet(orangeButtonStyle);

    // Record button styling (toggle between blue and red)
    QString recordBlueStyle = "QPushButton {"
                              "background-color: #5371ff;"
                              "border-radius: 6px;"
                              "color: white;"
                              "padding: 6px;"
                              "font-size: 12px;"
                              "} "
                              "QPushButton:hover {"
                              "background-color: #425BD0;"
                              "}";

    QString recordRedStyle = "QPushButton {"
                             "background-color: red;"
                             "border-radius: 6px;"
                             "color: white;"
                             "padding: 6px;"
                             "font-size: 12px;"
                             "} "
                             "QPushButton:hover {"
                             "background-color: darkred;"
                             "}";

    btnRecord->setStyleSheet(recordBlueStyle);
    QObject::connect(btnRecord, &QPushButton::clicked, [btnRecord, recordBlueStyle, recordRedStyle]() {
        static bool isRecording = false;
        isRecording = !isRecording;
        btnRecord->setStyleSheet(isRecording ? recordRedStyle : recordBlueStyle);
        btnRecord->setText(isRecording ? "Stop Recording" : "Record");
    });

    // Layouts
    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QHBoxLayout *patientControlsLayout = new QHBoxLayout();
    QHBoxLayout *summaryHeader = new QHBoxLayout();
    QHBoxLayout *recordSummarizeLayout = new QHBoxLayout();
    summarySection = new QVBoxLayout();

    // Top bar layout
    topBarLayout->addStretch();
    topBarLayout->addWidget(lblTitle);
    topBarLayout->addStretch();
    topBarLayout->addWidget(btnSettings);
    // Padding on left
    topBarLayout->setContentsMargins(125, 0, 0, 0);

    // Patient controls layout
    patientControlsLayout->addWidget(comboSelectPatient);
    patientControlsLayout->addWidget(btnAddPatient);
    patientControlsLayout->addWidget(btnRemovePatient);
    patientControlsLayout->setSpacing(10);

    // Summary layout header and format selection
    summaryHeader->addWidget(summaryTitle);
    summaryHeader->addWidget(selectSummaryLayout);
    summaryHeader->setSpacing(10);

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
    mainLayout->addLayout(patientControlsLayout);
    mainLayout->addLayout(summaryHeader);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(recordSummarizeLayout);
    mainLayout->setSpacing(15);

    // Set layout to central widget
    centralWidget->setLayout(mainLayout);
}
