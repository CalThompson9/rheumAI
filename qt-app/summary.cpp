/**
 * @file summary.cpp
 * @brief Definition of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#include "summary.h"

/**
 * @name Summary (Default Constructor)
 * @brief Creates an empty Summary object
 */
Summary::Summary() 
    : intervalHistory(""), 
      physicalExamination(""), 
      currentStatus(""), 
      plan("")
{
    // Default constructor initializes empty strings
}

/**
 * @name Summary (Copy Constructor)
 * @brief Creates a copy of a Summary
 * @param[in] other: Summary object to copy
 */
Summary::Summary(const Summary& other)
    : intervalHistory(other.intervalHistory),
      physicalExamination(other.physicalExamination),
      currentStatus(other.currentStatus),
      plan(other.plan)
{
}

/**
 * @name operator=
 * @brief Assigns one Summary to another
 * @param[in] original: Summary to copy
 * @return Reference to the updated Summary object
 */
Summary& Summary::operator=(const Summary& original)
{
    if (this != &original) // Prevent self-assignment
    {
        intervalHistory = original.intervalHistory;
        physicalExamination = original.physicalExamination;
        currentStatus = original.currentStatus;
        plan = original.plan;
    }
    return *this;
}

/**
 * @name clear
 * @brief Clears all fields of the summary
 */
void Summary::clear()
{
    intervalHistory.clear();
    physicalExamination.clear();
    currentStatus.clear();
    plan.clear();
}

/**
 * @name getText
 * @brief Retrieves the full summary as a formatted string
 * @return A QString containing the entire summary
 */
QString Summary::getText() const
{
    return QString("**INTERVAL HISTORY**\n%1\n\n"
                   "**PHYSICAL EXAMINATION**\n%2\n\n"
                   "**CURRENT STATUS**\n%3\n\n"
                   "**PLAN**\n%4\n")
        .arg(intervalHistory.isEmpty() ? "No interval history found." : intervalHistory)
        .arg(physicalExamination.isEmpty() ? "No physical examination found." : physicalExamination)
        .arg(currentStatus.isEmpty() ? "No current status found." : currentStatus)
        .arg(plan.isEmpty() ? "No plan found." : plan);
}


/**
 * @name getIntervalHistory
 * @brief Retrieves the interval history section of the summary
 * @return Interval history section
 */
const QString& Summary::getIntervalHistory() const { return intervalHistory; }

/**
 * @name setIntervalHistory
 * @brief Sets the interval history section of the summary
 * @param[in] newIntervalHistory: New interval history section
 */
void Summary::setIntervalHistory(const QString& newIntervalHistory) { intervalHistory = newIntervalHistory; }

/**
 * @name getPhysicalExamination
 * @brief Retrieves the physical examination section of the summary
 * @return Physical examination section
 */
const QString& Summary::getPhysicalExamination() const { return physicalExamination; }

/**
 * @name setPhysicalExamination
 * @brief Sets the physical examination section of the summary
 * @param[in] newPhysicalExamination: New physical examination section
 */
void Summary::setPhysicalExamination(const QString& newPhysicalExamination) { physicalExamination = newPhysicalExamination; }

/**
 * @name getCurrentStatus
 * @brief Retrieves the current status section of the summary
 * @return Current status section
 */
const QString& Summary::getCurrentStatus() const { return currentStatus; }

/**
 * @name setCurrentStatus
 * @brief Sets the current status section of the summary
 * @param[in] newCurrentStatus: New current status section
 */
void Summary::setCurrentStatus(const QString& newCurrentStatus) { currentStatus = newCurrentStatus; }

/**
 * @name getPlan
 * @brief Retrieves the plan section of the summary
 * @return Plan section
 */
const QString& Summary::getPlan() const { return plan; }

/**
 * @name setPlan
 * @brief Sets the plan section of the summary
 * @param[in] newPlan: New plan section
 */
void Summary::setPlan(const QString& newPlan) { plan = newPlan; }
