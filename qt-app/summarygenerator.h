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

#include <QObject>
#include "transcript.h"
#include "summary.h"

class SummaryGenerator : public QObject
{
    Q_OBJECT

    public:
        explicit SummaryGenerator(Transcript& transcript, QObject *parent = nullptr);
        virtual ~SummaryGenerator();

        SummaryGenerator& summarizeSymptoms();
        SummaryGenerator& summarizeDiagnoses();
        SummaryGenerator& summarizeMedicalHistory();
        SummaryGenerator& summarizeTreatmentPlans();
        Summary getSummary();

    private:
        Transcript& transcript;
        Summary summary;

};

#endif // SUMMARYGENERATOR_H