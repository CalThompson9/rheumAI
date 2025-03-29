/**
 * @file editpatientinfo.h
 * @brief Declaration of EditPatientInfo class
 *
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 22, 2025
 */

#ifndef EDITPATIENTINFO_H
#define EDITPATIENTINFO_H

#include <QDialog>

namespace Ui {
class EditPatientInfo;
}

/**
 * @class EditPatientInfo
 * @brief A dialog for editing patient information.
 * @details This class provides a user interface for editing various patient details
 * including first name, last name, date of birth, health card number, email,
 * phone number, address, postal code, province, and country. The dialog is
 * initialized with a logo and provides methods to set and get the values
 * entered in the fields.
 * @author Kalundi Serumaga
 * @author Joelene Hales
 */
class EditPatientInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditPatientInfo(QWidget *parent = nullptr);
    ~EditPatientInfo();

    // Setters
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setDateOfBirth(const QString &dob);
    void setHealthCard(const QString &card);
    void setEmail(const QString &email);
    void setPhoneNumber(const QString &phone);
    void setAddress(const QString &address);
    void setPostalCode(const QString &postalCode);
    void setProvince(const QString &province);
    void setCountry(const QString &country);

    // Getters
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


private:
    Ui::EditPatientInfo *ui;
};

#endif // EDITPATIENTINFO_H
