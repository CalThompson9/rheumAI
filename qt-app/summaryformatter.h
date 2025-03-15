/**
 * @file summaryformatter.h
 * @brief Declaration of SummaryFormatter class
 * @details Implements the Strategy pattern
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef SUMMARYFORMATTER_H
#define SUMMARYFORMATTER_H

#include <QVBoxLayout>
#include <QRegularExpression>
#include <QTextBrowser>
#include "summary.h"


class SummaryFormatter 
{
    public:

        struct Section {
            QString title;
            QString content;
        };

        virtual void generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const = 0;
        virtual ~SummaryFormatter() = default;  // Qt automatically manages memory of QObjects, no need for manual deletion
  
        static QString formatBoldText(const QString& text);
        static void clearLayout(QVBoxLayout* layout);
        
    protected:
        void displayNoSummaryText(QVBoxLayout* summaryLayout) const;
        void addSection(const Section& section, QVBoxLayout* summaryLayout) const;
};

#endif