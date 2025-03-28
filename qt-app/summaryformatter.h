/**
 * @file summaryformatter.h
 * @brief Declaration of SummaryFormatter class
 * @details Implements the Strategy pattern
 *
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 6, 2025
 */

#ifndef SUMMARYFORMATTER_H
#define SUMMARYFORMATTER_H

#include <QVBoxLayout>
#include <QRegularExpression>
#include <QTextBrowser>
#include <QWidget>
#include <QLabel>
#include "summary.h"

/**
 * @class SummaryFormatter
 * @brief Abstract base class for formatting summary layouts
 * @details This class provides an interface for all summary layout formats.
 * It defines a method to generate the layout and a method to format bold text.
 * The class also provides a method to clear the layout and a method to add sections.
 * The formatting of the summary is done using HTML tags.
 * The class is designed to be inherited by specific formatters that implement the generateLayout method.
 * The class uses the Strategy pattern to allow for different formatting strategies.
 * @author Joelene Hales
 * @author Callum Thompson
 */
class SummaryFormatter
{
public:
    struct Section
    {
        QString title;   // Section title
        QString content; // Body text
    };

    virtual void generateLayout(const Summary &summary, QVBoxLayout *summaryLayout) const = 0;
    virtual ~SummaryFormatter() = default; // Qt automatically manages memory of QObjects, no need for manual deletion

protected:
    QString formatBoldText(const QString &text) const;
    void clearLayout(QVBoxLayout *layout) const;
    void addSection(const Section &section, QVBoxLayout *summaryLayout) const;
};

#endif