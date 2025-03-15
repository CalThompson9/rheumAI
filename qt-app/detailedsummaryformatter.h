/**
 * @file detailedsummaryformatter.h
 * @brief Declaration of DetailedSummaryFormatter class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef DETAILEDSUMMARYFORMATTER_H
#define DETAILEDSUMMARYFORMATTER_H

#include "summaryformatter.h"


class DetailedSummaryFormatter : public SummaryFormatter
{
    public:
        void generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const override;
        ~DetailedSummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion

};

#endif