// Date.h
// MS: 5/11/21 - initial code

#ifndef DATE_H
#define DATE_H

#include <time.h>
#include <stdlib.h>

class Date
{
public:
    static int DayOfWeek(int *date);
    static int * ShiftDate(int *date, int days);

private:
    Date() { }
    //static int DaysInMonth(int month);
};

#endif