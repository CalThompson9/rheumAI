/**
 * @file summarygenerator.cpp
 * @brief Definition of SummaryGenerator builder class
 *
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 *
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 12, 2025
 */

#include "summarygenerator.h"

/**
 * @name SummaryGenerator
 * @brief Constructor for the SummaryGenerator class
 * @details Sends the transcript to the LLM and stores the structured response.
 * @param[in] transcript: Transcript to summarize
 * @param[in] parent: Parent object in Qt application
 * @author Joelene Hales
 */
SummaryGenerator::SummaryGenerator(QObject *parent)
    : QObject(parent)
{
    // Create an LLMClient object
    llmClient = LLMClient::getInstance();

    // Connect the LLMClient signal to the handler
    connect(llmClient, &LLMClient::responseReceived, this, &SummaryGenerator::handleLLMResponse, Qt::UniqueConnection);
}

/**
 * @name SendRequest
 * @brief Sends a request to the LLMClient
 * @param[in] prompt: Prompt to send to the LLMClient
 * @details Sends a request to the LLMClient with the given prompt
 * and stores the response in the summary object
 * @author Callum Thompson
 */
void SummaryGenerator::sendRequest(Transcript &prompt)
{
    llmClient->sendRequest(prompt.getContent());
}

/**
 * @name handleLLMResponse
 * @brief Handles the LLM response and updates Summary object
 * @details This function is called when the LLMClient emits a responseReceived signal.
 * It extracts the relevant sections from the LLM response and updates the summary object.
 * The sections are extracted based on the expected format of the LLM response.
 * @param[in] response: Full LLM-generated summary
 * @author Callum Thompson
 */
void SummaryGenerator::handleLLMResponse(const QString &response)
{

    summarizeIntervalHistory(response);
    summarizePhysicalExamination(response);
    summarizeCurrentStatus(response);
    summarizePlan(response);

    emit summaryReady();
}

/**
 * @name setSummary
 * @brief Sets the summary object with the given text
 * @details Sets the summary object with the given text
 * The summary object is cleared before setting the new text.
 * @param[in] newSummary: New summary object to set
 * @author Callum Thompson
 * @author Joelene Hales
 */
void SummaryGenerator::setSummary(const Summary &newSummary)
{
    summary.clear(); // Clear the current summary
    summary.setIntervalHistory(newSummary.getIntervalHistory());
    summary.setPhysicalExamination(newSummary.getPhysicalExamination());
    summary.setCurrentStatus(newSummary.getCurrentStatus());
    summary.setPlan(newSummary.getPlan());
    emit summaryReady();
}

/**
 * @name setSummaryText
 * @brief Sets the summary object with the given text
 * @details Sets the summary object with the given text
 * The summary object is cleared before setting the new text.
 * @param[in] summaryText: New summary text to set
 * @author Callum Thompson
 * @author Joelene Hales
 */
void SummaryGenerator::setSummaryText(const QString &summaryText)
{
    if (summaryText.isEmpty())
    {
        return;
    }

    summary.clear(); // Reset the current summary

    // Ensure LLM response follows expected structure
    summarizeIntervalHistory(summaryText);
    summarizePhysicalExamination(summaryText);
    summarizeCurrentStatus(summaryText);
    summarizePlan(summaryText);

    emit summaryReady();
}

/**
 * @name summarizeIntervalHistory
 * @brief Extracts and stores the interval history section of the summary
 * @details This function extracts the interval history section from the LLM response
 * and stores it in the summary object. It uses a specific search pattern to find the
 * section header and extracts the text until the next section header.
 * @param[in] response: Full LLM-generated summary
 * @author Callum Thompson
 * @author Joelene Hales
 */
void SummaryGenerator::summarizeIntervalHistory(const QString &response)
{
    summary.setIntervalHistory(extractSectionFromResponse(response, "INTERVAL HISTORY", "PHYSICAL EXAMINATION"));
}

/**
 * @name summarizePhysicalExamination
 * @brief Extracts and stores the physical examination section of the summary
 * @details This function extracts the physical examination section from the LLM response
 * and stores it in the summary object. It uses a specific search pattern to find the
 * section header and extracts the text until the next section header.
 * @param[in] response: Full LLM-generated summary
 * @author Callum Thompson
 * @author Joelene Hales
 */
void SummaryGenerator::summarizePhysicalExamination(const QString &response)
{
    summary.setPhysicalExamination(extractSectionFromResponse(response, "PHYSICAL EXAMINATION", "CURRENT STATUS"));
}

/**
 * @name summarizeCurrentStatus
 * @brief Extracts and stores the current status section of the summary
 * @details This function extracts the current status section from the LLM response
 * and stores it in the summary object. It uses a specific search pattern to find the
 * section header and extracts the text until the next section header.
 * @param[in] response: Full LLM-generated summary
 * @author Callum Thompson
 * @author Joelene Hales
 */
void SummaryGenerator::summarizeCurrentStatus(const QString &response)
{
    summary.setCurrentStatus(extractSectionFromResponse(response, "CURRENT STATUS", "PLAN"));
}

/**
 * @name summarizePlan
 * @brief Extracts and stores the plan section of the summary
 * @details This function extracts the plan section from the LLM response
 * and stores it in the summary object. It uses a specific search pattern to find the
 * section header and extracts the text until the next section header.
 * @param[in] response: Full LLM-generated summary
 * @author Callum Thompson
 * @author Joelene Hales
 */
void SummaryGenerator::summarizePlan(const QString &response)
{
    summary.setPlan(extractSectionFromResponse(response, "PLAN", "PHYSICAL EXAMINATION"));
}

/**
 * @name extractSectionFromResponse
 * @brief Extracts a specific section from the LLM response
 * @details This function extracts a specific section from the LLM response
 * based on the section name provided. It uses a specific search pattern to find the
 * section header and extracts the text until the next section header.
 * @param[in] response: Full text response from LLM
 * @param[in] sectionName: Section name to extract
 * @param[in] nextSectionName: Next section name to stop extraction
 * @return Extracted text of the section
 * @author Callum Thompson
 * @author Joelene Hales
 */
QString SummaryGenerator::extractSectionFromResponse(const QString &response, const QString &sectionName, const QString &nextSectionName)
{
    QString searchPattern = "**" + sectionName + "**";
    QString searchPatternAlt = "**" + sectionName + ":**";

    int startIndex = response.indexOf(searchPattern, 0, Qt::CaseInsensitive);
    int patternLength = searchPattern.length(); // Default to original pattern length

    if (startIndex == -1)
    {
        startIndex = response.indexOf(searchPatternAlt, 0, Qt::CaseInsensitive);
        if (startIndex == -1)
        {
            return "No " + sectionName.toLower() + " found.";
        }
        patternLength = searchPatternAlt.length(); // Use alt pattern length if matched
    }

    // Look for the next section by name, allowing both **nextSection** and **nextSection:**
    QString nextPattern = "**" + nextSectionName + "**";
    QString nextPatternAlt = "**" + nextSectionName + ":**";

    int endIndex = response.indexOf(nextPattern, startIndex + patternLength, Qt::CaseInsensitive);
    if (endIndex == -1)
        endIndex = response.indexOf(nextPatternAlt, startIndex + patternLength, Qt::CaseInsensitive);
    if (endIndex == -1)
        endIndex = response.length();

    QString extractedSection = response.mid(startIndex + patternLength, endIndex - (startIndex + patternLength)).trimmed();

    return extractedSection;
}

/**
 * @name getSummary
 * @brief Returns the generated summary
 * @details This function returns the generated summary object.
 * @return Generated summary
 * @author Joelene Hales
 */
Summary SummaryGenerator::getSummary()
{
    return summary;
}
