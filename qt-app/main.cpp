#include "mainwindow.h"
#include <QApplication>
#include "filehandler.h"
#include "patientrecord.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>  // Include this for debugging

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
