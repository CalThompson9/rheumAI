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
