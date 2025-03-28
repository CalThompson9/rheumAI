/**
 * @name transcript.cpp
 * @brief Definition of Transcript class
 *
 * Stores the textual transcript from a patient-doctor interaction (transcribed audio recording).
 * 
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 9, 2025
 */

#include "transcript.h"

/**
 * @name Transcript (constructor)
 * @brief Initializes a new Transcript instance
 * @details This constructor initializes a new Transcript instance with the given timestamp and content.
 * It sets the timestamp and content attributes of the Transcript object.
 * @param[in] timestamp: Transcript start timestamp
 * @param[in] content: Transcript content
 * @author Joelene Hales
 */
Transcript::Transcript(const QTime &timestamp, const QString &content) : timestamp(timestamp), content(content)
{
    // No logic body
}

/**
 * @name getTimestamp
 * @brief Get a read-only version of the transcript timestamp
 * @details This function returns a read-only version of the transcript timestamp.
 * It allows access to the timestamp without modifying it.
 * @return Transcript timestamp
 * @author Joelene Hales
 */
const QTime &Transcript::getTimestamp() const
{
    return timestamp;
}

/**
 * @name getContent
 * @brief Get a read-only version of the transcript content
 * @details This function returns a read-only version of the transcript content.
 * It allows access to the content without modifying it.
 * @return Transcript content
 * @author Joelene Hales
 */
const QString &Transcript::getContent() const
{
    return content;
}
