/**
 * @file addpatientdialog.h
 * @brief Declaration of AddPatientDialog class
 * @details This class provides a dialog interface for entering patient information.
 * It includes fields for first name, last name, date of birth, health card,
 * email, phone number, address, postal code, province, and country.
 * It also includes validation to ensure all fields are filled before submission.
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

/**
 * @class AddPatientDialog
 * @brief Dialog for adding a new patient
 * @details This class provides a dialog interface for entering patient information.
 * It includes fields for first name, last name, date of birth, health card,
 * email, phone number, address, postal code, province, and country.
 * It also includes validation to ensure all fields are filled before submission.
 * @note The dialog is modal, meaning it blocks input to other windows until closed.
 * @author Kalundi Serumaga
 * @author Joelene Hales
 * @date Mar. 16, 2025
 * @see QDialog
 */
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
    void accept();

private:
    Ui::AddPatientDialog *ui;
};

#endif // ADDPATIENTDIALOG_H
