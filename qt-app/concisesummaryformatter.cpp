/**
 * @file concisesummaryformatter.cpp
 * @brief Definition of ConciseSummaryFormatter class
 * @details Concise summary format displays the summaries of the current status and plan.
 * It is used to create a concise layout of the summary.
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

 // Libraries
#include <QLabel>
#include "concisesummaryformatter.h"


/**
 * @name generateLayout
 * @brief Creates a concise layout of the summary
 * @details Includes current status and plan sections.
 * Removes all other sections to keep the layout concise.
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 * @author Joelene Hales 
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
