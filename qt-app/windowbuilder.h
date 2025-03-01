#ifndef WINDOWBUILDER_H
#define WINDOWBUILDER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class WindowBuilder {
public:
    static void setupUI(QWidget *centralWidget,
                        QPushButton *&btnConnectDevice,
                        QPushButton *&btnSettings,
                        QLabel *&lblTitle,
                        QLabel *&lblPatientName,
                        QComboBox *&comboSelectPatient,
                        QPushButton *&btnRecord,
                        QTextEdit *&textTranscription,
                        QVBoxLayout *&mainLayout);
};

#endif // WINDOWBUILDER_H
