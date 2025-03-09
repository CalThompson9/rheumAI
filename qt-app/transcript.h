#ifndef TRANSCRIPT_H
#define TRANSCRIPT_H

#include <string>
#include <ctime>

class Transcript
{
private:
    time_t timestamp;
    std::string content;

public:
    Transcript(time_t timestamp, std::string content);

    time_t getTimestamp() const;
    std::string getContent() const;
    std::string toJSON() const;
};

#endif // TRANSCRIPT_H
