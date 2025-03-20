/**
 * @file summary.h
 * @brief Declaration of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef SUMMARY_H
#define SUMMARY_H

#include <QString>

class Summary
{
public:
    Summary();
    Summary(const Summary& other);
    ~Summary() = default;

    Summary& operator=(const Summary& original);

    const QString& getIntervalHistory() const;
    void setIntervalHistory(const QString& intervalHistory);
    
    const QString& getPhysicalExamination() const;
    void setPhysicalExamination(const QString& physicalExamination);
    
    const QString& getCurrentStatus() const;
    void setCurrentStatus(const QString& currentStatus);
    
    const QString& getPlan() const;
    void setPlan(const QString& plan);
    

    void clear();

private:
    QString intervalHistory;
    QString physicalExamination;
    QString currentStatus;
    QString plan;
};

#endif // SUMMARY_H
