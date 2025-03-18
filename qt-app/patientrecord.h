#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#include <QString>
#include <QJsonObject>

class PatientRecord {
private:
    int patientID;
    QString firstName;
    QString lastName;
    QString dateOfBirth;

public:
    PatientRecord(int id, const QString &firstName, const QString &lastName, const QString &dob);
    PatientRecord(); // Default constructor

    int getID() const;
    QString getFirstName() const;
    QString getLastName() const;
    QString getDateOfBirth() const;

    QJsonObject toJson() const;
    static PatientRecord fromJson(const QJsonObject &json);
};

#endif // PATIENTRECORD_H
