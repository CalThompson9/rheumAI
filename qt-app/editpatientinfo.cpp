/**
 * @file editpatientinfo.cpp
 * @brief Definition of EditPatientInfo class.
 * @details
 * Dialog window that appears when the user wants to edit an existing patient's data.
 * Prompts the user to update details such as the patient's name, date of birth,
 * health card number, email, phone number, and address.
 * The dialog is tied to a Qt UI form and supports both data entry and retrieval.
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 22, 2025
 */

 #include "editpatientinfo.h"
 #include "ui_editpatientinfo.h"
 
 /**
  * @name EditPatientInfo::EditPatientInfo
  * @brief Constructor for EditPatientInfo dialog.
  * @details
  * Initializes the dialog UI for editing patient details.
  * Sets up all visual components, including the application logo and form fields.
  * @param[in] parent The parent widget of the dialog, default is nullptr.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 EditPatientInfo::EditPatientInfo(QWidget *parent)
     : QDialog(parent)
     , ui(new Ui::EditPatientInfo)
 {
     ui->setupUi(this);
     QPixmap logo(":/logo.png");
     ui->logoLabelEdit->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio));
 }
 
 /**
  * @name EditPatientInfo::~EditPatientInfo
  * @brief Destructor for EditPatientInfo.
  * @details Cleans up the dynamically allocated UI resources for the dialog.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 EditPatientInfo::~EditPatientInfo()
 {
     delete ui;
 }
 
 /**
  * @name EditPatientInfo::setFirstName
  * @brief Sets the patient's first name.
  * @details Updates the first name field in the dialog.
  * @param[in] firstName The first name to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setFirstName(const QString &firstName)
 {
     ui->firstNameInput->setText(firstName);
 }
 
 /**
  * @name EditPatientInfo::setLastName
  * @brief Sets the patient's last name.
  * @details Updates the last name field in the dialog.
  * @param[in] lastName The last name to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setLastName(const QString &lastName)
 {
     ui->lastNameInput->setText(lastName);
 }
 
 /**
  * @name EditPatientInfo::setDateOfBirth
  * @brief Sets the patient's date of birth.
  * @details Updates the date field in the dialog using the format yyyy-MM-dd.
  * @param[in] dob The date of birth to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setDateOfBirth(const QString &dob)
 {
     ui->dobInput->setDate(QDate::fromString(dob, "yyyy-MM-dd"));
 }
 
 /**
  * @name EditPatientInfo::setHealthCard
  * @brief Sets the patient's health card number.
  * @details Updates the health card input field with the given string.
  * @param[in] card The health card number to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setHealthCard(const QString &card)
 {
     ui->healthCardInput->setText(card);
 }
 
 /**
  * @name EditPatientInfo::setEmail
  * @brief Sets the patient's email address.
  * @details Updates the email input field with the given string.
  * @param[in] email The email to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setEmail(const QString &email)
 {
     ui->emailInput->setText(email);
 }
 
 /**
  * @name EditPatientInfo::setPhoneNumber
  * @brief Sets the patient's phone number.
  * @details Updates the phone number input field with the given string.
  * @param[in] phone The phone number to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setPhoneNumber(const QString &phone)
 {
     ui->phoneNumberInput->setText(phone);
 }
 
 /**
  * @name EditPatientInfo::setAddress
  * @brief Sets the patient's address.
  * @details Updates the address input field with the given string.
  * @param[in] address The address to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setAddress(const QString &address)
 {
     ui->addressInput->setText(address);
 }
 
 /**
  * @name EditPatientInfo::setPostalCode
  * @brief Sets the patient's postal code.
  * @details Updates the postal code input field with the given string.
  * @param[in] postalCode The postal code to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setPostalCode(const QString &postalCode)
 {
     ui->postalCodeInput->setText(postalCode);
 }
 
 /**
  * @name EditPatientInfo::setProvince
  * @brief Sets the patient's province.
  * @details Updates the province input field with the given string.
  * @param[in] province The province to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setProvince(const QString &province)
 {
     ui->provinceInput->setText(province);
 }
 
 /**
  * @name EditPatientInfo::setCountry
  * @brief Sets the patient's country.
  * @details Updates the country input field with the given string.
  * @param[in] country The country to be set.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 void EditPatientInfo::setCountry(const QString &country)
 {
     ui->countryInput->setText(country);
 }
 
 /**
  * @name EditPatientInfo::getFirstName
  * @brief Gets the entered first name.
  * @details Retrieves the string from the first name input field.
  * @return The patient's first name as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getFirstName() const
 {
     return ui->firstNameInput->text();
 }
 
 /**
  * @name EditPatientInfo::getLastName
  * @brief Gets the entered last name.
  * @details Retrieves the string from the last name input field.
  * @return The patient's last name as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getLastName() const
 {
     return ui->lastNameInput->text();
 }
 
 /**
  * @name EditPatientInfo::getDateOfBirth
  * @brief Gets the entered date of birth.
  * @details Retrieves the value from the date picker in yyyy-MM-dd format.
  * @return The patient's date of birth as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getDateOfBirth() const
 {
     return ui->dobInput->date().toString("yyyy-MM-dd");
 }
 
 /**
  * @name EditPatientInfo::getHealthCard
  * @brief Gets the entered health card number.
  * @details Retrieves the string from the health card input field.
  * @return The health card number as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getHealthCard() const
 {
     return ui->healthCardInput->text();
 }
 
 /**
  * @name EditPatientInfo::getEmail
  * @brief Gets the entered email address.
  * @details Retrieves the string from the email input field.
  * @return The email address as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getEmail() const
 {
     return ui->emailInput->text();
 }
 
 /**
  * @name EditPatientInfo::getPhoneNumber
  * @brief Gets the entered phone number.
  * @details Retrieves the string from the phone number input field.
  * @return The phone number as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getPhoneNumber() const
 {
     return ui->phoneNumberInput->text();
 }
 
 /**
  * @name EditPatientInfo::getAddress
  * @brief Gets the entered address.
  * @details Retrieves the string from the address input field.
  * @return The address as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getAddress() const
 {
     return ui->addressInput->text();
 }
 
 /**
  * @name EditPatientInfo::getPostalCode
  * @brief Gets the entered postal code.
  * @details Retrieves the string from the postal code input field.
  * @return The postal code as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getPostalCode() const
 {
     return ui->postalCodeInput->text();
 }
 
 /**
  * @name EditPatientInfo::getProvince
  * @brief Gets the entered province.
  * @details Retrieves the string from the province input field.
  * @return The province as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getProvince() const
 {
     return ui->provinceInput->text();
 }
 
 /**
  * @name EditPatientInfo::getCountry
  * @brief Gets the entered country.
  * @details Retrieves the string from the country input field.
  * @return The country as a QString.
  * @author Kalundi Serumaga
  * @author Joelene Hales
  */
 QString EditPatientInfo::getCountry() const
 {
     return ui->countryInput->text();
 }
 