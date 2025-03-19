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
#include <QCheckBox>

class WindowBuilder {
public:
    static void setupUI(QWidget *centralWidget,
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
                        QPushButton *&btnRemovePatient);
};

#endif // WINDOWBUILDER_H
