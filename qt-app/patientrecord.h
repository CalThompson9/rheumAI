/**
 * @file patientrecord.h
 * @brief Declaration of PatientRecord class
 * @details This class represents a patient's record in the system. It includes
 *          personal information such as name, date of birth, contact details,
 *          and health card information. The class provides methods to access
 *          and modify these details, as well as to convert the record to and
 *          from JSON format for easy storage and retrieval.
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @date Mar. 16, 2025
 */

#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#include <QString>
#include <QJsonObject>

/**
 * @class PatientRecord
 * @brief Represents a patient's record in the system.
 * @details This class contains personal information such as name, date of birth,
 *         contact details, and health card information. It provides methods to
 *        access and modify these details, as well as to convert the record to
 *       and from JSON format for easy storage and retrieval.
 * @author Kalundi Serumaga
 */
class PatientRecord {
private:
    int patientID;
    QString healthCard;
    QString firstName;
    QString lastName;
    QString dateOfBirth;
    QString email;
    QString phoneNumber;
    QString address;
    QString postalCode;
    QString province;
    QString country;

public:
    PatientRecord(
        int id,
        const QString& healthCard,
        const QString &firstName,
        const QString &lastName,
        const QString &dob,
        const QString &email,
        const QString &phoneNumber,
        const QString &address,
        const QString &postalCode,
        const QString &province,
        const QString &country);
    PatientRecord(); // Default constructor

    int getID() const;
    QString getHealthCard() const;
    QString getFirstName() const;
    QString getLastName() const;
    QString getDateOfBirth() const;
    QString getEmail() const;
    QString getPhoneNumber() const;
    QString getAddress() const;
    QString getPostalCode() const;
    QString getProvince() const;
    QString getCountry() const;

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


    QJsonObject toJson() const; 
    static PatientRecord fromJson(const QJsonObject &json);
};

#endif // PATIENTRECORD_H
