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

class Summary : public QObject
{
    Q_OBJECT

    public:
        explicit Summary(QObject *parent = nullptr);
        ~Summary();

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

#endif