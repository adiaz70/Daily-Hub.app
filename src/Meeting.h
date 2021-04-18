// Meeting.h
// MS: 3/28/21 - initial code
// MS: 4/2/21 - added Contact object
// MS: 4/18/21 - reworked contacts to be strings because the object route wasn't actually helping

#ifndef MEETING_H
#define MEETING_H

#include <string>

class Meeting
{
public:
    Meeting(std::string _name, std::string _link, std::string _contact)
    {
        name = _name;
        link = _link;
        contact = _contact;
    }

    // MS: 4/7/21 - added new constructor to accept arguments for a recurring meeting
    Meeting(std::string _name, std::string _link, std::string _contact, bool _recurringDays[7])
        : Meeting(_name, _link, _contact)
    {
        recurring = true;
        for (int i = 0; i < 7; i++)
        {
            recurringDays[i] = _recurringDays[i];
        }
    }

    std::string GetName() { return name; }
    std::string GetLink() { return link; }
    std::string GetContact() { return contact; }

private:
    std::string name;
    std::string link;
    std::string contact;
    bool recurring = false;
    bool recurringDays[7];
    // Date and time is probably some sort of object
};

#endif