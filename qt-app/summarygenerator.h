/**
 * @name summarygenerator.h
 * @brief Declaration of SummaryGenerator builder class
 * TODO: Add to documentation explaining design patterns
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#ifndef SUMMARYGENERATOR_H
#define SUMMARYGENERATOR_H

#include "summary.h"

class SummaryGenerator
{

    public:
        SummaryGenerator();
        ~SummaryGenerator() = default;

        SummaryGenerator& summarizeSymptoms();
        SummaryGenerator& summarizeDiagnoses();
        SummaryGenerator& summarizeMedicalHistory();
        SummaryGenerator& summarizeTreatmentPlans();
        Summary getSummary();

    private:
        Summary summary;

};

#endif // SUMMARYGENERATOR_H