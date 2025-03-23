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
 * @param[in] healthCard: Patient's health card number (ex OHIP)
 * @param[in] firstName: Patient's first name
 * @param[in] lastName: Patient's last name
 * @param[in] dateOfBirth: Patient's date of birth
 * @param[in] email: Patient's email
 * @param[in] phoneNumber: Patient's phone number
 * @param[in] address: Patient's address
 * @param[in] postalCode: Patient's postal code
 * @param[in] province: Patient's province
 * @param[in] country: Patient's country
 */
PatientRecord::PatientRecord(
    int id,
    const QString& healthCard,
    const QString &firstName,
    const QString &lastName,
    const QString &dateOfBirth,
    const QString &email,
    const QString &phoneNumber,
    const QString &address,
    const QString &postalCode,
    const QString &province,
    const QString &country
) : patientID(id),
    healthCard(healthCard),
    firstName(firstName),
    lastName(lastName),
    dateOfBirth(dateOfBirth),
    email(email),
    phoneNumber(phoneNumber),
    address(address),
    postalCode(postalCode),
    province(province),
    country(country)
{
    // No logic body
}

/**
 * @name PatientRecord (default constructor)
 * @brief Default constructor creates an empty patient record
 * @details Initializes ID to -1 and all strings to empty
 */
PatientRecord::PatientRecord()
    : patientID(-1),
      healthCard(""),
      firstName(""),
      lastName(""),
      dateOfBirth(""),
      email(""),
      phoneNumber(""),
      address(""),
      postalCode(""),
      province(""),
      country("")
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
 * @name getHealthCard
 * @brief Get the patient's health card number
 * @return Patient's health card number
 */
QString PatientRecord::getHealthCard() const
{
    return healthCard;
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
 * @name getEmail
 * @brief Gets the patient's email
 * @return Patient's email
 */
QString PatientRecord::getEmail() const
{
    return email;
}

/**
 * @name getPhoneNumber
 * @brief Gets the patient's phone number
 * @return Patient's phone number
 */
QString PatientRecord::getPhoneNumber() const
{
    return phoneNumber;
}

/**
 * @name getHomeAddress
 * @brief Gets the patient's home address
 * @return Patient's home address
 */
QString PatientRecord::getAddress() const
{
    return address;
}

/**
 * @name getPostalCode
 * @brief Gets the patient's postal code
 * @return Patient's postal code
 */
QString PatientRecord::getPostalCode() const
{
    return postalCode;
}

/**
 * @name getProvince
 * @brief Gets the patient's province
 * @return Patient's province
 */
QString PatientRecord::getProvince() const
{
    return province;
}

/**
 * @name getCountry
 * @brief Gets the patient's country
 * @return Patient's country
 */
QString PatientRecord::getCountry() const
{
    return country;
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
    json["healthCard"] = healthCard;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["dateOfBirth"] = dateOfBirth;
    json["email"] = email;
    json["phoneNumber"] = phoneNumber;
    json["address"] = address;
    json["postalCode"] = postalCode;
    json["province"] = province;
    json["country"] = country;
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
        json["healthCard"].toString(),
        json["firstName"].toString(),
        json["lastName"].toString(),
        json["dateOfBirth"].toString(),
        json["email"].toString(),
        json["phoneNumber"].toString(),
        json["address"].toString(),
        json["postalCode"].toString(),
        json["province"].toString(),
        json["country"].toString()
        );
}



void PatientRecord::setFirstName(const QString &value) {
    firstName = value;
}

void PatientRecord::setLastName(const QString &value) {
    lastName = value;
}

void PatientRecord::setDateOfBirth(const QString &value) {
    dateOfBirth = value;
}

void PatientRecord::setHealthCard(const QString &value) {
    healthCard = value;
}

void PatientRecord::setEmail(const QString &value) {
    email = value;
}

void PatientRecord::setPhoneNumber(const QString &value) {
    phoneNumber = value;
}

void PatientRecord::setAddress(const QString &value) {
    address = value;
}

void PatientRecord::setPostalCode(const QString &value) {
    postalCode = value;
}

void PatientRecord::setProvince(const QString &value) {
    province = value;
}

void PatientRecord::setCountry(const QString &value) {
    country = value;
}
