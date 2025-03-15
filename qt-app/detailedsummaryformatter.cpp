/**
 * @file detailedsummaryformatter.cpp
 * @brief Definition of DetailedSummaryFormatter class
 * TODO: Fix documentation for this class, talk about design patterns
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QLabel>
#include "detailedsummaryformatter.h"

/**
 * @name generateLayout
 * @brief Creates a detailed layout of the summary
 * @details Includes medical history, symptopms, diagnoses, and treatment plans
 * @param[in] summary: Summary to create layout for
 * @param[in,out] summaryLayout: Layout to display summary in. Existing layout elements will be cleared.
 */
void DetailedSummaryFormatter::generateLayout(const Summary& summary, QVBoxLayout* summaryLayout) const
{
    clearLayout(summaryLayout);  // Clear existing layout elements

    struct Section {
        QString title;
        QString content;
    };

    QList<Section> sections = {
        {"Interval History:", formatBoldText(summary.getIntervalHistory())},
        {"Physical Examination:", formatBoldText(summary.getPhysicalExamination())},
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

/**
 * @name formatBoldText
 * @brief Formats bold text in the summary
 * @param[in] text: Text to format
 * @return Formatted text with bold tags
 */
QString DetailedSummaryFormatter::formatBoldText(const QString& text) const
{
    QString formattedText = text;

    // Replace **bold text** with <b>bold text</b>
    QRegularExpression boldRegex(R"(\*\*(.*?)\*\*)");
    formattedText.replace(boldRegex, "<b>\\1</b>");

    // Preserve new lines by replacing \n with <br>
    formattedText.replace("\n", "<br>");

    return formattedText;
}


