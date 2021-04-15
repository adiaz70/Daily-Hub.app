// UserData.h
// MS: 4/15/21 - initial code

#ifndef USER_DATA_H
#define USER_DATA_H

#include <sqlite3.h>
#include "Contact.h"
#include "Meeting.h"

class UserData
{
public:
    // Getters
    static void GetMeetings(bool print = false);
    static void GetMeetings(Contact *contact, bool print = false);
    static void GetContacts(bool print = false);

    // Setters
    static void AddMeeting(Meeting *meeting, sqlite3 *database = nullptr);
    static void AddMeeting(Meeting **meetings, int num);

    // Database stuff
    static void CreateDatabase(bool populate = false);
    static void ResetDatabase(bool populate = false);

private:
    UserData(){}
    static int Callback(void *data, int argc, char **argv, char **azColName);
    static void CreateDatabase(sqlite3 *database);
    static void OpenDatabase(sqlite3 **database);
};

#endif