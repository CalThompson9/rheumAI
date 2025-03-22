/**
 * @file patientrecord.cpp
 * @brief Definition of PatientRecord class
 * 
 * Stores the information about a single patient, including their ID number,
 * name, and date of birth. Includes methods to read/write the record from/to a
 * JSON file.
 * 
 * @author Kalundi Serumaga (kserumag@uwo.ca)
 * @date Mar. 16, 2025
 */

#include "patientrecord.h"

/**
 * @name PatientRecord (constructor)
 * @brief Initializes a patient record
 * @param[in] id: Patient ID
 * @param[in] firstName: Patient's first name
 * @param[in] lastName: Patient's last name
 * @param[in] dateOfBirth: Patient's date of birth
 */
PatientRecord::PatientRecord(int id, const QString &firstName, const QString &lastName, const QString &dob)
    : patientID(id), firstName(firstName), lastName(lastName), dateOfBirth(dob) 
{
    // No logic body
}

/**
 * @name PatientRecord (default constructor)
 * @brief Default constructor creates an empty patient record
 * @details Initializes ID to -1 and all strings to empty
 */
PatientRecord::PatientRecord() : patientID(-1), firstName(""), lastName(""), dateOfBirth("") 
{
    // No logic body
}

/**
 * @name getId
 * @brief Get the patient's ID number
 * @return Patient ID
 */
int PatientRecord::getID() const
{
    return patientID;
}

/**
 * @name getFirstName
 * @brief Get the patient's first name
 * @return Patient's first name
 */
QString PatientRecord::getFirstName() const
{
    return firstName;
}

/**
 * @name getLastName
 * @brief Get the patient's last name
 * @return Patient's last name
 */
QString PatientRecord::getLastName() const
{
    return lastName;
}

/**
 * @name getDateOfBirth
 * @brief Gets the patient's date of birth
 * @return Patient's date of birth
 */
QString PatientRecord::getDateOfBirth() const
{
    return dateOfBirth;
}

/**
 * @name toJson
 * @brief Converts the patient record to a JSON object
 * @return JSON object storing the patient record
 */
QJsonObject PatientRecord::toJson() const
{
    QJsonObject json;
    json["patientID"] = patientID;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["dateOfBirth"] = dateOfBirth;
    return json;
}

/**
 * @name fromJson
 * @brief Creates a patient record from a JSON object
 * @param[in] json: JSON object containing patient record data
 * @return Patient record
 */
PatientRecord PatientRecord::fromJson(const QJsonObject &json)
{
    return PatientRecord(
        json["patientID"].toInt(),
        json["firstName"].toString(),
        json["lastName"].toString(),
        json["dateOfBirth"].toString()
        );
}
