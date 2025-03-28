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
 * @details Sets the patient ID and all other fields to the provided values.
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
 * @author Kalundi Serumaga
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
 * @author Kalundi Serumaga
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
 * @details The ID is a unique identifier for the patient
 * @return Patient ID
 * @author Kalundi Serumaga
 */
int PatientRecord::getID() const
{
    return patientID;
}

/**
 * @name getHealthCard
 * @brief Get the patient's health card number
 * @details The health card number is used to identify the patient
 * @return Patient's health card number
 * @author Kalundi Serumaga
 */
QString PatientRecord::getHealthCard() const
{
    return healthCard;
}

/**
 * @name getFirstName
 * @brief Get the patient's first name
 * @details First name field stored in the patients file
 * @return Patient's first name
 * @author Kalundi Serumaga
 */
QString PatientRecord::getFirstName() const
{
    return firstName;
}

/**
 * @name getLastName
 * @brief Get the patient's last name
 * @details Last name field stored in the patients file
 * @return Patient's last name
 * @author Kalundi Serumaga
 */
QString PatientRecord::getLastName() const
{
    return lastName;
}

/**
 * @name getDateOfBirth
 * @brief Gets the patient's date of birth
 * @details The date of birth stored in the patients file
 * @return Patient's date of birth
 * @author Kalundi Serumaga
 */
QString PatientRecord::getDateOfBirth() const
{
    return dateOfBirth;
}

/**
 * @name getEmail
 * @brief Gets the patient's email
 * @details The email stored in the patients file
 * @return Patient's email
 * @author Kalundi Serumaga
 */
QString PatientRecord::getEmail() const
{
    return email;
}

/**
 * @name getPhoneNumber
 * @brief Gets the patient's phone number
 * @details The phone number stored in the patients file
 * @return Patient's phone number
 * @author Kalundi Serumaga
 */
QString PatientRecord::getPhoneNumber() const
{
    return phoneNumber;
}

/**
 * @name getHomeAddress
 * @brief Gets the patient's home address
 * @details The home address stored in the patients file
 * @return Patient's home address
 * @author Kalundi Serumaga
 */
QString PatientRecord::getAddress() const
{
    return address;
}

/**
 * @name getPostalCode
 * @brief Gets the patient's postal code
 * @details The postal code stored in the patients file
 * @return Patient's postal code
 * @author Kalundi Serumaga
 */
QString PatientRecord::getPostalCode() const
{
    return postalCode;
}

/**
 * @name getProvince
 * @brief Gets the patient's province
 * @details The province stored in the patients file
 * @return Patient's province
 * @author Kalundi Serumaga
 */
QString PatientRecord::getProvince() const
{
    return province;
}

/**
 * @name getCountry
 * @brief Gets the patient's country
 * @details The country stored in the patients file
 * @return Patient's country
 * @author Kalundi Serumaga
 */
QString PatientRecord::getCountry() const
{
    return country;
}

/**
 * @name toJson
 * @brief Converts the patient record to a JSON object
 * @details This function creates a JSON object that contains all the
 * patient's information. The JSON object can be used to save the
 * patient record to a file or send it over the network.
 * @return JSON object storing the patient record
 * @author Kalundi Serumaga
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
 * @details This function takes a JSON object that contains
 * all the patient's information and creates a new patient record
 * from it. The JSON object can be used to load the patient record
 * from a file or receive it over the network.
 * @author Kalundi Serumaga
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


/**
 * @name PatientRecord::setFirstName
 * @brief Sets the patient's first name
 * @details Assigns the provided value to the patient's first name field
 * @param[in] value The patient's first name
 * @author Kalundi Serumaga
 */
void PatientRecord::setFirstName(const QString &value) {
    firstName = value;
}

/**
 * @name PatientRecord::setLastName
 * @brief Sets the patient's last name
 * @details Assigns the provided value to the patient's last name field
 * @param[in] value The patient's last name
 * @author Kalundi Serumaga
 */
void PatientRecord::setLastName(const QString &value) {
    lastName = value;
}

/**
 * @name PatientRecord::setDateOfBirth
 * @brief Sets the patient's date of birth
 * @details Assigns the provided value to the patient's date of birth field
 * @param[in] value The patient's date of birth
 * @author Kalundi Serumaga
 */
void PatientRecord::setDateOfBirth(const QString &value) {
    dateOfBirth = value;
}

/**
 * @name PatientRecord::setHealthCard
 * @brief Sets the patient's health card number
 * @details Assigns the provided value to the patient's health card field
 * @param[in] value The patient's health card number
 * @author Kalundi Serumaga
 */
void PatientRecord::setHealthCard(const QString &value) {
    healthCard = value;
}

/**
 * @name PatientRecord::setEmail
 * @brief Sets the patient's email address
 * @details Assigns the provided value to the patient's email field
 * @param[in] value The patient's email address
 * @author Kalundi Serumaga
 */
void PatientRecord::setEmail(const QString &value) {
    email = value;
}

/**
 * @name PatientRecord::setPhoneNumber
 * @brief Sets the patient's phone number
 * @details Assigns the provided value to the patient's phone number field
 * @param[in] value The patient's phone number
 * @author Kalundi Serumaga
 */
void PatientRecord::setPhoneNumber(const QString &value) {
    phoneNumber = value;
}

/**
 * @name PatientRecord::setAddress
 * @brief Sets the patient's address
 * @details Assigns the provided value to the patient's address field
 * @param[in] value The patient's address
 * @author Kalundi Serumaga
 */
void PatientRecord::setAddress(const QString &value) {
    address = value;
}

/**
 * @name PatientRecord::setPostalCode
 * @brief Sets the patient's postal code
 * @details Assigns the provided value to the patient's postal code field
 * @param[in] value The patient's postal code
 * @author Kalundi Serumaga
 */
void PatientRecord::setPostalCode(const QString &value) {
    postalCode = value;
}

/**
 * @name PatientRecord::setProvince
 * @brief Sets the patient's province
 * @details Assigns the provided value to the patient's province field
 * @param[in] value The patient's province
 * @author Kalundi Serumaga
 */
void PatientRecord::setProvince(const QString &value) {
    province = value;
}

/**
 * @name PatientRecord::setCountry
 * @brief Sets the patient's country
 * @details Assigns the provided value to the patient's country field
 * @param[in] value The patient's country
 * @author Kalundi Serumaga
 */
void PatientRecord::setCountry(const QString &value) {
    country = value;
}

