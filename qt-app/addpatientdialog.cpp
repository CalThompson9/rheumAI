/**
 * @file addpatientdialog.cpp
 * @brief Dialog window for adding a new patient
 * @details This file contains the implementation of the AddPatientDialog class, 
 * which provides a user interface for entering patient information.
 * The dialog includes fields for first name, last name, date of birth, health card, 
 * email, phone number, address, postal code, province, and country.
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 16, 2025
 */

// include "addpatientdialog.h"
#include <QMessageBox>
#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"

/**
 * @name AddPatientDialog (constructor)
 * @brief Initializes a new dialog box to add a new patient
 * @details Sets up the user interface and loads the logo image. 
 * The logo is scaled to fit within the label.
 * @author Kalundi Serumaga
 * @param[in] parent: Qt parent widget
 */
AddPatientDialog::AddPatientDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPatientDialog) {
    ui->setupUi(this);
    QPixmap logo(":/logo.png"); // Resource path
    ui->logoLabel->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio));
}

/**
 * @name ~AddPatientDialog (destructor)
 * @brief Cleans up dynamically allocated user interface
 * @details Deletes the user interface object to free memory.
 * @author Kalundi Serumaga
 * @author Joelene Hales
 */
AddPatientDialog::~AddPatientDialog() {
    delete ui;
}

/**
 * @name getFirstName
 * @brief Gets the string entered in the first name field of the dialog box
 * @details Trims any leading or trailing spaces from the input. 
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered first name
 */
QString AddPatientDialog::getFirstName() const {
    return ui->firstNameInput->text().trimmed();  // Trim spaces
}

/**
 * @name getLastName
 * @brief Gets the string entered in the last name field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered last name
 */
QString AddPatientDialog::getLastName() const {
    return ui->lastNameInput->text().trimmed();
}

/**
 * @name getDateOfBirth
 * @brief Gets the string entered in the date of birth field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered date of birth
 */
QString AddPatientDialog::getDateOfBirth() const {
    return ui->dobInput->text().trimmed();
}

/**
 * @name getHealthCard
 * @brief Gets the string entered in the health card field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered health card number
 */
QString AddPatientDialog::getHealthCard() const {
    return ui->healthCardInput->text().trimmed();
}

/**
 * @name getEmail
 * @brief Gets the string entered in the email field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered email
 */
QString AddPatientDialog::getEmail() const {
    return ui->emailInput->text().trimmed();
}

/**
 * @name getPhoneNumber
 * @brief Gets the string entered in the phone number field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered phone number
 */
QString AddPatientDialog::getPhoneNumber() const {
    return ui->phoneNumberInput->text().trimmed();
}

/**
 * @name getAddress
 * @brief Gets the string entered in the address field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered address
 */
QString AddPatientDialog::getAddress() const {
    return ui->addressInput->text().trimmed();
}

/**
 * @name getPostalCode
 * @brief Gets the string entered in the postal code field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered postal code
 */
QString AddPatientDialog::getPostalCode() const {
    return ui->postalCodeInput->text().trimmed();
}

/**
 * @name getProvince
 * @brief Gets the string entered in the province field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered province
 */
QString AddPatientDialog::getProvince() const {
    return ui->provinceInput->text().trimmed();
}

/**
 * @name getCountry
 * @brief Gets the string entered in the country field of the dialog box
 * @details Trims any leading or trailing spaces from the input.
 * This is useful for ensuring that the input is clean and does not contain unnecessary spaces.
 * @author Kalundi Serumaga
 * @return The entered country
 */
QString AddPatientDialog::getCountry() const {
    return ui->countryInput->text().trimmed();
}

/**
 * @name accept
 * @brief Validate entered data, submit, and close the dialog window
 * @details This function is called when the user clicks the "OK" button.
 * It checks if all fields are filled in and shows a warning message if any field is empty.
 * If all fields are filled, it accepts the dialog and closes it.
 * @author Kalundi Serumaga
 * @author Joelene Hales
 * @return void
 */
void AddPatientDialog::accept() {
    // Validate inputs
    if (getFirstName().isEmpty() || 
        getLastName().isEmpty() || 
        getDateOfBirth().isEmpty() ||
        getHealthCard().isEmpty() ||
        getEmail().isEmpty() ||
        getPhoneNumber().isEmpty() ||
        getAddress().isEmpty() ||
        getPostalCode().isEmpty() ||
        getProvince().isEmpty() ||
        getCountry().isEmpty()
    )
    {
        QMessageBox::warning(this, "Missing Information", "Please fill in all fields.");
        return;
    }

    QDialog::accept();  // Close the dialog and return Accepted state
}