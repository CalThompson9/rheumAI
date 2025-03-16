#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "windowbuilder.h"
#include "llmclient.h"
#include "summary.h"
#include "summaryformatter.h"
#include "summarygenerator.h"

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

    // Layout
    QVBoxLayout *mainLayout;

    SummaryFormatter *summaryFormatter;
    SummaryGenerator *summaryGenerator;
    Summary testSummary;

private slots:
    void handleSummaryLayoutChanged(SummaryFormatter* summaryFormatter);
    void handleSummarizeButtonClicked();
    void handleSummaryReady();
    void on_addPatientButton_clicked();

public slots:
    void showSettings();
};

#endif // MAINWINDOW_H
