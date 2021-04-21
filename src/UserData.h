// UserData.h
// MS: 4/15/21 - initial code
// MS: 4/18/21 - can now build objects after querying the database, and contacts are handled as strings rather than objects
// MS: 4/21/21 - added support for saving and reading notes

#ifndef USER_DATA_H
#define USER_DATA_H

#include <sqlite3.h>
#include <string.h>
#include <vector>
#include "Meeting.h"

class UserData
{
public:
    // Getters
    static std::vector<Meeting*> GetMeetings(bool print = false);
    static std::vector<Meeting*> GetMeetings(std::string contact, bool print = false);
    static std::vector<std::string> GetContacts(bool print = false);
    static std::string GetNotes(int meetingID);

    // Setters
    static void AddMeeting(Meeting *meeting, sqlite3 *database = nullptr);
    static void AddMeeting(Meeting **meetings, int num);
    static void AddContact(std::string contact);
    static void SaveNotes(int meetingID, std::string notes);

    // Database stuff
    static void CreateDatabase(bool populate = false);
    static void ResetDatabase(bool populate = false);

private:
    UserData(){}
    static int Callback(void *data, int argc, char **argv, char **colName);
    static int MeetingCallback(void *data, int argc, char **argv, char **colName);
    static int ContactCallback(void *data, int argc, char **argv, char **colName);
    static int IDCallback(void *data, int argc, char **argv, char **colName);
    static int NotesCallback(void *data, int argc, char **argv, char **colName);
    static void CreateDatabase(sqlite3 *database);
    static void OpenDatabase(sqlite3 **database, std::string name = "user_data.db");
};

#endif