/**
 * @file mainwindow.h
 * @brief Declaration of MainWindow class
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Thomas Llamzon (tllamzon@uwo.ca)
 * @date Mar. 1, 2025
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QMovie>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QTimer>
#include "editpatientinfo.h"
#include "audiohandler.h"
#include "detailedsummaryformatter.h"
#include "concisesummaryformatter.h"
#include "filehandler.h"
#include "patientrecord.h"
#include "transcript.h"
#include "addpatientdialog.h"
#include "windowbuilder.h"
#include "llmclient.h"
#include "summary.h"
#include "summaryformatter.h"
#include "summarygenerator.h"
#include "settings.h"

/**
 * @class MainWindow
 * @brief Main application window for the program
 * @details This class manages the main window of the application, including
 * the UI elements, patient management, and summary generation.
 * It handles user interactions, such as adding, editing, and deleting patients,
 * as well as generating and displaying summaries based on the patient's transcript.
 * The class also manages the layout of the summary display and provides
 * functionality to switch between different summary formats.
 * It connects to the LLMClient for generating summaries and handles the
 * responses from the LLM.
 * @author Andres Pedreros Castro
 * @author Callum Thompson
 * @author Joelene Hales
 * @author Kalundi Serumaga
 * @author Thomas Llamzon
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSummaryFormatter(SummaryFormatter *summaryFormatter);
    void displaySummary(const Summary &summary);
    void viewPatient();

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
    QPushButton *btnEditPatient;
    QPushButton *btnDeletePatient;
    QPushButton *btnArchivePatient;
    QPushButton *toggleSwitch;
    QPushButton *selectSummaryLayout;
    QMenu *summaryLayoutOptions;
    QVBoxLayout *summarySection;
    QDialog *loadingDialog;
    QLabel *loadingLabel;
    LLMClient *llmClient;

    // Layout
    QVBoxLayout *mainLayout;

    // Summarization
    SummaryFormatter *summaryFormatter;
    SummaryGenerator *summaryGenerator;

    QString currentTranscriptText;

    int patientID;
    bool archiveMode;

    Settings *settings;

private slots:
    void handleSummaryLayoutChanged(SummaryFormatter *summaryFormatter);
    void handleSummarizeButtonClicked();
    void handleSummaryReady();
    void on_addPatientButton_clicked();
    void on_editPatientButton_clicked();
    void handleArchiveToggled();
    void checkDropdownEmpty();

public slots:
    void on_patientSelected(int index);
    void on_removePatientButton_clicked();
    void on_archivePatientButton_clicked();
    bool loadPatientsIntoDropdown();
    bool loadArchivedPatientsIntoDropdown();
    void handleLLMResponse(const QString &response);
};

#endif // MAINWINDOW_H
