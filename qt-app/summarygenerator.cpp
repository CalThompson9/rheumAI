/**
 * @name summarygenerator.cpp
 * @brief Definition of SummaryGenerator builder class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#include "summarygenerator.h"

SummaryGenerator::SummaryGenerator(const Transcript& transcript) : transcript(transcript)
{
    // No logic body
}

SummaryGenerator& SummaryGenerator::summarizeSymptoms()
{
    // TODO: Implement
    return *this;
}

SummaryGenerator& SummaryGenerator::summarizeDiagnoses()
{
    // TODO: Implement
    return *this;
}

SummaryGenerator& SummaryGenerator::summarizeMedicalHistory()
{
    // TODO: Implement
    return *this;
}

SummaryGenerator& SummaryGenerator::summarizeTreatmentPlans()
{
    // TODO: Implement
    return *this;
}

Summary SummaryGenerator::getSummary()
{
    return summary;
}
