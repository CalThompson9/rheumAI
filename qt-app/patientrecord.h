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
    PatientRecord();
    PatientRecord(int id, QString first, QString last, QString dob);

    int getPatientID() const;
    QString getFirstName() const;
    QString getLastName() const;
    QString getDateOfBirth() const;

    QJsonObject toJSON() const;
    static PatientRecord fromJSON(const QJsonObject &obj);
};

#endif // PATIENTRECORD_H
