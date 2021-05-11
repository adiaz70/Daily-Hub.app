// Date.cpp
// MS: 5/11/21 - initial code

#include "Date.h"

//**************************
// Public member functions *
//**************************

// Expects 'date' to have the values for the date in the format MM/DD/YY
// Returns values from 0-6, where 0 = Monday and 6 = Sunday (I know, I know...)
int Date::DayOfWeek(int *date)
{
    // Initialize a 'tm' struct with the provided date, adjusting the month
    // to be zero-based indexed, and the year to be in terms of time since 1900
    tm _time { 0, 0, 0, date[1], date[0] - 1, date[2] - 1900 };
    mktime(&_time);
    // Get the day of the week, from 0-6, starting with Sunday
    int weekday = _time.tm_wday;
    // Convert the value so that it starts with Monday instead (sorry!) and return it
    weekday--;
    if (weekday == -1)
        weekday = 6;
    return weekday;
}

// Credit for this brilliantly simple solution to shift the date without getting bogged down in calendar eccentricities:
// https://stackoverflow.com/questions/2344330/algorithm-to-add-or-subtract-days-from-a-date
int * Date::ShiftDate(int *date, int days)
{
    // First, initialize a 'tm' struct with the current date.
    // Note that the month is expected to use zero-based indexing, which it currently does not, and that the year
    // is expected as an interval since 1900, and it's currently the interval since 2000
    tm _time { 0, 0, 0, date[1], date[0] - 1, date[2] + 100 };
    // Then create a new time from the current time and the length that we need to shift forward (the seconds in a day * number of days)
    time_t newTime = mktime(&_time) + (24 * 60 * 60 * days);
    // And update the date with the new time
    _time = *localtime(&newTime);

    // Now that we have the shifted date, allocate space for the return values
    int *newDate = (int *) malloc(3 * sizeof(int));
    // And load in our results
    newDate[0] = _time.tm_mon + 1; // reverse previous adjustment
    newDate[1] = _time.tm_mday;
    newDate[2] = _time.tm_year - 100; // reverse previous adjustment

    return newDate;
}

//***************************
// Private member functions *
//***************************