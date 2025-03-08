#include "patientrecord.h"

PatientRecord::PatientRecord() : patientID(0) {}

PatientRecord::PatientRecord(int id, QString first, QString last, QString dob)
    : patientID(id), firstName(first), lastName(last), dateOfBirth(dob) {}

int PatientRecord::getPatientID() const { return patientID; }
QString PatientRecord::getFirstName() const { return firstName; }
QString PatientRecord::getLastName() const { return lastName; }
QString PatientRecord::getDateOfBirth() const { return dateOfBirth; }

QJsonObject PatientRecord::toJSON() const {
    QJsonObject obj;
    obj["patientID"] = patientID;
    obj["firstName"] = firstName;
    obj["lastName"] = lastName;
    obj["dateOfBirth"] = dateOfBirth;
    return obj;
}

PatientRecord PatientRecord::fromJSON(const QJsonObject &obj) {
    return PatientRecord(
        obj["patientID"].toInt(),
        obj["firstName"].toString(),
        obj["lastName"].toString(),
        obj["dateOfBirth"].toString()
        );
}
