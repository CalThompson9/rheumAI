/**
 * @file main.cpp
 * @brief Main entry point for the Qt application.
 * @details This file initializes the QApplication, sets up the main window, and starts the event loop.
 * @author Callum Thompson
 * @author Joelene Hales
 * @author Andres Pedreros Castro
 * @author Kalundi Serumaga
 * @author Thomas Llamzon
 */

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include "mainwindow.h"
#include "filehandler.h"
#include "patientrecord.h"

/**
 * @name main
 * @brief Main function of the application.
 * @details Initializes the QApplication, sets the application icon, and creates the main window. 
 * @return int Exit status of the application.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @note This function is the entry point of the application. It sets up the QApplication and starts the event loop.
 * @author Callum Thompson
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QIcon icon(":/logo.png"); // Load the application icon
    if (!icon.isNull()) {
        QApplication::setWindowIcon(icon); // Set global application icon
    }

    // Initialize and show the main window
    MainWindow w;
    w.show();

    return a.exec();
}
