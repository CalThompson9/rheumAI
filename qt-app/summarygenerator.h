/**
 * @file summarygenerator.h
 * @brief Declaration of SummaryGenerator builder class
 * 
 * @author Joelene Hales
 * @date Mar. 9, 2025
 * 
 * @author Callum Thompson
 * @date Mar. 12, 2025
 */

 #ifndef SUMMARYGENERATOR_H
 #define SUMMARYGENERATOR_H
 
 #include <QObject>
 #include "transcript.h"
 #include "summary.h"
 #include "llmclient.h"
 
 class SummaryGenerator : public QObject
 {
     Q_OBJECT
 
 public:
     explicit SummaryGenerator(Transcript& transcript, QObject *parent = nullptr);
     virtual ~SummaryGenerator() = default;
 
     Summary getSummary();
 
 private:
     Transcript& transcript;
     LLMClient *llmClient;
     Summary summary;
 
     QString extractSectionFromResponse(const QString &response, const QString &sectionName);
 
 private slots:
     void handleLLMResponse(const QString &response);
 
 signals:
     void summaryReady();
 };
 
 #endif // SUMMARYGENERATOR_H
 