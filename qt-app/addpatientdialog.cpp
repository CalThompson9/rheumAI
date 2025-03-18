/**
 * @file addpatientdialog.cpp
 * @brief Definition of AddPatientDialog class
 * 
 * Dialog window that appears when the user adds a new patient. Prompts the user
 * to enter the patient's details, including their name and date of birth.
 * 
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @date Mar. 16, 2025
 */

#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"
#include <QMessageBox>

/**
 * @name AddPatientDialog (constructor)
 * @brief Initializes a new dialog box to add a new patient
 * @param[in] parent: Qt parent widget
 */
AddPatientDialog::AddPatientDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPatientDialog) {
    ui->setupUi(this);
}

/**
 * @name ~AddPatientDialog (destructor)
 * @brief Cleans up dynamically allocated user interface
 */
AddPatientDialog::~AddPatientDialog() {
    delete ui;
}

/**
 * @name getFirstName
 * @brief Gets the string entered in the first name field of the dialog box
 * @return The entered first name
 */
QString AddPatientDialog::getFirstName() const {
    return ui->firstNameInput->text().trimmed();  // Trim spaces
}

/**
 * @name getLastName
 * @brief Gets the string entered in the last name field of the dialog box
 * @return The entered last name
 */
QString AddPatientDialog::getLastName() const {
    return ui->lastNameInput->text().trimmed();
}

/**
 * @name getDateOfBirth
 * @brief Gets the string entered in the date of birth field of the dialog box
 * @return The entered date of birth
 */
QString AddPatientDialog::getDateOfBirth() const {
    return ui->dobInput->text().trimmed();
}

/**
 * @name accept
 * @brief Validate entered data, submit, and close the dialog window
 */
void AddPatientDialog::accept() {
    // Validate inputs
    if (getFirstName().isEmpty() || getLastName().isEmpty() || getDateOfBirth().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all fields.");
        return;
    }

    QDialog::accept();  // Close the dialog and return Accepted state
}
