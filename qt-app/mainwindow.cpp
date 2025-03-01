#include "mainwindow.h"
#include "llmclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set screen bounds
    setGeometry(0, 0, 1200, 800);
    
    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Use WindowBuilder to set up UI
    WindowBuilder::setupUI(centralWidget, btnConnectDevice, btnSettings,
                           lblTitle, lblPatientName, comboSelectPatient,
                           btnRecord, textTranscription, mainLayout);

    // Initialize LLM client
    llmClient = new LLMClient(this);
    connect(llmClient, &LLMClient::responseReceived, this, &MainWindow::handleLLMResponse);

    // Connect "Record" button to LLM API request
    connect(btnRecord, &QPushButton::clicked, this, [this]() {
        llmClient->sendRequest("Hello, AI! How are you?");
    });
}

void MainWindow::handleLLMResponse(const QString &response)
{
    textTranscription->setPlainText(response);
}

MainWindow::~MainWindow()
{
    // Qt automatically manages memory, no need for manual deletion
}
