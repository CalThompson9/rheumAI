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

    private:
        QString symptoms;
        QString diagnoses;
        QString medicalHistory;
        QString treatmentPlans;

};

#endif