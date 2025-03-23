/**
 * @file addpatientdialog.h
 * @brief Declaration of AddPatientDialog class
 * 
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 16, 2025
 */

#ifndef ADDPATIENTDIALOG_H
#define ADDPATIENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddPatientDialog;
}

class AddPatientDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddPatientDialog(QWidget *parent = nullptr);
    ~AddPatientDialog();
    
    QString getFirstName() const;
    QString getLastName() const;
    QString getDateOfBirth() const;
    QString getHealthCard() const;
    QString getEmail() const;
    QString getPhoneNumber() const;
    QString getAddress() const;
    QString getPostalCode() const;
    QString getProvince() const;
    QString getCountry() const;

private slots:
    void accept();  // Ensure the dialog properly submits data

private:
    Ui::AddPatientDialog *ui;
};

#endif // ADDPATIENTDIALOG_H
