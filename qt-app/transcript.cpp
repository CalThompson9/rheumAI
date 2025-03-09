#include "transcript.h"

Transcript::Transcript(time_t timestamp, std::string content)
    : timestamp(timestamp), content(content) {}

time_t Transcript::getTimestamp() const
{
    return timestamp;
}

std::string Transcript::getContent() const
{
    return content;
}

std::string Transcript::toJSON() const
{
    return "{\"timestamp\": " + std::to_string(timestamp) +
           ", \"content\": \"" + content + "\"}";
}
