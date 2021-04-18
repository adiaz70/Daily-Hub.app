// UserData.h
// MS: 4/15/21 - initial code
// MS: 4/18/21 - can now build objects after querying the database, and contacts are handled as strings rather than objects

#include "UserData.h"
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
            std::cout << "Contact: " << meetings[i]->GetContact() << "\n\n";
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

    // Now create and return Meeting objects from the returned data... or do I want to create them all at the start
    // and then do a lookup? Probably not.

    sqlite3_close(database);
    return meetings;
}

std::vector<std::string> UserData::GetContacts(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);

    std::vector<std::string> contacts;
    sqlite3_exec(database, "SELECT DISTINCT Contact FROM MEETINGS ORDER BY Contact;", ContactCallback, &contacts, NULL);

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

    std::string sql = "INSERT INTO MEETINGS (Name, Link, Contact) VALUES ('" + meeting->GetName() + "', " \
                "'" + meeting->GetLink() + "', '" + meeting->GetContact() + "');";


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

void UserData::CreateDatabase(bool populate)
{
    sqlite3 *database;
    OpenDatabase(&database);
    CreateDatabase(database);

    if (populate)
    {
        std::string contact = "Joe";
        Meeting **meetings = (Meeting **) malloc(3 * sizeof(Meeting*));
        meetings[0] = new Meeting("Design Meeting", "www.thisisnotreal.com", contact);
        meetings[1] = new Meeting("Class", "www.thisisveryimportant.edu", "");
        meetings[2] = new Meeting("Implementation Meeting", "www.thisissuperhelpful.org", contact);
        AddMeeting(meetings, 3);
        delete(meetings[0]);
        delete(meetings[1]);
        delete(meetings[2]);
        free(meetings);
    }

    sqlite3_close(database);
}

void UserData::ResetDatabase(bool populate)
{
    sqlite3 *database;
    OpenDatabase(&database);
    sqlite3_exec(database, "DROP TABLE MEETINGS", Callback, NULL, NULL);
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
    std::vector<Meeting*> *meetings = (std::vector<Meeting*> *) data;
    meetings->push_back(new Meeting(argv[0] ? argv[0] : "", argv[1] ? argv[1] : "", argv[2] ? argv[2] : ""));

    // To-do: Need to incorporate contacts, but, how? Would have to do a lookup to find an already existing object,
    // or just start passing it around as a string... *sigh* That would probably be the 'easier' route.

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
    sqlite3_exec(database, "CREATE TABLE MEETINGS (Name TEXT PRIMARY KEY, Link TEXT, Contact TEXT);", Callback, NULL, NULL);
}

// This is abstracted away so that any error-handling or specifics of finding the right filepath to the database
// doesn't need to be repeated. This method might never become very long, but in case it does, I'm starting with
// it already abstracted away.
void UserData::OpenDatabase(sqlite3 **database)
{
    if (sqlite3_open("user_data.db", database) != 0)
        printf("An error occurred opening the database: %s\n", sqlite3_errmsg(*database));
}