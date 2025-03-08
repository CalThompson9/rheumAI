#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windowbuilder.h"
#include "llmclient.h"
#include "summary.h"
#include "summaryformatter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSummaryFormatter(SummaryFormatter* summaryFormatter);
    void displaySummary(const Summary& summary);

private:
    QWidget *centralWidget;

    // UI Components
    QPushButton *btnConnectDevice;
    QPushButton *btnSettings;
    QLabel *lblTitle;
    QLabel *lblPatientName;
    QComboBox *comboSelectPatient;
    QPushButton *btnRecord;
    QPushButton *btnAddPatient;
    QTextEdit *textTranscription;
    QPushButton *selectSummaryLayout;    
    QVBoxLayout *summarySection;

    // Backend LLM
    LLMClient *llmClient;

    // Layout
    QVBoxLayout *mainLayout;

    SummaryFormatter *summaryFormatter;  // Summary layout formatter strategy

    Summary testSummary; // FIXME: For testing only. Remove once actual summary in patient file is implemented

private slots:
    void handleLLMResponse(const QString &response);
    void on_addPatientButton_clicked();

};

#endif // MAINWINDOW_H
