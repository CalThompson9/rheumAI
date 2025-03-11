/**
 * @file summaryformatter.h
 * @brief Declaration of SummaryFormatter class
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
        virtual void generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const = 0;
        virtual ~SummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion

        static void clearLayout(QVBoxLayout* layout);
    
    protected:
        void displayNoSummaryText(QVBoxLayout* summaryLayout) const;
        void addSection(const QString& title, const QString& text, QVBoxLayout* summaryLayout) const;
};

#endif