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
 #include <QRegularExpression>
 #include <QRegularExpressionMatch>
 #include "transcript.h"
 #include "summary.h"
 #include "llmclient.h"
 
 class SummaryGenerator : public QObject
 {
     Q_OBJECT
 
 public:
    explicit SummaryGenerator(QObject *parent = nullptr);
    virtual ~SummaryGenerator() = default;
    void sendRequest(Transcript& prompt);
 
     Summary getSummary();
 
 private:
     LLMClient *llmClient;
     Summary summary;
 
     QString extractSectionFromResponse(const QString &response, const QString &sectionName);

     void summarizeIntervalHistory(const QString &response);
     void summarizePhysicalExamination(const QString &response);
     void summarizeCurrentStatus(const QString &response);
     void summarizePlan(const QString &response);
     
 
 private slots:
     void handleLLMResponse(const QString &response);
 
 signals:
     void summaryReady();
 };
 
 #endif // SUMMARYGENERATOR_H
