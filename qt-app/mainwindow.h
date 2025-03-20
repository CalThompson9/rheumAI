#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "windowbuilder.h"
#include "llmclient.h"
#include "summary.h"
#include "summaryformatter.h"
#include "summarygenerator.h"
#include "settings.h"

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
    QPushButton *btnSettings;
    QLabel *lblTitle;
    QLabel *lblPatientName;
    QComboBox *comboSelectPatient;
    QPushButton *btnRecord;
    QPushButton *btnSummarize;
    QPushButton *btnAddPatient;
    QPushButton *btnRemovePatient;
    QTextEdit *textTranscription;
    QPushButton *selectSummaryLayout;
    QMenu* summaryLayoutOptions;
    QVBoxLayout *summarySection;
    LLMClient *llmClient;

    // Layout
    QVBoxLayout *mainLayout;

    // Summarization
    SummaryFormatter *summaryFormatter;
    SummaryGenerator *summaryGenerator;
    Summary testSummary;

    QString currentTranscriptText;

    int patientID;
    
    Settings *settings;

private slots:
    void handleSummaryLayoutChanged(SummaryFormatter* summaryFormatter);
    void handleSummarizeButtonClicked();
    void handleSummaryReady();
    void on_addPatientButton_clicked();

public slots:
    void showSettings();
    void on_patientSelected(int index);
    void on_removePatientButton_clicked();
    void loadPatientsIntoDropdown();
    void handleLLMResponse(const QString &response);


};

#endif // MAINWINDOW_H
