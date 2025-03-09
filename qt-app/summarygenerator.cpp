/**
 * @name summarygenerator.cpp
 * @brief Definition of SummaryGenerator builder class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#include <QDebug>
#include "summarygenerator.h"

SummaryGenerator::SummaryGenerator(Transcript& transcript, QObject *parent)
    : QObject(parent), transcript(transcript)
{
    // No logic body
}

SummaryGenerator& SummaryGenerator::summarizeSymptoms()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Symptoms summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of symptoms at " << currentTime;
    summary.setSymptoms(placeholderText);

    return *this;
}

SummaryGenerator& SummaryGenerator::summarizeDiagnoses()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Diagnoses summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of diagnoses at " << currentTime;
    summary.setDiagnoses(placeholderText);
    
    return *this;
}

SummaryGenerator& SummaryGenerator::summarizeMedicalHistory()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Medical history summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of medical history at " << currentTime;
    summary.setMedicalHistory(placeholderText);

    return *this;
}

SummaryGenerator& SummaryGenerator::summarizeTreatmentPlans()
{
    // TODO: Implement LLM summarization and replace this placeholder string
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    QString placeholderText = "Treatment plans summary... (Generated at: " + currentTime + ")";
    
    qDebug() << "Generated summary of treatment plans at " << currentTime;
    summary.setTreatmentPlans(placeholderText);

    return *this;
}

Summary SummaryGenerator::getSummary()
{
    return summary;
}

SummaryGenerator::~SummaryGenerator()
{
    // Qt automatically manages memory of QObjects, no need for manual deletion
}
