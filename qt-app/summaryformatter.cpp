/**
 * @file summaryformatter.cpp
 * @brief Definition of SummaryFormatter class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 7, 2025
 */

#include <QWidget>
#include <QLabel>
#include "summaryformatter.h"

/**
 * @name clearLayout
 * @brief Clears all layout elements
 * @param[in, out] layout: Layout to be cleared
 */
void SummaryFormatter::clearLayout(QVBoxLayout* layout) 
{
    if (!layout) {  // Ensure layout exists before accessing it
        return;
    }

    while (QLayoutItem *item = layout->takeAt(0))
    {
        if (QWidget *widget = item->widget())
        {
            widget->deleteLater();  // Mark for deletion when safe
        }
        delete item;  // Destroy item and free memory
    }
}

/**
 * @name displayEmptyText
 * @brief Displays a message indicating that there is no summary data to display
 * @param[in, out] summaryLayout: Layout to add message to
 */
void SummaryFormatter::displayNoSummaryText(QVBoxLayout* summaryLayout) const
{
    QLabel* textWidget = new QLabel("No data to display. Try generating a summary!");
    summaryLayout->addWidget(textWidget);
}

/**
 * @name addSection
 * @brief Adds a section to the summary layout
 * @param[in] title: Section title
 * @param[in] text: Body text
 * @param[in, out] summaryLayout: Layout to add section to
 */
void SummaryFormatter::addSection(const QString& title, const QString& text, QVBoxLayout* summaryLayout) const
{
    // Generate layout elements
    QLabel* titleWidget = new QLabel(title);
    QLabel* textWidget = new QLabel(!text.isEmpty() ? text : "Nothing to display.");

    // Set styling
    titleWidget->setStyleSheet("font-weight: bold; font-size: 13px;");
    textWidget->setWordWrap(true);

    // Add elements to layout
    summaryLayout->addWidget(titleWidget);
    summaryLayout->addWidget(textWidget);
}

/**
 * @name formatBoldText
 * @brief Formats bold text in the summary
 * @param[in] text: Text to format
 * @return Formatted text with bold tags
 */
QString SummaryFormatter::formatBoldText(const QString& text) const
{
    QString formattedText = text;

    // Replace **bold text** with <b>bold text</b>
    QRegularExpression boldRegex(R"(\*\*(.*?)\*\*)");
    formattedText.replace(boldRegex, "<b>\\1</b>");

    // Preserve new lines by replacing \n with <br>
    formattedText.replace("\n", "<br>");

    return formattedText;
}
