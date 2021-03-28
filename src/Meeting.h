// Meeting.h
// MS: 3/28/21 - initial code

#ifndef MEETING_H
#define MEETING_H

#include <string>

class Meeting
{
public:
    Meeting(std::string _name, std::string _link)
    {
        name = _name;
        link = _link;
    }

    std::string GetName() { return name; }
    std::string GetLink() { return link; }

private:
    std::string name;
    std::string link;
    // Are contacts just a string name that needs an exact match, or a pointer to an object?
    // Date and time is probably some sort of object
};

#endif