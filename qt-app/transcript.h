/**
 * @name transcript.h
 * @brief Declaration of Transcript class
 *
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#ifndef TRANSCRIPT_H
#define TRANSCRIPT_H

#include <QTime>
#include <QString>

/**
 * @class Transcript
 * @brief Class to represent a transcript of a patient-doctor interaction
 * @details This class stores the timestamp and content of a transcript.
 * It provides methods to access the timestamp and content.
 * The class is designed to be used in a Qt application and inherits from QObject.
 * It is used to manage the transcript data and provide a structured representation
 * of the transcript.
 * @author Joelene Hales
 */
class Transcript
{

public:
    Transcript(const QTime &timestamp, const QString &content);
    ~Transcript() = default; // Qt automatically manages memory of QObjects, no need for manual deletion

    const QTime &getTimestamp() const;
    const QString &getContent() const;

private:
    QTime timestamp;
    QString content;
};

#endif // TRANSCRIPT_H
