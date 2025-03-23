/**
 * @file editpatientinfo.cpp
 * @brief Definition of EditPatientInfo class
 *
 * Dialog window that appears when the user would like to edit an existing patients data. Prompts the user
 * to change the patient's details, including their name and date of birth.
 *
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 22, 2025
 */

#include "editpatientinfo.h"
#include "ui_editpatientinfo.h"


/**
 * @name EditPatientIinfo (constructor)
 * @brief Initializes a new dialog box to edit patient details
 * @param[in] parent: Qt parent widget
 */
EditPatientInfo::EditPatientInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditPatientInfo)
{
    ui->setupUi(this);
    QPixmap logo(":/logo.png"); // Resource path
    ui->logoLabelEdit->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio));
}


/**
 * @name ~EditPatientInfo (destructor)
 * @brief Cleans up dynamically allocated user interface
 */
EditPatientInfo::~EditPatientInfo()
{
    delete ui;
}

// Setters

/**
 * @name setFirstName
 * @brief Sets/Updates the string entered in the first name field of the dialog box
 */
void EditPatientInfo::setFirstName(const QString &firstName) { ui->firstNameInput->setText(firstName); }

/**
 * @name setLastName
 * @brief Sets/Updates the string entered in the last name field of the dialog box
 */
void EditPatientInfo::setLastName(const QString &lastName) { ui->lastNameInput->setText(lastName); }

/**
 * @name gsetDateOfBirth
 * @brief Sets/Updates the string entered in the date of birth field of the dialog box
 */
void EditPatientInfo::setDateOfBirth(const QString &dob) { ui->dobInput->setDate(QDate::fromString(dob, "yyyy-MM-dd")); }

/**
 * @name setHealthCard
 * @brief Sets/Updates the string entered in the health card field of the dialog box
 */
void EditPatientInfo::setHealthCard(const QString &card) { ui->healthCardInput->setText(card); }

/**
 * @name setEmail
 * @brief Sets/Updates the string entered in the email field of the dialog box
 */
void EditPatientInfo::setEmail(const QString &email) { ui->emailInput->setText(email); }

/**
 * @name setPhoneNumber
 * @brief Sets/Updates the string entered in the phone number field of the dialog box
 */
void EditPatientInfo::setPhoneNumber(const QString &phone) { ui->phoneNumberInput->setText(phone); }

/**
 * @name setAddress
 * @brief Sets/Updates the string entered in the address field of the dialog box
 */
void EditPatientInfo::setAddress(const QString &address) { ui->addressInput->setText(address); }

/**
 * @name setPostalCode
 * @brief Sets/Updates the string entered in the postal code field of the dialog box
 */
void EditPatientInfo::setPostalCode(const QString &postalCode) { ui->postalCodeInput->setText(postalCode); }

/**
 * @name setProvince
 * @brief Sets/Updates the string entered in the province field of the dialog box
 */
void EditPatientInfo::setProvince(const QString &province) { ui->provinceInput->setText(province); }

/**
 * @name setCountry
 * @brief Sets/Updates the string entered in the country field of the dialog box
 */
void EditPatientInfo::setCountry(const QString &country) { ui->countryInput->setText(country); }

// Getters

/**
 * @name getFirstName
 * @brief Gets the string entered in the first name field of the dialog box
 * @return The entered first name
 */
QString EditPatientInfo::getFirstName() const { return ui->firstNameInput->text(); }

/**
 * @name getLastName
 * @brief Gets the string entered in the last name field of the dialog box
 * @return The entered last name
 */
QString EditPatientInfo::getLastName() const { return ui->lastNameInput->text(); }

/**
 * @name getDateOfBirth
 * @brief Gets the string entered in the date of birth field of the dialog box
 * @return The entered date of birth
 */
QString EditPatientInfo::getDateOfBirth() const { return ui->dobInput->date().toString("yyyy-MM-dd"); }

/**
 * @name getHealthCard
 * @brief Gets the string entered in the health card field of the dialog box
 * @return The entered health card number
 */
QString EditPatientInfo::getHealthCard() const { return ui->healthCardInput->text(); }

/**
 * @name getEmail
 * @brief Gets the string entered in the email field of the dialog box
 * @return The entered email
 */
QString EditPatientInfo::getEmail() const { return ui->emailInput->text(); }

/**
 * @name getPhoneNumber
 * @brief Gets the string entered in the phone number field of the dialog box
 * @return The entered phone number
 */
QString EditPatientInfo::getPhoneNumber() const { return ui->phoneNumberInput->text(); }

/**
 * @name getAddress
 * @brief Gets the string entered in the address field of the dialog box
 * @return The entered address
 */
QString EditPatientInfo::getAddress() const { return ui->addressInput->text(); }

/**
 * @name getPostalCode
 * @brief Gets the string entered in the postal code field of the dialog box
 * @return The entered postal code
 */
QString EditPatientInfo::getPostalCode() const { return ui->postalCodeInput->text(); }

/**
 * @name getProvince
 * @brief Gets the string entered in the province field of the dialog box
 * @return The entered province
 */
QString EditPatientInfo::getProvince() const { return ui->provinceInput->text(); }

/**
 * @name getCountry
 * @brief Gets the string entered in the country field of the dialog box
 * @return The entered country
 */
QString EditPatientInfo::getCountry() const { return ui->countryInput->text(); }
