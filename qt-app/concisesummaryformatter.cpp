/**
 * @file concisesummaryformatter.cpp
 * @brief Definition of ConciseSummaryFormatter class
 * 
 * Concise summary format displays the summaries of the current status and plan.
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QLabel>
#include "concisesummaryformatter.h"


/**
 * @name generateLayout
 * @brief Creates a concise layout of the summary
 * @details Includes current status and plan
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 */
void ConciseSummaryFormatter::generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const
{
    clearLayout(summaryLayout);  // Clear existing layout elements

    // Define sections
    QList<Section> sections = {
        {"Current Status:", summary.getCurrentStatus()},
        {"Plan:", summary.getPlan()}
    };

    // Add each section to layout
    for (const Section& section : sections)
    {
        addSection(section, summaryLayout);
    }

}
