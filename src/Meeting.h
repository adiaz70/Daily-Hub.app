// Meeting.h
// MS: 3/28/21 - initial code
// MS: 4/2/21 - added Contact object

#ifndef MEETING_H
#define MEETING_H

#include <string>
#include "Contact.h"

class Meeting
{
public:
    Meeting(std::string _name, std::string _link, Contact *_contact = nullptr)
    {
        name = _name;
        link = _link;
        contact = _contact;
    }

    std::string GetName() { return name; }
    std::string GetLink() { return link; }
    Contact * GetContact() { return contact; }

private:
    std::string name;
    std::string link;
    Contact *contact;
    // Date and time is probably some sort of object
};

#endif