#ifndef WINDOWBUILDER_H
#define WINDOWBUILDER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

class WindowBuilder {
public:
    static void setupUI(QWidget *centralWidget,
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
                        QPushButton *&btnAddPatient);

};

#endif // WINDOWBUILDER_H
