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

class ConciseSummaryFormatter : public SummaryFormatter
{
    public:
        void generateLayout(Summary& summary, QVBoxLayout* summaryLayout) const override;
        ~ConciseSummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion
};

#endif