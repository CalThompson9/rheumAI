/**
 * @file concisesummaryformatter.cpp
 * @brief Definition of ConciseSummaryFormatter class
 * TODO: Fix documentation for this class, talk about design patterns
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QLabel>
#include "concisesummaryformatter.h"

/**
 * @name generateLayout
 * @brief Creates a concise layout of the summary
 * @details Includes symptoms and diagnoses
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 */
void ConciseSummaryFormatter::generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const
{
    clearLayout(summaryLayout);  // Clear existing layout elements

    // WILL BE IMPLEMENTED IN FUTURE ITERATION (CALLUM WILL DO IT)
}
