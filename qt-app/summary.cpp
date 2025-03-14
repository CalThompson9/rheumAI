/**
 * @file summary.cpp
 * @brief Definition of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#include "summary.h"

Summary::Summary() 
    : symptoms(""), diagnoses(""), medicalHistory(""), treatmentPlans(""), physicalExamination(""), socialHistory("")
{
    // Default constructor initializes empty strings
}

/**
 * @name Summary (Copy Constructor)
 * @brief Creates a copy of a Summary
 * @param[in] other: Summary object to copy
 */
Summary::Summary(const Summary& other)
    : symptoms(other.symptoms), 
      diagnoses(other.diagnoses), 
      medicalHistory(other.medicalHistory), 
      treatmentPlans(other.treatmentPlans),
      physicalExamination(other.physicalExamination),
      socialHistory(other.socialHistory)
{
}

/**
 * @name operator=
 * @brief Assigns one Summary to another
 * @param[in] original: Summary to copy
 * @return Reference to the updated Summary object
 */
Summary& Summary::operator=(const Summary& original)
{
    if (this != &original) // Prevent self-assignment
    {
        symptoms = original.symptoms;
        diagnoses = original.diagnoses;
        medicalHistory = original.medicalHistory;
        treatmentPlans = original.treatmentPlans;
        physicalExamination = original.physicalExamination;
        socialHistory = original.socialHistory;
    }
    return *this;
}

/**
 * @name clear
 * @brief Clears all fields of the summary
 */
void Summary::clear()
{
    symptoms.clear();
    diagnoses.clear();
    medicalHistory.clear();
    treatmentPlans.clear();
    physicalExamination.clear();
    socialHistory.clear();
}

/**
 * @name getSymptoms
 * @brief Get a read-only summary of symptoms
 * @return Summary of symptoms
 */
const QString& Summary::getSymptoms() const
{
    return symptoms;
}

/**
 * @name getDiagnoses
 * @brief Get a read-only summary of diagnoses
 * @return Summary of diagnoses
 */
const QString& Summary::getDiagnoses() const
{
    return diagnoses;
}

/**
 * @name getMedicalHistory
 * @brief Get a read-only summary of medical history
 * @return Summary of medical history
 */
const QString& Summary::getMedicalHistory() const
{
    return medicalHistory;
}

/**
 * @name getTreatmentPlans
 * @brief Get a read-only summary of treatment plans
 * @return Summary of treatment plans
 */
const QString& Summary::getTreatmentPlans() const
{
    return treatmentPlans;
}

/**
 * @name getPhysicalExamination
 * @brief Get a read-only summary of physical examination
 * @return Summary of physical examination
 */
const QString& Summary::getPhysicalExamination() const
{
    return physicalExamination;
}

/**
 * @name getSocialHistory
 * @brief Get a read-only summary of social history
 * @return Summary of social history
 */
const QString& Summary::getSocialHistory() const
{
    return socialHistory;
}


/**
 * @name setSymptoms
 * @brief Sets the symptoms summary
 * @param[in] newSymptoms: Symptoms summary
 */
void Summary::setSymptoms(const QString& newSymptoms)
{
    symptoms = newSymptoms;
}

/**
 * @name setDiagnoses
 * @brief Sets the diagnoses summary
 * @param[in] newDiagnoses: Diagnoses summary
 */
void Summary::setDiagnoses(const QString& newDiagnoses)
{
    diagnoses = newDiagnoses;
}

/**
 * @name setMedicalHistory
 * @brief Sets the medical history summary
 * @param[in] newMedicalHistory: Medical history summary
 */
void Summary::setMedicalHistory(const QString& newMedicalHistory)
{
    medicalHistory = newMedicalHistory;
}

/**
 * @name setTreatmentPlans
 * @brief Sets the treatment plans summary
 * @param[in] newTreatmentPlans: Treatment plans summary
 */
void Summary::setTreatmentPlans(const QString& newTreatmentPlans)
{
    treatmentPlans = newTreatmentPlans;
}

/**
 * @name setPhysicalExamination
 * @brief Sets the physical examination summary
 * @param[in] newPhysicalExamination: Physical examination summary
 */
void Summary::setPhysicalExamination(const QString& newPhysicalExamination)
{
    physicalExamination = newPhysicalExamination;
}

/**
 * @name setSocialHistory
 * @brief Sets the social history summary
 * @param[in] newSocialHistory: Social history summary
 */
void Summary::setSocialHistory(const QString& newSocialHistory)
{
    socialHistory = newSocialHistory;
}