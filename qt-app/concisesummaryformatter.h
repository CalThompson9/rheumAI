/**
 * @file concisesummaryformatter.h
 * @brief Declaration of ConciseSummaryFormatter class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#ifndef CONCISESUMMARYFORMATTER_H
#define CONCISESUMMARYFORMATTER_H

#include "summaryformatter.h"

/**
 * @class ConciseSummaryFormatter
 * @brief A class that formats the summary in a concise manner.
 * @details This class inherits from SummaryFormatter and implements the generateLayout method
 * to create a concise layout for the summary.
 * @author Joelene Hales
 */
class ConciseSummaryFormatter : public SummaryFormatter
{
    public:
        void generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const override;
        ~ConciseSummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion
};

#endif