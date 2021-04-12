// MeetingTime.cpp
// MS: 4/11/21 - initial code

#include "MeetingTime.h"

MeetingTime::MeetingTime(int _times[4], bool _isStartAM, bool _isEndAM)
{
    for (int i = 0; i < 4; i++)
    {
        times[i] = _times[i];
    }

    isStartAM = _isStartAM;
    isEndAM = _isEndAM;
}

bool MeetingTime::IsValidTime()
{
    // Confirm that none of the entries contain numbers larger than should be possible
    if (times[0] <= 12 && times[1] <= 59 && times[2] <= 12 && times[3] <= 59)
    {
        int hours[2] = { AM_PMto24(times[0], isStartAM), AM_PMto24(times[2], isEndAM) };

        // Check that the start time is before the end time
        if ((isStartAM == isEndAM && (hours[0] < hours[1] || (hours[0] == hours[1] && times[1] < times[3])))
            || (isStartAM && !isEndAM))
        {
            return true;
        }
    }

    return false;
}

// Here's the really fun thing about AM/PM times. 12 AM is earlier than 1 AM. Like... did they do this to programmers on purpose?
// So the modulus operator comes to the rescue. (hour % 12) gives us a range of possible results from 0 to 11 where 0 is the
// outcome of hour = 12, which is what we want because it puts it first. Then, add 1 to the result so that we get 1-12.
// If the hour is a PM time, add an additional 12 to it so it's in the range 13-24.
int MeetingTime::AM_PMto24(int hour, bool AM)
{
    if (AM)
        return (hour % 12) + 1;
    else
        return (hour % 12) + 13;
}