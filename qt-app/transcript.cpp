/**
 * @name transcript.cpp
 * @brief Definition of Transcript class
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#include "transcript.h"

Transcript::Transcript(const QTime& timestamp, const QString& content) : timestamp(timestamp), content(content)
{
    // No logic body
}


/**
 * @name getTimestamp
 * @brief Get a read-only version of the transcript timestamp
 * @return Transcript timestamp
 */
const QTime& Transcript::getTimestamp() const
{
    return timestamp;
}


/**
 * @name getContent
 * @brief Get a read-only version of the transcript content
 * @return Transcript content
 */
const QString& Transcript::getContent() const
{
    return content;
}