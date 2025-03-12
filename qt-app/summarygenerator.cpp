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
 SummaryGenerator::SummaryGenerator(Transcript& transcript, QObject *parent)
     : QObject(parent), transcript(transcript)
 {
     // Create an LLMClient object
     llmClient = new LLMClient(this);
 
     // Connect the LLMClient signal to the handler
     connect(llmClient, &LLMClient::responseReceived, this, &SummaryGenerator::handleLLMResponse);
 
     // Send a request to the LLMClient 
     llmClient->sendRequest(transcript.getContent());
 }
 
 /**
  * @name handleLLMResponse
  * @brief Handles the LLM response and updates Summary object
  * @param[in] response: Full LLM-generated summary
  */
 void SummaryGenerator::handleLLMResponse(const QString &response)
 {
     summary.setSymptoms(
         extractSectionFromResponse(response, "GENERAL") + "\n" +
         extractSectionFromResponse(response, "ARTHRITIS") + "\n" +
         extractSectionFromResponse(response, "CARDIAC") + "\n" +
         extractSectionFromResponse(response, "RESPIRATORY") + "\n" +
         extractSectionFromResponse(response, "OCULAR") + "\n" +
         extractSectionFromResponse(response, "GASTROINTESTINAL") + "\n" +
         extractSectionFromResponse(response, "GLANDULAR ENLARGEMENT") + "\n" +
         extractSectionFromResponse(response, "SKIN") + "\n" +
         extractSectionFromResponse(response, "CNS") + "\n" +
         extractSectionFromResponse(response, "CANCER")
     );
 
     summary.setMedicalHistory(
         extractSectionFromResponse(response, "MEDICATIONS") + "\n" +
         extractSectionFromResponse(response, "BLOODWORK") + "\n" +
         extractSectionFromResponse(response, "SEROLOGY") + "\n" +
         extractSectionFromResponse(response, "IMAGING") + "\n" +
         extractSectionFromResponse(response, "HEALTH CHANGES") + "\n" +
         extractSectionFromResponse(response, "LIFE CHANGES") + "\n" +
         extractSectionFromResponse(response, "BONE HEALTH")
     );
 
     summary.setDiagnoses(extractSectionFromResponse(response, "CLINICAL IMPRESSION"));
 
     summary.setTreatmentPlans(
         extractSectionFromResponse(response, "PLAN") + "\n" +
         extractSectionFromResponse(response, "Patient Education") + "\n" +
         extractSectionFromResponse(response, "Follow-up")
     );
 
     // Extract and store physical examination separately
     QString physicalExam =
         extractSectionFromResponse(response, "VITAL SIGNS") + "\n" +
         extractSectionFromResponse(response, "HEAD & NECK") + "\n" +
         extractSectionFromResponse(response, "SKIN") + "\n" +
         extractSectionFromResponse(response, "CARDIAC") + "\n" +
         extractSectionFromResponse(response, "RESPIRATORY") + "\n" +
         extractSectionFromResponse(response, "ABDOMEN") + "\n" +
         extractSectionFromResponse(response, "NEUROLOGIC") + "\n" +
         extractSectionFromResponse(response, "JOINT EXAM");
 
     summary.setPhysicalExamination(physicalExam);
 
     // Extract work/living situation details
     QString livingSituation =
         extractSectionFromResponse(response, "LIVING/FAMILY SITUATION") + "\n" +
         extractSectionFromResponse(response, "WORK/EDUCATION");
 
     if (!livingSituation.trimmed().isEmpty()) {
         summary.setMedicalHistory(summary.getMedicalHistory() + "\n\n**SOCIAL HISTORY:**\n" + livingSituation);
     }
     emit summaryReady();
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
     // Ensure correct search pattern format
     QString searchPattern = "**" + sectionName + ":**";
     int startIndex = response.indexOf(searchPattern);
 
     if (startIndex == -1) {
         qWarning() << "🔍 Section not found in LLM response: " << sectionName;
         return "No " + sectionName.toLower() + " found.";
     }
 
     // Locate the next major section header (starting with "**")
     int endIndex = response.indexOf("\n**", startIndex + searchPattern.length());
     if (endIndex == -1) endIndex = response.length(); // If last section
 
     // Extract the section content
     QString extractedText = response.mid(startIndex + searchPattern.length(), endIndex - startIndex - searchPattern.length()).trimmed();
 
     // ✅ Remove placeholders and handle empty sections
     if (extractedText.isEmpty() || extractedText.contains("(No details provided in the prompt)")) {
         return "No " + sectionName.toLower() + " found.";
     }

     return extractedText;
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
 