#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windowbuilder.h"
#include "llmclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;

    // UI Components
    QPushButton *btnConnectDevice;
    QPushButton *btnSettings;
    QLabel *lblTitle;
    QLabel *lblPatientName;
    QComboBox *comboSelectPatient;
    QPushButton *btnRecord;
    QTextEdit *textTranscription;

    // Backend LLM
    LLMClient *llmClient;

    // Layout
    QVBoxLayout *mainLayout;

private slots:
    void handleLLMResponse(const QString &response);
};

#endif // MAINWINDOW_H
