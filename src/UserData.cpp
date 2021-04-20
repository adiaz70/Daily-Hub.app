// UserData.h
// MS: 4/15/21 - initial code
// MS: 4/18/21 - can now build objects after querying the database, and contacts are handled as strings rather than objects
// MS: 4/20/21 - the database now accepts lots more information on meetings so that they can be fully reconstructed

#include "UserData.h"
#include "MeetingTime.h"
#include <string>
#include <stdio.h>
#include <iostream>

//***************************
// Public member functions. *
//***************************

std::vector<Meeting*> UserData::GetMeetings(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);

    std::vector<Meeting*> meetings;
    sqlite3_exec(database, "SELECT * FROM MEETINGS ORDER BY Name", MeetingCallback, (void*) &meetings, NULL);

    if (print)
    {
        std::cout << "Found " << meetings.size() << " meetings.\n\n";
        for (int i = 0; i < meetings.size(); i++)
        {
            std::cout << "Name: " << meetings[i]->GetName() << "\n";
            std::cout << "Link: " << meetings[i]->GetLink() << "\n";
            std::cout << "Contact: " << meetings[i]->GetContact() << "\n";
            if (meetings[i]->IsRecurring())
            {
                std::cout << std::string("This meeting recurs between ") << meetings[i]->GetFirstDate()[0] << "/" <<
                              meetings[i]->GetFirstDate()[1] << "/" << meetings[i]->GetFirstDate()[2] <<
                              " and " << meetings[i]->GetSecondDate()[0] << "/" << meetings[i]->GetSecondDate()[1] <<
                              "/" << meetings[i]->GetSecondDate()[2] << "\n";
            }
            else
            {
                std::cout << "This meeting takes place on " << meetings[i]->GetFirstDate()[0] << "/" <<
                              meetings[i]->GetFirstDate()[1] << "/" << meetings[i]->GetFirstDate()[2] << "\n";
            }
            int *times = meetings[i]->GetMeetingTime()->GetTimes();
            std::cout << "It is scheduled to start at " << times[0] << ":" << times[1] << " " << (meetings[i]->GetMeetingTime()->IsStartAM() ? "AM" : "PM") <<
            " and finish at " << times[2] << ":" << times[3] << " " << (meetings[i]->GetMeetingTime()->IsEndAM() ? "AM" : "PM") << "\n\n";
        }
    }

    sqlite3_close(database);
    return meetings;
}

std::vector<Meeting*> UserData::GetMeetings(std::string contact, bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);
    
    std::string sql = "SELECT * FROM MEETINGS WHERE Contact = " + contact + " ORDER BY Name;";
    std::vector<Meeting*> meetings;

    sqlite3_exec(database, sql.c_str(), MeetingCallback, (void*) &meetings, NULL);

    if (print)
    {
        std::cout << "Found " << meetings.size() << " meetings.\n\n";
        for (int i = 0; i < meetings.size(); i++)
        {
            std::cout << "Name: " << meetings[i]->GetName() << "\n";
            std::cout << "Link: " << meetings[i]->GetLink() << "\n";
            std::cout << "Contact: " << meetings[i]->GetContact() << "\n\n";
        }
    }

    sqlite3_close(database);
    return meetings;
}

std::vector<std::string> UserData::GetContacts(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);

    std::vector<std::string> contacts;
    sqlite3_exec(database, "SELECT * FROM CONTACTS ORDER BY Name;", ContactCallback, &contacts, NULL);

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

void UserData::AddMeeting(Meeting *meeting, sqlite3 *database)
{
    bool close = false;
    if (database == nullptr)
    {
        OpenDatabase(&database);
        close = true;
    }
    
    std::string sql = "INSERT INTO MEETINGS (Name, Link, Contact, " \
                                            "StartHour, StartMinute, IsStartAM, " \
                                            "EndHour, EndMinute, IsEndAM, " \
                                            "IsRecurring, " \
                                            "FirstMonth, FirstDay, FirstYear, " \
                                            "SecondMonth, SecondDay, SecondYear, " \
                                            "Mon, Tue, Wed, Thur, Fri, Sat, Sun) ";
    sql += "VALUES ('" + meeting->GetName() + "', '" + meeting->GetLink() + "', '" + meeting->GetContact() + "'";
    
    int *times = meeting->GetMeetingTime()->GetTimes();
    sql += ", " + std::to_string(times[0]) + ", " + std::to_string(times[1]) + ", " + std::to_string(meeting->GetMeetingTime()->IsStartAM());
    sql += ", " + std::to_string(times[2]) + ", " + std::to_string(times[3]) + ", " + std::to_string(meeting->GetMeetingTime()->IsEndAM());

    sql += ", " + std::to_string(meeting->IsRecurring());
    int *firstDate = meeting->GetFirstDate();
    sql += ", " + std::to_string(firstDate[0]) + ", " + std::to_string(firstDate[1]) + ", " + std::to_string(firstDate[2]);
    if (meeting->IsRecurring())
    {
        int *secondDate = meeting->GetSecondDate();
        sql += ", " + std::to_string(secondDate[0]) + ", " + std::to_string(secondDate[1]) + ", " + std::to_string(secondDate[2]);
    }
    else
        sql += ", -1, -1, -1";

    for (int i = 0; i < 7; i++)
    {
        sql += ", " + std::to_string(meeting->GetRecurringDays()[i]);
    }
    sql += ");";

    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);

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

void UserData::AddContact(std::string contact)
{
    sqlite3 *database;
    OpenDatabase(&database);
    std::string sql = "INSERT INTO CONTACTS (Name) VALUES ('" + contact + "');";
    sqlite3_exec(database, sql.c_str(), Callback, NULL, NULL);
    sqlite3_close(database);
}

void UserData::CreateDatabase(bool populate)
{
    sqlite3 *database;
    OpenDatabase(&database);
    CreateDatabase(database);

    if (populate)
    {
        std::string contact[3] = { "Alice", "Bob", "The Universe" };
        AddContact(contact[0]);
        AddContact(contact[1]);
        AddContact(contact[2]);

        Meeting **meetings = (Meeting **) malloc(4 * sizeof(Meeting*));
        int times[4] = { 12, 0, 12, 45 };
        int date[3] = { 1, 25, 22 };
        meetings[0] = new Meeting("Design Meeting", "www.thisisnotreal.com", contact[0], new MeetingTime(times, false, false), date);
        meetings[1] = new Meeting("Class", "www.thisisveryimportant.edu", "", new MeetingTime(times, false, false), date);
        meetings[2] = new Meeting("Implementation Meeting", "www.thisissuperhelpful.org", contact[0], new MeetingTime(times, false, false), date);
        meetings[3] = new Meeting("Your Destiny", "https://docs.wxwidgets.org/3.0/", contact[2], new MeetingTime(times, false, false), date);
        AddMeeting(meetings, 4);
        delete(meetings[0]);
        delete(meetings[1]);
        delete(meetings[2]);
        delete(meetings[3]);
        free(meetings);
    }

    sqlite3_close(database);
}

void UserData::ResetDatabase(bool populate)
{
    sqlite3 *database;
    OpenDatabase(&database);
    sqlite3_exec(database, "DROP TABLE MEETINGS", Callback, NULL, NULL);
    sqlite3_exec(database, "DROP TABLE CONTACTS", Callback, NULL, NULL);
    CreateDatabase(populate);
    sqlite3_close(database);
}

//****************************
// Private member functions. *
//****************************

// References:
// https://www.sqlite.org/c3ref/exec.html
// https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
int UserData::Callback(void *data, int argc, char **argv, char **azColName)
{
    // If this is supposed to be output to the terminal, then do so
    if (data != NULL && (char*)data == "print")
    {
        for (int i = 0; i < argc; i++)
            printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        printf("\n");
    }

    return 0;
}

// This function expects 'data' to be a pointer to vector of Meeting objects
int UserData::MeetingCallback(void *data, int argc, char **argv, char **azColName)
{
    /*  (Name TEXT PRIMARY KEY, " \           0
        "Link TEXT," \                        1
        "Contact TEXT," \                     2
        "StartHour INTEGER," \                3  
        "StartMinute INTEGER," \              4
        "IsStartAM INTEGER," \                5
        "EndHour INTEGER," \                  6
        "EndMinute INTEGER," \                7
        "IsEndAM INTEGER," \                  8
        "IsRecurring INTEGER," \              9
        "FirstMonth INTEGER," \               10
        "FirstDay INTEGER," \                 11 
        "FirstYear INTEGER," \                12  
        "SecondMonth INTEGER," \              13
        "SecondDay INTEGER," \                14
        "SecondYear INTEGER," \               15
        "Mon INTEGER," \                      16
        "Tue INTEGER," \                      17
        "Wed INTEGER," \                      18
        "Thur INTEGER," \                     19
        "Fri INTEGER," \                      20
        "Sat INTEGER," \                      21
        "Sun INTEGER)                         22*/
    
    if (argc < 23)
    {
        std::cout << "Invalid number of arguments in MeetingCallback\n";
        return 0;
    }

    Meeting *meeting;
    int times[4] = { std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[6]), std::stoi(argv[7]) };
    MeetingTime *meetingTime = new MeetingTime(times, std::stoi(argv[5]), std::stoi(argv[8]));
    int firstDate[3] = { std::stoi(argv[10]), std::stoi(argv[11]), std::stoi(argv[12]) };

    // If this is a recurring meeting
    if (std::stoi(argv[9]) == 1)
    {
        int secondDate[3] = { std::stoi(argv[13]), std::stoi(argv[14]), std::stoi(argv[15]) };

        bool days[7];
        for (int i = 0; i < 7; i++)
        {
            days[i] = std::stoi(argv[16 + i], nullptr, 0);
        }

        meeting = new Meeting(argv[0], argv[1], argv[2], meetingTime, firstDate, secondDate, days);
    }
    else
    {
        meeting = new Meeting(argv[0], argv[1], argv[2], meetingTime, firstDate);
    }

    std::vector<Meeting*> *meetings = (std::vector<Meeting*> *) data;
    meetings->push_back(meeting);

    return 0;
}

// This function expects 'data' to be a pointer to vector of strings
int UserData::ContactCallback(void *data, int argc, char **argv, char **azColName)
{
    std::vector<std::string> *contacts = (std::vector<std::string> *) data;
    contacts->push_back(argv[0]);

    return 0;
}

void UserData::CreateDatabase(sqlite3 *database)
{
    sqlite3_exec(database, "CREATE TABLE MEETINGS (Name TEXT PRIMARY KEY," \
                                                  "Link TEXT," \
                                                  "Contact TEXT," \
                                                  "StartHour INTEGER," \
                                                  "StartMinute INTEGER," \
                                                  "IsStartAM INTEGER," \
                                                  "EndHour INTEGER," \
                                                  "EndMinute INTEGER," \
                                                  "IsEndAM INTEGER," \
                                                  "IsRecurring INTEGER," \
                                                  "FirstMonth INTEGER," \
                                                  "FirstDay INTEGER," \
                                                  "FirstYear INTEGER," \
                                                  "SecondMonth INTEGER," \
                                                  "SecondDay INTEGER," \
                                                  "SecondYear INTEGER," \
                                                  "Mon INTEGER," \
                                                  "Tue INTEGER," \
                                                  "Wed INTEGER," \
                                                  "Thur INTEGER," \
                                                  "Fri INTEGER," \
                                                  "Sat INTEGER," \
                                                  "Sun INTEGER);", Callback, NULL, NULL);
    
    sqlite3_exec(database, "CREATE TABLE CONTACTS (Name TEXT PRIMARY KEY);", Callback, NULL, NULL);
}

// This is abstracted away so that any error-handling or specifics of finding the right filepath to the database
// doesn't need to be repeated. This method might never become very long, but in case it does, I'm starting with
// it already abstracted away.
void UserData::OpenDatabase(sqlite3 **database, std::string name)
{
    if (sqlite3_open(name.c_str(), database) != 0)
        printf("An error occurred opening the database: %s\n", sqlite3_errmsg(*database));
}