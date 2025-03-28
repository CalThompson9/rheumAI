/**
 * @file detailedsummaryformatter.h
 * @brief Declaration of DetailedSummaryFormatter class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef DETAILEDSUMMARYFORMATTER_H
#define DETAILEDSUMMARYFORMATTER_H

// Libraries
#include "summaryformatter.h"

/**
 * @class DetailedSummaryFormatter
 * @brief Class to format detailed summaries
 * @details This class is responsible for generating a detailed layout of the summary.
 * It includes sections for interval history, physical examination, current status, and plan.
 * This is the standard detailed summary format.
 * @author Joelene Hales
 */
class DetailedSummaryFormatter : public SummaryFormatter
{
    public:
        void generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const override;
        ~DetailedSummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion

};

#endif