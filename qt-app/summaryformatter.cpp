/**
 * @file summaryformatter.cpp
 * @brief Definition of SummaryFormatter class
 *
 * Provides an interface for all summary layout formats.
 *
 * @author Joelene Hales (jhales5@uwo.ca)
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 7, 2025
 */

#include "summaryformatter.h"

/**
 * @name clearLayout
 * @brief Clears all layout elements
 * @param[in, out] layout: Layout to be cleared
 */
void SummaryFormatter::clearLayout(QVBoxLayout *layout) const
{
    if (!layout)
    { // Ensure layout exists before accessing it
        return;
    }

    while (QLayoutItem *item = layout->takeAt(0))
    {
        if (QWidget *widget = item->widget())
        {
            widget->deleteLater(); // Mark for deletion when safe
        }
        delete item; // Destroy item and free memory
    }
}

/**
 * @name formatBoldText
 * @brief Formats bold text in the summary
 * @param[in] text: Text to format
 * @return Formatted text with bold tags
 */
QString SummaryFormatter::formatBoldText(const QString &text) const
{
    QString formattedText = text;

    // Replace **bold text** with <b>bold text</b>
    QRegularExpression boldRegex(R"(\*\*(.*?)\*\*)");
    formattedText.replace(boldRegex, "<b>\\1</b>");

    // Preserve new lines by replacing \n with <br>
    formattedText.replace("\n", "<br>");

    return formattedText;
}

/**
 * @name addSection
 * @brief Adds a section to the summary layout
 * @param[in] section: Section content
 * @param[in, out] summaryLayout: Layout to add section to
 */
void SummaryFormatter::addSection(const Section &section, QVBoxLayout *summaryLayout) const
{
    // Create layout elements
    QLabel *sectionLabel = new QLabel(section.title);
    QTextBrowser *sectionText = new QTextBrowser();

    // Set styling
    sectionText->setHtml(formatBoldText(section.content)); // Use HTML formatting. Format any bold elements.
    sectionText->setReadOnly(true);
    sectionText->setFixedHeight(150); // Adjust height as needed

    // Add layouts to widget
    summaryLayout->addWidget(sectionLabel);
    summaryLayout->addWidget(sectionText);
}
