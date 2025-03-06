/**
 * @file summaryformatter.h
 * @brief Declaration of SummaryFormatter interface
 * TODO: Fix documentation for this, talk about design pattern
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef SUMMARYFORMATTER_H
#define SUMMARYFORMATTER_H

#include <QVBoxLayout>
#include "summary.h"

class SummaryFormatter 
{
    public:
        virtual QVBoxLayout* generateLayout(const Summary& summary) const = 0;
        virtual ~SummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion
};

#endif