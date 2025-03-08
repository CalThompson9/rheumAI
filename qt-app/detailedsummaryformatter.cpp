/**
 * @file detailedsummaryformatter.cpp
 * @brief Definition of DetailedSummaryFormatter class
 * TODO: Fix documentation for this class, talk about design patterns
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QLabel>
#include "detailedsummaryformatter.h"

/**
 * @name generateLayout
 * @brief Creates a detailed layout of the summary
 * @details Includes medical history, symptopms, diagnoses, and treatment plans
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 */
void DetailedSummaryFormatter::generateLayout(Summary& summary, QVBoxLayout* summaryLayout) const
{
    clearLayout(summaryLayout);  // Clear existing layout elements

    // Add sections
    addSection(QString("Symptoms"), summary.getSymptoms(), summaryLayout);
    addSection(QString("Medical History"), summary.getMedicalHistory(), summaryLayout);
    addSection(QString("Diagnoses"), summary.getDiagnoses(), summaryLayout);
    addSection(QString("Treatment Plans"), summary.getTreatmentPlans(), summaryLayout);
}
