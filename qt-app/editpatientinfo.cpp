#include "editpatientinfo.h"
#include "ui_editpatientinfo.h"

editpatientinfo::editpatientinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editpatientinfo)
{
    ui->setupUi(this);
}

editpatientinfo::~editpatientinfo()
{
    delete ui;
}

// Setters
void editpatientinfo::setFirstName(const QString &firstName) { ui->firstNameInput->setText(firstName); }
void editpatientinfo::setLastName(const QString &lastName) { ui->lastNameInput->setText(lastName); }
void editpatientinfo::setDateOfBirth(const QString &dob) { ui->dobInput->setDate(QDate::fromString(dob, "yyyy-MM-dd"));
 }
void editpatientinfo::setHealthCard(const QString &card) { ui->healthCardInput->setText(card); }
void editpatientinfo::setEmail(const QString &email) { ui->emailInput->setText(email); }
void editpatientinfo::setPhoneNumber(const QString &phone) { ui->phoneNumberInput->setText(phone); }
void editpatientinfo::setAddress(const QString &address) { ui->addressInput->setText(address); }
void editpatientinfo::setPostalCode(const QString &postalCode) { ui->postalCodeInput->setText(postalCode); }
void editpatientinfo::setProvince(const QString &province) { ui->provinceInput->setText(province); }
void editpatientinfo::setCountry(const QString &country) { ui->countryInput->setText(country); }

// Getters
QString editpatientinfo::getFirstName() const { return ui->firstNameInput->text(); }
QString editpatientinfo::getLastName() const { return ui->lastNameInput->text(); }
QString editpatientinfo::getDateOfBirth() const { return ui->dobInput->date().toString("yyyy-MM-dd"); }
QString editpatientinfo::getHealthCard() const { return ui->healthCardInput->text(); }
QString editpatientinfo::getEmail() const { return ui->emailInput->text(); }
QString editpatientinfo::getPhoneNumber() const { return ui->phoneNumberInput->text(); }
QString editpatientinfo::getAddress() const { return ui->addressInput->text(); }
QString editpatientinfo::getPostalCode() const { return ui->postalCodeInput->text(); }
QString editpatientinfo::getProvince() const { return ui->provinceInput->text(); }
QString editpatientinfo::getCountry() const { return ui->countryInput->text(); }
