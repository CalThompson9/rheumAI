/**
 * @file summary.h
 * @brief Declaration of Summary class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 6, 2025
 * 
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 12, 2025
 */

 #ifndef SUMMARY_H
 #define SUMMARY_H
 
 #include <QString>
 
 class Summary
 {
     public:
         Summary();
         Summary(const Summary& other);
         ~Summary() = default;
 
         Summary& operator=(const Summary& original);
         
         const QString& getSymptoms() const;
         const QString& getDiagnoses() const;
         const QString& getMedicalHistory() const;
         const QString& getTreatmentPlans() const;
         const QString& getPhysicalExamination() const;
 
         void setSymptoms(const QString& symptoms);
         void setDiagnoses(const QString& diagnoses);
         void setMedicalHistory(const QString& medicalHistory);
         void setTreatmentPlans(const QString& treatmentPlans);
         void setPhysicalExamination(const QString& physicalExamination);
         
         void clear();
 
     private:
         QString symptoms;
         QString diagnoses;
         QString medicalHistory;
         QString treatmentPlans;
         QString physicalExamination;
 };
 
 #endif // SUMMARY_H
 
