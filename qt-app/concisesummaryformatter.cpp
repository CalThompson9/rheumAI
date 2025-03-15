/**
 * @file concisesummaryformatter.cpp
 * @brief Definition of ConciseSummaryFormatter class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QLabel>
#include "concisesummaryformatter.h"

/**
 * @name generateLayout
 * @brief Creates a concise layout of the summary
 * @details Includes current status and plan
 *          TODO: This is just a placeholder. Update later.
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 */
void ConciseSummaryFormatter::generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const
{
    clearLayout(summaryLayout);  // Clear existing layout elements

    struct Section {
        QString title;
        QString content;
    };

    // TODO: This is just a placeholder. Will update the contents of this format later
    QList<Section> sections = {
        {"Current Status:", formatBoldText(summary.getCurrentStatus())},
        {"Plan:", formatBoldText(summary.getPlan())}
    };

    for (const Section& section : sections)
    {
        QLabel* sectionLabel = new QLabel(section.title);
        QTextBrowser* sectionText = new QTextBrowser();
        sectionText->setHtml(section.content); // Use HTML formatting
        sectionText->setReadOnly(true);
        sectionText->setFixedHeight(150); // Adjust height as needed

        summaryLayout->addWidget(sectionLabel);
        summaryLayout->addWidget(sectionText);
    }

}
