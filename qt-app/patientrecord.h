/**
 * @file patientrecord.h
 * @brief Declaration of PatientRecord class
 * 
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @date Mar. 16, 2025
 */

#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#include <QString>
#include <QJsonObject>

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

    QJsonObject toJson() const;
    static PatientRecord fromJson(const QJsonObject &json);
};

#endif // PATIENTRECORD_H
