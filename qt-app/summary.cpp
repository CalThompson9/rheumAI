/**
 * @file summary.cpp
 * @brief Definition of Summary class
 * 
 * @details Stores the transcript summary generated by the LLM. Includes summaries of
 * the interval history, physical examination, current status, and plan.
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 6, 2025
 */

#include "summary.h"

/**
 * @name Summary (Default Constructor)
 * @brief Constructor creates an empty Summary object.
 * @details Initializes all strings to empty. 
 * @author Joelene Hales
 */
Summary::Summary() 
    : intervalHistory(""), 
      physicalExamination(""), 
      currentStatus(""), 
      plan("")
{
    // No logic body
}

/**
 * @name Summary (Copy Constructor)
 * @brief Creates a copy of a Summary
 * @details Initializes the new Summary object with the values from the provided object.
 * @param[in] other: Summary object to copy
 * @author Joelene Hales
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
 * @details Copies the values from the provided Summary object to the current object.
 * @param[in] original: Summary to copy
 * @return Reference to the updated Summary object
 * @author Joelene Hales
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
 * @details Sets all strings to empty. This is useful for resetting the summary
 *          before generating a new one.
 * @author Joelene Hales
 */
void Summary::clear()
{
    // Clear all sections of the summary
    intervalHistory.clear();
    physicalExamination.clear();
    currentStatus.clear();
    plan.clear();
}

/**
 * @name getText
 * @brief Retrieves the full summary as a formatted string
 * @return A QString containing the entire summary
 * @details The summary is formatted with section headers for interval history,
 *          physical examination, current status, and plan. Each section is
 *          separated by a newline. If any section is empty, a default message
 *          is included in its place.
 * @author Joelene Hales
 * @author Callum Thompson
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
 * @name Summary::getIntervalHistory
 * @brief Retrieves the interval history section of the summary
 * @details Returns a reference to the interval history field
 * @return Interval history section as a const QString reference
 * @author Joelene Hales
 */
const QString& Summary::getIntervalHistory() const {
    return intervalHistory;
}

/**
 * @name Summary::setIntervalHistory
 * @brief Sets the interval history section of the summary
 * @details Assigns a new value to the interval history field
 * @param[in] newIntervalHistory New interval history section
 * @author Joelene Hales
 */
void Summary::setIntervalHistory(const QString& newIntervalHistory) {
    intervalHistory = newIntervalHistory;
}

/**
 * @name Summary::getPhysicalExamination
 * @brief Retrieves the physical examination section of the summary
 * @details Returns a reference to the physical examination field
 * @return Physical examination section as a const QString reference
 * @author Joelene Hales
 */
const QString& Summary::getPhysicalExamination() const {
    return physicalExamination;
}

/**
 * @name Summary::setPhysicalExamination
 * @brief Sets the physical examination section of the summary
 * @details Assigns a new value to the physical examination field
 * @param[in] newPhysicalExamination New physical examination section
 * @author Joelene Hales
 */
void Summary::setPhysicalExamination(const QString& newPhysicalExamination) {
    physicalExamination = newPhysicalExamination;
}

/**
 * @name Summary::getCurrentStatus
 * @brief Retrieves the current status section of the summary
 * @details Returns a reference to the current status field
 * @return Current status section as a const QString reference
 * @author Joelene Hales
 */
const QString& Summary::getCurrentStatus() const {
    return currentStatus;
}

/**
 * @name Summary::setCurrentStatus
 * @brief Sets the current status section of the summary
 * @details Assigns a new value to the current status field
 * @param[in] newCurrentStatus New current status section
 * @author Joelene Hales
 */
void Summary::setCurrentStatus(const QString& newCurrentStatus) {
    currentStatus = newCurrentStatus;
}

/**
 * @name Summary::getPlan
 * @brief Retrieves the plan section of the summary
 * @details Returns a reference to the plan field
 * @return Plan section as a const QString reference
 * @author Joelene Hales
 */
const QString& Summary::getPlan() const {
    return plan;
}

/**
 * @name Summary::setPlan
 * @brief Sets the plan section of the summary
 * @details Assigns a new value to the plan field
 * @param[in] newPlan New plan section
 * @author Joelene Hales
 */
void Summary::setPlan(const QString& newPlan) {
    plan = newPlan;
}

