/**
 * @file summary.h
 * @brief Declaration of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef SUMMARY_H
#define SUMMARY_H

#include <QString>

class Summary
{

    public:
        Summary();
        ~Summary() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion
        Summary& operator=(const Summary& original);

        const QString& getSymptoms() const;
        const QString& getDiagnoses() const;
        const QString& getMedicalHistory() const;
        const QString& getTreatmentPlans() const;

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

#endif // SUMMARY_H