// MeetingTime.h
// MS: 4/11/21 - initial code

#ifndef MEETING_TIME_H
#define MEETING_TIME_H

class MeetingTime
{
public:
    MeetingTime(int _times[4], bool _isStartAM, bool _isEndAM);
    bool IsValidTime(); // checks whether the start and stop times are logically possible

private:
    int AM_PMto24(int hour, bool AM);

    int times[4];
    bool isStartAM;
    bool isEndAM;
};

#endif