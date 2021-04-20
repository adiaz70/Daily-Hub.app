// Meeting.h
// MS: 3/28/21 - initial code
// MS: 4/2/21 - added Contact object
// MS: 4/18/21 - reworked contacts to be strings because the object route wasn't actually helping and added support for date and time
// MS: 4/20/21 - added getters for the rest of the code to be able to access the meeting information

#ifndef MEETING_H
#define MEETING_H

#include <string>
#include "MeetingTime.h"

class Meeting
{
public:
    Meeting(std::string _name, std::string _link, std::string _contact, MeetingTime *_meetingTime, int date[3])
    {
        name = _name;
        link = _link;
        contact = _contact;
        meetingTime = _meetingTime;
        for (int i = 0; i < 3; i++)
            firstDate[i] = date[i];
    }

    // MS: 4/7/21 - added new constructor to accept arguments for a recurring meeting
    Meeting(std::string _name, std::string _link, std::string _contact, MeetingTime *_meetingTime, int _firstDate[3], int _secondDate[3], bool _recurringDays[7])
        : Meeting(_name, _link, _contact, _meetingTime, _firstDate)
    {
        recurring = true;
        for (int i = 0; i < 7; i++)
        {
            if (i < 3)
                secondDate[i] = _secondDate[i];

            recurringDays[i] = _recurringDays[i];
        }
    }

    ~Meeting() { delete(meetingTime); }

    std::string GetName() { return name; }
    std::string GetLink() { return link; }
    std::string GetContact() { return contact; }
    MeetingTime * GetMeetingTime() { return meetingTime; }
    int * GetFirstDate() { return firstDate; }
    int * GetSecondDate()
    {
        if (recurring)
            return secondDate;
        else
            return nullptr;
    }
    bool IsRecurring() { return recurring; }
    bool * GetRecurringDays() { return recurringDays; }

private:
    std::string name;
    std::string link;
    std::string contact;
    bool recurring = false;
    bool recurringDays[7];
    int firstDate[3];
    int secondDate[3];
    MeetingTime *meetingTime;
};

#endif