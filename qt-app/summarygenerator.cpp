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

#include <QDebug>
#include "summarygenerator.h"

/**
 * @name SummaryGenerator
 * @brief Constructor for the SummaryGenerator class
 * @details Sends the transcript to the LLM and stores the structured response.
 * @param[in] transcript: Transcript to summarize
 * @param[in] parent: Parent object in Qt application
 */
SummaryGenerator::SummaryGenerator(QObject *parent)
    : QObject(parent)
{
    // Create an LLMClient object
    llmClient = new LLMClient(this);

    // Connect the LLMClient signal to the handler
    connect(llmClient, &LLMClient::responseReceived, this, &SummaryGenerator::handleLLMResponse);
}

/**
 * @name SendRequest
 * @brief Sends a request to the LLMClient
 * @param[in] prompt: Prompt to send to the LLMClient
 * @details Sends a request to the LLMClient with the given prompt
 * and stores the response in the summary object
 */
void SummaryGenerator::sendRequest(Transcript &prompt)
{
    llmClient->sendRequest(prompt.getContent());
}

/**
 * @name handleLLMResponse
 * @brief Handles the LLM response and updates Summary object
 * @param[in] response: Full LLM-generated summary
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
 * @name summarizeIntervalHistory
 * @brief Extracts and stores the interval history section of the summary
 * @param[in] response: Full LLM-generated summary
 */
void SummaryGenerator::summarizeIntervalHistory(const QString &response)
{
    summary.setIntervalHistory(extractSectionFromResponse(response, "INTERVAL HISTORY"));
}

/**
 * @name summarizePhysicalExamination
 * @brief Extracts and stores the physical examination section of the summary
 * @param[in] response: Full LLM-generated summary
 */
void SummaryGenerator::summarizePhysicalExamination(const QString &response)
{
    summary.setPhysicalExamination(extractSectionFromResponse(response, "PHYSICAL EXAMINATION"));
}

/**
 * @name summarizeCurrentStatus
 * @brief Extracts and stores the current status section of the summary
 * @param[in] response: Full LLM-generated summary
 */
void SummaryGenerator::summarizeCurrentStatus(const QString &response)
{
    summary.setCurrentStatus(extractSectionFromResponse(response, "CURRENT STATUS"));
}

/**
 * @name summarizePlan
 * @brief Extracts and stores the plan section of the summary
 * @param[in] response: Full LLM-generated summary
 */
void SummaryGenerator::summarizePlan(const QString &response)
{
    summary.setPlan(extractSectionFromResponse(response, "PLAN"));
}

/**
 * @name extractSectionFromResponse
 * @brief Extracts a specific section from the LLM response
 * @param[in] response: Full text response from LLM
 * @param[in] sectionName: Section name to extract
 * @return Extracted text of the section
 */
QString SummaryGenerator::extractSectionFromResponse(const QString &response, const QString &sectionName)
{
    QString searchPattern = "**" + sectionName + "**";
    int startIndex = response.indexOf(searchPattern);

    if (startIndex == -1)
    {
        qWarning() << "🔍 Section not found in LLM response: " << sectionName;
        return "No " + sectionName.toLower() + " found.";
    }

    // Find the next section header (e.g., "**SOMETHING ELSE**")
    QRegularExpression regex(R"(\n\*\*[A-Z ]+\*\*)"); // Matches new section headers
    QRegularExpressionMatch match = regex.match(response, startIndex + searchPattern.length());

    int endIndex = match.hasMatch() ? match.capturedStart() : response.length(); // If no match, take entire string

    // Extract only the section content
    return response.mid(startIndex + searchPattern.length(), endIndex - (startIndex + searchPattern.length())).trimmed();
}

/**
 * @brief getSummary
 * @brief Get the summary generated
 * @return Generated summary
 */
Summary SummaryGenerator::getSummary()
{
    return summary;
}
