/**
 * @file windowbuilder.h
 * @brief Declaration of WindowBuilder class
 *
 * @author Andres Pedreros Castro (apedrero@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Thomas Llamzon (tllamazon@uwo.ca)
 * @date Mar. 1, 2025
 */

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

class WindowBuilder
{
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
                        QPushButton *&btnEditPatient,
                        QPushButton *&btnDeletePatient,
                        QPushButton *&btnArchivePatient,
                        QPushButton *&toggleSwitch);

    static const QString blueButtonStyle;
    static const QString orangeButtonStyle;
    static const QString redButtonStyle;
    static const QString greyButtonStyle;
    static const QString settingsBlueButtonStyle;
};

#endif // WINDOWBUILDER_H
