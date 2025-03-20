/**
 * @file detailedsummaryformatter.cpp
 * @brief Definition of DetailedSummaryFormatter class
 * 
 * Detailed summary format displays the summaries of the interval history,
 * physical examination, current status, and plan.
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QLabel>
#include "detailedsummaryformatter.h"

/**
 * @name generateLayout
 * @brief Creates a detailed layout of the summary
 * @details Includes interval history, physical examination, current status, and plan
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 */
void DetailedSummaryFormatter::generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const
{
    clearLayout(summaryLayout);  // Clear existing layout elements

    // Define sections
    QList<Section> sections = {
        {"Interval History:", summary.getIntervalHistory()},
        {"Physical Examination:", summary.getPhysicalExamination()},
        {"Current Status:", summary.getCurrentStatus()},
        {"Plan:", summary.getPlan()}
    };

    // Add each section to layout
    for (const Section& section : sections)
    {
        addSection(section, summaryLayout);
    }

}

