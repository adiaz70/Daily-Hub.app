// UserData.h
// MS: 4/15/21 - initial code
// MS: 4/18/21 - can now build objects after querying the database, and contacts are handled as strings rather than objects
// MS: 4/20/21 - the database now accepts lots more information on meetings so that they can be fully reconstructed
// MS: 4/21/21 - changed MEETINGS table primary key so that there can be matching names but unique IDs
// MS: 4/21/21 - added NOTES table to database from which user-written notes can be read and saved to
// MS: 4/21/21 - all user-generated strings are now sanitized before being executed in SQLite
// MS: 4/23/21 - rearranged a couple of functions for greater privacy and efficiency
// MS: 4/25/21 - added a couple of functions to sort meetings according to date
// MS: 5/1/21 - changed SQL commands to ignore case when sorting alphabetically
// MS: 5/4/21 - abstracted formatting a string for the date into a new function
// MS: 5/4/21 - added static variable to track the last time that the database was accessed

#include "UserData.h"
#include "Settings.h"
#include "MeetingTime.h"
#include <string>
#include <stdio.h>
#include <iostream>

//***************************
// Public member functions. *
//***************************

//**********
// Getters *
//**********

std::vector<Meeting*> UserData::GetMeetings(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);

    std::vector<Meeting *> meetings;
    sqlite3_exec(database, "SELECT * FROM MEETINGS ORDER BY Name COLLATE NOCASE;", MeetingCallback, (void*) &meetings, NULL);

    if (print)
    {
        std::cout << "Found " << meetings.size() << " meetings.\n\n";
        for (int i = 0; i < meetings.size(); i++)
            PrintMeetingInfo(meetings[i]);
    }

    sqlite3_close(database);
    return meetings;
}

std::vector<Meeting*> UserData::GetMeetings(std::string contact, bool print)
{
    SanitizeString(&contact);

    sqlite3 *database;
    OpenDatabase(&database);
    
    std::string sql = "SELECT * FROM MEETINGS WHERE Contact = " + contact + " ORDER BY Name COLLATE NOCASE;";
    std::vector<Meeting *> meetings;

    sqlite3_exec(database, sql.c_str(), MeetingCallback, (void*) &meetings, NULL);

    if (print)
    {
        std::cout << "Found " << meetings.size() << " meetings.\n\n";
        for (int i = 0; i < meetings.size(); i++)
            PrintMeetingInfo(meetings[i]);
    }

    sqlite3_close(database);
    return meetings;
}

// Expects dates in the format MM/DD/YY.
// Also, any value that is -1 is ignored when filtering results.
// For example, 12,-1,21 would get all meetings in December 2021.
std::vector<Meeting *> UserData::GetMeetings(int date[3], bool print)
{
    //************************
    // Not thoroughly tested *
    //************************
    std::cout << "Warning: UserData::GetMeetings(int[3], bool) has not been thoroughly tested for accuracy.\n";

    // If there is no date provided at all, return empty vector with a warning
    if (date[0] == -1 && date[1] == -1 && date[2] == -1)
    {
        std::cout << "No date provided to UserData::GetMeetings(int[3], bool)\nReturning empty vector\n";
        std::vector<Meeting *> empty;
        return empty;
    }

    sqlite3 *database;
    OpenDatabase(&database);

    // Start the SQL command
    std::string sql = "SELECT * FROM MEETINGS WHERE ";
    // Then add only the relevant elements of a date to the command to narrow down the results
    if (date[0] != -1)
    {
        std::string date;
        if (date[0] < 10)
            date = "0";
        date += std::to_string(date[0]);

        sql += "strftime('%m', FirstDate) = '" + date + "'";

        if (date[1] != -1 || date[2] != -1)
            sql += ",";

        sql += " ";
    }
    if (date[1] != -1)
    {
        std::string date;
        if (date[1] < 10)
            date = "0";
        date += std::to_string(date[1]);

        sql += "strftime('%d', FirstDate) = '" + date + "'";

        if (date[2] != -1)
            sql += ",";

        sql += " ";
    }
    if (date[2] != -1)
        sql += "strftime('%Y', FirstDate) = '20" + std::to_string(date[2]) + "' ";
    // And end the command by specifying how to sort the results
    sql += "ORDER BY IsStartAM, StartHour, StartMinute;";
    
    std::vector<Meeting *> meetings;
    sqlite3_exec(database, sql.c_str(), MeetingCallback, (void*) &meetings, NULL);

    if (print)
    {
        std::cout << "Found " << meetings.size() << " meetings.\n\n";
        for (int i = 0; i < meetings.size(); i++)
            PrintMeetingInfo(meetings[i]);
    }

    sqlite3_close(database);
    return meetings;
}

// Expects dates in the format MM/DD/YY.
// Finds and returns all meetings that occur in between startDate and endDate
std::vector<Meeting *> UserData::GetMeetings(int startDate[3], int endDate[3], bool print)
{
    //************************
    // Not thoroughly tested *
    //************************
    std::cout << "Warning: UserData::GetMeetings(int[3], int[3], bool) has not been thoroughly tested for accuracy.\n";

    sqlite3 *database;
    OpenDatabase(&database);

    std::string sql = "SELECT * FROM MEETINGS WHERE ";

    // Format the provided dates the way that SQLite wants them
    std::string searchDates[2];
    searchDates[0] = "20" + std::to_string(startDate[3]) + "-" + std::to_string(startDate[0]) + "-" + std::to_string(startDate[1]);
    searchDates[0] = "20" + std::to_string(endDate[3]) + "-" + std::to_string(endDate[0]) + "-" + std::to_string(endDate[1]);

    // If the meeting does not recur, make sure it's scheduled within the date range
    sql += " (IsRecurring = 0 AND FirstDate BETWEEN " + searchDates[0] + " AND " + searchDates[1] + ") ";
    // If the meeting does recur, make sure that it's scheduled to start or end within the search range, or that
    // the search range starts between it's scheduled range
    sql += "OR (IsRecurring = 1 AND ((FirstDate BETWEEN " + searchDates[0] + " AND " + searchDates[1] + ") OR " +
           "(SecondDate BETWEEN " + searchDates[0] + " AND " + searchDates[1] + ") OR " +
           "(" + searchDates[0] + " BETWEEN FirstDate AND SecondDate))) ORDER BY FirstDate;";

    std::vector<Meeting *> meetings;
    sqlite3_exec(database, sql.c_str(), MeetingCallback, (void*) &meetings, NULL);

    if (print)
    {
        std::cout << "Found " << meetings.size() << " meetings.\n\n";
        for (int i = 0; i < meetings.size(); i++)
            PrintMeetingInfo(meetings[i]);
    }

    sqlite3_close(database);
    return meetings;
}

std::vector<std::string> UserData::GetContacts(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);

    std::vector<std::string> contacts;
    sqlite3_exec(database, "SELECT * FROM CONTACTS ORDER BY Name COLLATE NOCASE;", ContactCallback, (void*) &contacts, NULL);

    // Check if the first element in the vector is blank because a meeting exists with no assigned contact.
    // If so, remove it
    if (contacts.size() > 0 && contacts[0] == "")
        contacts.erase(contacts.begin());

    if (print)
    {
        std::cout << "Found " << contacts.size() << " contacts.\n\n";
        for (int i = 0; i < contacts.size(); i++)
            std::cout << "Contact Name: " << contacts[i] << "\n\n";
    }
    
    sqlite3_close(database);
    return contacts;
}

std::string UserData::GetNotes(int meetingID)
{
    sqlite3 *database;
    OpenDatabase(&database);

    std::string sql = "SELECT ID, Notes FROM NOTES WHERE ID = " + std::to_string(meetingID) + ";";
    std::string notes;
    sqlite3_exec(database, sql.c_str(), NotesCallback, &notes, NULL);

    sqlite3_close(database);

    return notes;
}

time_t UserData::GetLastAccessTime()
{
    return lastAccessTime;
}

//**********
// Setters *
//**********

void UserData::AddMeeting(Meeting *meeting, sqlite3 *database)
{
    bool close = false;
    if (database == nullptr)
    {
        OpenDatabase(&database);
        close = true;
    }
    
    std::string sql = "INSERT INTO MEETINGS (ID, " \
                                            "Name, Link, Contact, " \
                                            "StartHour, StartMinute, IsStartAM, " \
                                            "EndHour, EndMinute, IsEndAM, " \
                                            "IsRecurring, " \
                                            "FirstDate, SecondDate, " \
                                            "Mon, Tue, Wed, Thur, Fri, Sat, Sun) ";

    // Calculate a unique ID to be assigned to this meeting and concatenate it to the SQLite command
    int maxID;
    sqlite3_exec(database, "SELECT MAX(ID) FROM MEETINGS;", IDCallback, (void*) &maxID, NULL);
    sql += "VALUES (" + std::to_string(maxID + 1);

    // Concatenate the meeting name, link, and contact (after sanitizing them JUST TO BE SAFE)
    std::string name = meeting->GetName();
    SanitizeString(&name);
    std::string link = meeting->GetLink();
    SanitizeString(&link);
    std::string contact = meeting->GetContact();
    SanitizeString(&contact);
    sql += ", '" + name + "', '" + link + "', '" + contact + "'";
    
    // Concatenate the meeting's start and end times
    int *times = meeting->GetMeetingTime()->GetTimes();
    sql += ", " + std::to_string(times[0]) + ", " + std::to_string(times[1]) + ", " + std::to_string(meeting->GetMeetingTime()->IsStartAM());
    sql += ", " + std::to_string(times[2]) + ", " + std::to_string(times[3]) + ", " + std::to_string(meeting->GetMeetingTime()->IsEndAM());

    // Concatenate all of the information about whether the meeting recurs between two dates or occurs only once
    sql += ", " + std::to_string(meeting->IsRecurring());
    sql += ", '" + FormatDateString(meeting->GetFirstDate()) + "'";
    if (meeting->IsRecurring())
        sql += ", '" + FormatDateString(meeting->GetSecondDate()) + "'";
    else
        sql += ", 'NULL'";

    // Concatenate the exact days of the week on which the meeting is set to recur
    for (int i = 0; i < 7; i++)
    {
        sql += ", " + std::to_string(meeting->GetRecurringDays()[i]);
    }
    sql += ");";

    // Execute the command to add this meeting to the database
    int result = sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);

    // If the command executed successfully, insert a corresponding string into the NOTES table for later use
    if (result == 0)
    {
        sql = "INSERT INTO NOTES (ID, Notes) VALUES (" + std::to_string(maxID + 1) + ", '');";
        sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    }

    if (close)
        sqlite3_close(database);
}

void UserData::AddMeeting(Meeting **meetings, int num)
{
    sqlite3 *database;
    OpenDatabase(&database);

    for (int i = 0; i < num; i++)
    {
        AddMeeting(meetings[i], database);
    }

    sqlite3_close(database);
}

// MS: 5/4/21 - added function
void UserData::UpdateMeeting(Meeting *meeting)
{
    sqlite3 *database;
    OpenDatabase(&database);
    std::string sql = "UPDATE MEETINGS SET ";

    std::string name = meeting->GetName();
    std::string link = meeting->GetLink();
    std::string contact = meeting->GetContact();
    SanitizeString(&name);
    SanitizeString(&link);
    SanitizeString(&contact);
    sql += "Name = '" + name + "', ";
    sql += "Contact = '" + contact + "', ";
    sql += "Link = '" + link + "', ";

    int *times = meeting->GetMeetingTime()->GetTimes();
    sql += "StartHour = " + std::to_string(times[0]) + ", ";
    sql += "StartMinute = " + std::to_string(times[1]) + ", ";
    sql += "IsStartAM = " + std::to_string(meeting->GetMeetingTime()->IsStartAM()) + ", ";
    sql += "EndHour = " + std::to_string(times[2]) + ", ";
    sql += "EndMinute = " + std::to_string(times[3]) + ", ";
    sql += "IsEndAM = " + std::to_string(meeting->GetMeetingTime()->IsEndAM()) + ", ";

    sql += "IsRecurring = " + std::to_string(meeting->IsRecurring()) + ", ";
    sql += "FirstDate = '" + FormatDateString(meeting->GetFirstDate()) + "', ";
    sql += "SecondDate = '" + (meeting->IsRecurring() ? FormatDateString(meeting->GetSecondDate()) : "NULL") + "', ";

    std::string days[7] = { "Mon", "Tue", "Wed", "Thur", "Fri", "Sat", "Sun" };
    bool *recurringDays = meeting->GetRecurringDays();
    for (int i = 0; i < 7; i++)
    {
        sql += days[i] + " = " + std::to_string(recurringDays[i]);
        
        if (i < 6)
            sql += ", ";
        else
            sql += " ";
    }
    
    sql += "WHERE ID = " + std::to_string(meeting->GetID()) + ";";

    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    sqlite3_close(database);
}

// MS: 5/3/21 - added function
void UserData::DeleteMeeting(Meeting *meeting)
{
    sqlite3 *database;
    OpenDatabase(&database);
    std::string sql = "DELETE FROM MEETINGS WHERE ID = " + std::to_string(meeting->GetID()) + ";";
    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    sql = "DELETE FROM NOTES WHERE ID = " + std::to_string(meeting->GetID()) + ";";
    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    sqlite3_close(database);
}

void UserData::AddContact(std::string contact)
{
    SanitizeString(&contact);

    sqlite3 *database;
    OpenDatabase(&database);
    std::string sql = "INSERT INTO CONTACTS (Name) VALUES ('" + contact + "');";
    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    sqlite3_close(database);
}

void UserData::SaveNotes(int meetingID, std::string notes)
{
    SanitizeString(&notes);

    sqlite3 *database;
    OpenDatabase(&database);
    std::string sql = "UPDATE NOTES SET Notes = '" + notes + "' WHERE ID = " + std::to_string(meetingID) + ";";
    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    sqlite3_close(database);
}

//**********************
// Database operations *
//**********************

// MS: 5/5/21 - added function
// Indicate that the database might have changed in some way at this time so that anything relying on
// up-to-date information will know to refresh.
void UserData::RefreshDatabase()
{
    lastAccessTime = time(0);
}

// MS: 5/14/21 - changed DROP TABLE commands to only execute if the database successfully opened
void UserData::ResetDatabase(bool populate)
{
    sqlite3 *database;
    if (OpenDatabase(&database))
    {
        sqlite3_exec(database, "DROP TABLE MEETINGS;", Callback, NULL, NULL);
        sqlite3_exec(database, "DROP TABLE CONTACTS;", Callback, NULL, NULL);
        sqlite3_exec(database, "DROP TABLE NOTES;", Callback, NULL, NULL);
    }
    sqlite3_close(database);

    CreateDatabase(populate);
}

// MS: 5/14/21 - added function to create database tables if they don't already exist
bool UserData::CheckDatabase()
{
    bool foundEntry = false;

    sqlite3 *database;
    OpenDatabase(&database);
    sqlite3_exec(database, "SELECT name FROM sqlite_master WHERE TYPE = 'table' AND name != 'SETTINGS';", TrueFalseCallback, &foundEntry, NULL);
    sqlite3_close(database);

    // If no tables where found in the database, create them
    if (!foundEntry)
        CreateDatabase();

    return foundEntry;
}

//****************************
// Private member functions. *
//****************************

// Define private static member variable 'lastAccessTime'
time_t UserData::lastAccessTime = time(0);

// References:
// https://www.sqlite.org/c3ref/exec.html
// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
int UserData::Callback(void *data, int argc, char **argv, char **colName)
{
    // If this is supposed to be output to the terminal, then do so
    if (data != NULL && (char*)data == "print")
    {
        for (int i = 0; i < argc; i++)
            printf("%s: %s\n", colName[i], argv[i] ? argv[i] : "NULL");
        printf("\n");
    }

    return 0;
}

// This function expects 'data' to be a pointer to vector of Meeting objects
int UserData::MeetingCallback(void *data, int argc, char **argv, char **colName)
{
    /*  (ID INTEGER PRIMARY KEY," \           0
        "Name TEXT, " \                       1
        "Link TEXT," \                        2
        "Contact TEXT," \                     3
        "StartHour INTEGER," \                4  
        "StartMinute INTEGER," \              5
        "IsStartAM INTEGER," \                6
        "EndHour INTEGER," \                  7
        "EndMinute INTEGER," \                8
        "IsEndAM INTEGER," \                  9
        "IsRecurring INTEGER," \              10
        "FirstDate TEXT," \                   11
        "SecondDate TEXT," \                  12
        "Mon INTEGER," \                      13
        "Tue INTEGER," \                      14
        "Wed INTEGER," \                      15
        "Thur INTEGER," \                     16
        "Fri INTEGER," \                      17
        "Sat INTEGER," \                      18
        "Sun INTEGER)                         19*/

    if (argc < 20)
    {
        std::cout << "Invalid number of arguments in MeetingCallback\n";
        return 0;
    }

    Meeting *meeting;
    int times[4] = { std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[7]), std::stoi(argv[8]) };
    MeetingTime *meetingTime = new MeetingTime(times, std::stoi(argv[6]), std::stoi(argv[9]));
    
    // Expected to be formatted as: "YYYY-MM-DD"
    std::string date = argv[11];
    int firstDate[3] = { std::stoi(date.substr(5, 2)),
                         std::stoi(date.substr(8, 2)),
                         std::stoi(date.substr(2, 2)) };

    // If this is a recurring meeting
    if (std::stoi(argv[10]) == 1)
    {
        if (argv[12] == "NULL")
        {
            std::cout << "Something has gone wrong in MeetingCallback.\nIsRecurring = true but SecondDate = NULL\n\n";
            return 0;
        }

        date = argv[12];
        int secondDate[3] = { std::stoi(date.substr(5, 2)),
                              std::stoi(date.substr(8, 2)),
                              std::stoi(date.substr(2, 2)) };

        bool days[7];
        for (int i = 0; i < 7; i++)
        {
            days[i] = std::stoi(argv[13 + i], nullptr, 0);
        }

        meeting = new Meeting(argv[1], argv[2], argv[3], meetingTime, firstDate, secondDate, days);
    }
    else
    {
        meeting = new Meeting(argv[1], argv[2], argv[3], meetingTime, firstDate);
    }

    // Make sure to assign the database ID to this newly created meeting
    meeting->SetID(std::stoi(argv[0]));

    std::vector<Meeting*> *meetings = (std::vector<Meeting*> *) data;
    meetings->push_back(meeting);

    return 0;
}

// This function expects 'data' to be a pointer to vector of strings
int UserData::ContactCallback(void *data, int argc, char **argv, char **colName)
{
    std::vector<std::string> *contacts = (std::vector<std::string> *) data;
    contacts->push_back(argv[0]);

    return 0;
}

// This function expects 'data' to be a pointer to an integer
int UserData::IDCallback(void *data, int argc, char **argv, char **colName)
{
    int *id = (int *) data;
    *id = argv[0] ? std::stoi(argv[0]) : -1;

    return 0;
}

// This function expects 'data' to be a pointer to a string
int UserData::NotesCallback(void *data, int argc, char **argv, char **colName)
{
    std::string *notes = (std::string *) data;
    *notes = argv[1] ? argv[1] : "";

    return 0;
}

// This function expects 'data' to be a pointer to a bool that will be set to true if this function is called
// (indicates the existence, or lack thereof, of items in the database)
// MS: 5/14/21 - added function
int UserData::TrueFalseCallback(void *data, int argc, char **argv, char **colName)
{
    bool *foundEntry = (bool *) data;
    *foundEntry = true;

    return 0;
}

void UserData::CreateDatabase(bool populate)
{
    sqlite3 *database;
    OpenDatabase(&database);

    sqlite3_exec(database, "CREATE TABLE MEETINGS (ID INTEGER PRIMARY KEY," \
                                                  "Name TEXT," \
                                                  "Link TEXT," \
                                                  "Contact TEXT," \
                                                  "StartHour INTEGER," \
                                                  "StartMinute INTEGER," \
                                                  "IsStartAM INTEGER," \
                                                  "EndHour INTEGER," \
                                                  "EndMinute INTEGER," \
                                                  "IsEndAM INTEGER," \
                                                  "IsRecurring INTEGER," \
                                                  "FirstDate TEXT," \
                                                  "SecondDate TEXT," \
                                                  "Mon INTEGER," \
                                                  "Tue INTEGER," \
                                                  "Wed INTEGER," \
                                                  "Thur INTEGER," \
                                                  "Fri INTEGER," \
                                                  "Sat INTEGER," \
                                                  "Sun INTEGER);", Callback, NULL, NULL);
    
    sqlite3_exec(database, "CREATE TABLE CONTACTS (Name TEXT PRIMARY KEY);", Callback, NULL, NULL);

    sqlite3_exec(database, "CREATE TABLE NOTES (ID INTEGER PRIMARY KEY, Notes TEXT);", Callback, NULL, NULL);

    // Add some test data to the database if desired
    // MS: 5/4/21 - changed sample data to be more realistic/detailed
    if (populate)
    {
        std::string contact[3] = { "CMPS 3350", "Daily Hub Team", "The Universe" };
        AddContact(contact[0]);
        AddContact(contact[1]);
        AddContact(contact[2]);

        Meeting **meetings = (Meeting **) malloc(5 * sizeof(Meeting*));
        // Create meetings for the lecture and lab times for the class this project is for
        int lectureTimes[4] = { 10, 0, 10, 50 };
        int labTimes[4] = { 10, 0, 12, 30 };
        int classStartDate[3] = { 1, 25, 21 };
        int classEndDate[3] = { 5, 14, 21 };
        bool lectureDays[7] = { 1, 0, 1, 0, 1, 0, 0 };
        bool labDays[7] = { 0, 1, 0, 0, 0, 0, 0 };
        meetings[0] = new Meeting("Lecture", "https://csub.zoom.us/j/88490634128?pwd=V0t4U0FVdVBtb25DRWRkNTg2ZGF3UT09", contact[0], new MeetingTime(lectureTimes, true, true), classStartDate, classEndDate, lectureDays);
        meetings[1] = new Meeting("Lab", "https://csub.zoom.us/j/88490634128?pwd=V0t4U0FVdVBtb25DRWRkNTg2ZGF3UT09", contact[0], new MeetingTime(labTimes, true, false), classStartDate, classEndDate, labDays);
        // Create a meeting for when I *think* we're going to be presenting this project
        int finalDate[3] = { 5, 18, 21 };
        meetings[2] = new Meeting("Project Demo", "https://csub.zoom.us/j/88490634128?pwd=V0t4U0FVdVBtb25DRWRkNTg2ZGF3UT09", contact[0], new MeetingTime(labTimes, true, false), finalDate);
        // I just think this one is funny
        int destinyDate[3] = { 12, 31, 99 };
        int destinyTimes[4] = { 12, 58, 12, 59 };
        meetings[3] = new Meeting("Your Destiny", "https://docs.wxwidgets.org/3.0/", contact[2], new MeetingTime(destinyTimes, false, false), destinyDate);
        // And finally, I want to have a sample meeting with our team (although the specific time that I've put down here may not ever happen)
        int workDate[3] = { 5, 16, 21 };
        int workTime[4] = { 4, 0, 6, 0 };
        meetings[4] = new Meeting("Final Work Session", "https://discord.com", contact[1], new MeetingTime(workTime, false, false), workDate);
        AddMeeting(meetings, 5);
        delete(meetings[0]);
        delete(meetings[1]);
        delete(meetings[2]);
        delete(meetings[3]);
        free(meetings);
    }

    sqlite3_close(database);
}

// This is abstracted away so that any error-handling or specifics of finding the right filepath to the database
// doesn't need to be repeated.
// MS: 5/14/21 - changed function to return a boolean value indicating whether the database opened without error
bool UserData::OpenDatabase(sqlite3 **database)
{
    lastAccessTime = time(0);

    std::string path = Settings::GetDatabasePath() + "user_data.db";

    if (sqlite3_open(path.c_str(), database) != 0)
    {
        printf("An error occurred opening the database: %s\n", sqlite3_errmsg(*database));
        return false;
    }

    return true;
}

void UserData::SanitizeString(std::string *text, std::string escapeSequence)
{
    for (int i = 0; i < text->length(); i++)
    {
        if ((*text)[i] == '\'')
        {
            text->insert((size_t) i, escapeSequence);
            // Make sure to bump the index ahead so that it doesn't keep counting the same apostrophe forever
            i++;
        }
    }
}

// Convert the date from three integers (MM/DD/YY) into a string (YYYY/MM/DD) for the database.
// Also, making sure that there are no single-digit entries (e.g. 4 = 04, expected when read back from the database later).
std::string UserData::FormatDateString(int *date)
{
    std::string dateString = "20" + std::to_string(date[2]) + "-";
    if (date[0] < 10)
        dateString += "0";
    dateString += std::to_string(date[0]) + "-";
    if (date[1] < 10)
        dateString += "0";
    dateString += std::to_string(date[1]);

    return dateString;
}

void UserData::PrintMeetingInfo(Meeting *meeting)
{
    std::cout << "Name: " << meeting->GetName() << "\n";
    std::cout << "Link: " << meeting->GetLink() << "\n";
    std::cout << "Contact: " << meeting->GetContact() << "\n";
    if (meeting->IsRecurring())
    {
        std::cout << std::string("This meeting recurs between ") << meeting->GetFirstDate()[0] << "/" <<
                    meeting->GetFirstDate()[1] << "/" << meeting->GetFirstDate()[2] <<
                    " and " << meeting->GetSecondDate()[0] << "/" << meeting->GetSecondDate()[1] <<
                    "/" << meeting->GetSecondDate()[2] << "\n";
    }
    else
    {
        std::cout << "This meeting takes place on " << meeting->GetFirstDate()[0] << "/" <<
                        meeting->GetFirstDate()[1] << "/" << meeting->GetFirstDate()[2] << "\n";
    }
    int *times = meeting->GetMeetingTime()->GetTimes();
    std::cout << "It is scheduled to start at " << times[0] << ":" << times[1] << " " << (meeting->GetMeetingTime()->IsStartAM() ? "AM" : "PM") <<
                 " and finish at " << times[2] << ":" << times[3] << " " << (meeting->GetMeetingTime()->IsEndAM() ? "AM" : "PM") << "\n\n";
}