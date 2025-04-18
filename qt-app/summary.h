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

/**
 * @class Summary
 * @brief Represents a summary with various sections
 * @details This class provides methods to manage the interval history,
 * physical examination, current status, and plan sections of a summary.
 * It includes methods to get and set these sections, as well as a method to clear all sections.
 * @author Joelene Hales
 * @author Callum Thompson
 */
class Summary
{
public:
    Summary();
    Summary(const Summary& other);
    ~Summary() = default;

    Summary& operator=(const Summary& original);

    QString getText() const;

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
