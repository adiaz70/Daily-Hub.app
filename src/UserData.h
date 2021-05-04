// UserData.h
// MS: 4/15/21 - initial code
// MS: 4/18/21 - can now build objects after querying the database, and contacts are handled as strings rather than objects
// MS: 4/21/21 - added support for saving and reading notes
// MS: 4/21/21 - added function to sanitize user input
// MS: 4/23/21 - rearranged a couple of functions for greater privacy and efficiency
// MS: 4/25/21 - added a couple of functions to sort meetings according to date
// MS: 5/4/21 - abstracted formatting a string for the date into a new function and added function to update a meeting

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
    static std::vector<Meeting *> GetMeetings(bool print = false);
    static std::vector<Meeting *> GetMeetings(std::string contact, bool print = false);
    static std::vector<Meeting *> GetMeetings(int date[3], bool print = false);
    static std::vector<Meeting *> GetMeetings(int startDate[3], int endDate[3], bool print = false);
    static std::vector<std::string> GetContacts(bool print = false);
    static std::string GetNotes(int meetingID);

    // Setters
    static void AddMeeting(Meeting *meeting, sqlite3 *database = nullptr);
    static void AddMeeting(Meeting **meetings, int num);
    static void UpdateMeeting(Meeting *meeting);
    static void DeleteMeeting(Meeting *meeting);
    static void AddContact(std::string contact);
    static void SaveNotes(int meetingID, std::string notes);

    // Database stuff
    static void ResetDatabase(bool populate = false);

private:
    UserData(){}
    static int Callback(void *data, int argc, char **argv, char **colName);
    static int MeetingCallback(void *data, int argc, char **argv, char **colName);
    static int ContactCallback(void *data, int argc, char **argv, char **colName);
    static int IDCallback(void *data, int argc, char **argv, char **colName);
    static int NotesCallback(void *data, int argc, char **argv, char **colName);
    static void CreateDatabase(bool populate = false);
    static void OpenDatabase(sqlite3 **database, std::string name = "user_data.db");
    static void SanitizeString(std::string *text, std::string escapeSequence = "\'");
    static std::string FormatDateString(int *date);
    static void PrintMeetingInfo(Meeting *meeting);
};

#endif