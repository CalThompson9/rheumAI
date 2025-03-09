/**
 * @name summarygenerator.cpp
 * @brief Definition of SummaryGenerator builder class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#include <QDebug>
#include "summarygenerator.h"

/**
 * @name SummaryGenerator
 * @brief Constructor for the SummaryGenerator class
 * @details Implements the Builder design pattern to create a Summary object.
 *          Add desired elements to the summary by calling the corresponding method.
 * @param[in] transcript: Transcript to summarize
 *                        TODO: This functionality is currently not implemented.
 *                        Summaries are simply placeholder text
 * @param[in] parent: Parent object in Qt application
 */
SummaryGenerator::SummaryGenerator(Transcript& transcript, QObject *parent)
    : QObject(parent), transcript(transcript)
{
    // No logic body
}


/**
 * @name summarizeSymptoms
 * @brief Summarizes symptoms discussed in the transcript
 * @return Reference to the builder. This allows methods to be chained together.
 */
SummaryGenerator& SummaryGenerator::summarizeSymptoms()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Symptoms summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of symptoms at " << currentTime;
    summary.setSymptoms(placeholderText);

    return *this;
}


/**
 * @name summarizeDiagnoses
 * @brief Summarizes diagnoses discussed in the transcript
 * @return Reference to the builder. This allows methods to be chained together.
 */
SummaryGenerator& SummaryGenerator::summarizeDiagnoses()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Diagnoses summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of diagnoses at " << currentTime;
    summary.setDiagnoses(placeholderText);
    
    return *this;
}


/**
 * @name summarizeMedicalHistory
 * @brief Summarizes medical history discussed in the transcript
 * @return Reference to the builder. This allows methods to be chained together.
 */
SummaryGenerator& SummaryGenerator::summarizeMedicalHistory()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Medical history summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of medical history at " << currentTime;
    summary.setMedicalHistory(placeholderText);

    return *this;
}


/**
 * @name summarizeTreatmentPlans
 * @brief Summarizes treatment plans discussed in the transcript
 * @return Reference to the builder. This allows methods to be chained together.
 */
SummaryGenerator& SummaryGenerator::summarizeTreatmentPlans()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Treatment plans summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of treatment plans at " << currentTime;
    summary.setTreatmentPlans(placeholderText);

    return *this;
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
