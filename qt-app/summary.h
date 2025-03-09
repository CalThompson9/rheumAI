/**
 * @file summary.h
 * @brief Declaration of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef SUMMARY_H
#define SUMMARY_H

#include <QObject>

class Summary
{

    public:
        Summary();
        ~Summary();

        const QString& getSymptoms();
        const QString& getDiagnoses();
        const QString& getMedicalHistory();
        const QString& getTreatmentPlans();

        void setSymptoms(const QString& symptoms);
        void setDiagnoses(const QString& diagnoses);
        void setMedicalHistory(const QString& medicalHistory);
        void setTreatmentPlans(const QString& treatmentPlans);

    private:
        QString symptoms;
        QString diagnoses;
        QString medicalHistory;
        QString treatmentPlans;

};

#endif