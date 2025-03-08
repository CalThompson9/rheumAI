/**
 * @file summary.cpp
 * @brief Definition of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#include "summary.h"

Summary::Summary(QObject *parent) : QObject(parent)
{
    // No logic body; QStrings get initialized to null by default
}

Summary::~Summary()
{
    // Qt automatically manages memory of QObjects, no need for manual deletion
}

/**
 * @name getSymptoms
 * @brief Get summary of symptoms
 * @return Summary of symptoms
 */
const QString& Summary::getSymptoms() const
{
    return symptoms;
}

/**
 * @name getDiagnoses
 * @brief Get summary of diagnoses
 * @return Summary of diagnoses
 */
const QString& Summary::getDiagnoses() const
{
    return diagnoses;
}

/**
 * @name getMedicalHistory
 * @brief Get summary of medical history
 * @return Summary of medical history
 */
const QString& Summary::getMedicalHistory() const
{
    return medicalHistory;
}

/**
 * @name getTreatmentPlans
 * @brief Get summary of treatment plans
 * @return Summary of treatment plans
 */
const QString& Summary::getTreatmentPlans() const
{
    return treatmentPlans;
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
