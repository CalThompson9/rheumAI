#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
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
    QPushButton *btnSummarize;
    QPushButton *btnAddPatient;
    QTextEdit *textTranscription;
    QPushButton *selectSummaryLayout;
    QMenu* summaryLayoutOptions;
    QVBoxLayout *summarySection;

    // Backend LLM
    LLMClient *llmClient;

    // Layout
    QVBoxLayout *mainLayout;

    SummaryFormatter *summaryFormatter;  // Summary layout formatter strategy

    Summary testSummary; // FIXME: For testing only. Summary should eventually be located in PatientFile instead

private slots:
    void handleLLMResponse(const QString &response);
    void handleSummaryLayoutChanged(SummaryFormatter* summaryFormatter);
    void handleSummarizeButtonClicked();
    void on_addPatientButton_clicked();

};

#endif // MAINWINDOW_H
