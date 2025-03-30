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
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QPixmap>

/**
 * @class WindowBuilder
 * @brief WindowBuilder class
 * @details This class is responsible for building the main user interface of the application.
 * It includes methods to set up the UI elements, layout, and styling.
 * The UI includes buttons, labels, combo boxes, and layouts for organizing the interface.
 * The class also provides styles for different buttons used in the application.
 * The setupUI method initializes the UI elements and arranges them in a layout.
 * The class is designed to be used in conjunction with the main application window.
 * The UI elements include buttons for settings, recording, summarizing, and managing patients.
 * It also includes labels for displaying the title and patient name, as well as a combo box
 * for selecting patients. The class provides a consistent look and feel for the application.
 * The button styles are defined as static constants, allowing for easy customization and reuse.
 * The setupUI method takes pointers to the UI elements and arranges them in a layout.
 * @author Andres Pedreros Castro
 * @author Callum Thompson
 * @author Joelene Hales
 * @author Kalundi Serumaga
 * @author Thomas Llamzon
 */
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
                        QLabel *&summaryTitle,
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
    static const QString disabledButtonStyle;
};

#endif // WINDOWBUILDER_H
