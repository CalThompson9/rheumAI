#include "patientrecord.h"

PatientRecord::PatientRecord(int id, const QString &firstName, const QString &lastName, const QString &dob)
    : patientID(id), firstName(firstName), lastName(lastName), dateOfBirth(dob) {}

PatientRecord::PatientRecord() : patientID(-1), firstName(""), lastName(""), dateOfBirth("") {}

int PatientRecord::getID() const { return patientID; }
QString PatientRecord::getFirstName() const { return firstName; }
QString PatientRecord::getLastName() const { return lastName; }
QString PatientRecord::getDateOfBirth() const { return dateOfBirth; }

QJsonObject PatientRecord::toJson() const {
    QJsonObject json;
    json["patientID"] = patientID;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["dateOfBirth"] = dateOfBirth;
    return json;
}

PatientRecord PatientRecord::fromJson(const QJsonObject &json) {
    return PatientRecord(
        json["patientID"].toInt(),
        json["firstName"].toString(),
        json["lastName"].toString(),
        json["dateOfBirth"].toString()
        );
}
