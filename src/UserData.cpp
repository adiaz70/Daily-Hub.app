// UserData.h
// MS: 4/15/21 - initial code

#include "UserData.h"
#include <string.h>
#include <stdio.h>

//***************************
// Public member functions. *
//***************************

void UserData::GetMeetings(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);
    sqlite3_exec(database, "SELECT * FROM MEETINGS ORDER BY Name", Callback, print ? (void*)"print" : NULL, NULL);
    sqlite3_close(database);
}

void UserData::GetMeetings(Contact *contact, bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);
    
    std::string sql = "SELECT * FROM MEETINGS WHERE Contact = " + contact->GetNameString() + " ORDER BY Name;";
    sqlite3_exec(database, sql.c_str(), Callback, print ? (void*)"print" : NULL, NULL);

    // Now create and return Meeting objects from the returned data... or do I want to create them all at the start
    // and then do a lookup? Probably not.

    sqlite3_close(database);
}

void UserData::GetContacts(bool print)
{
    sqlite3 *database;
    OpenDatabase(&database);
    sqlite3_exec(database, "SELECT DISTINCT Contact FROM MEETINGS ORDER BY Contact;", Callback, print ? (void*)"print" : NULL, NULL);
    sqlite3_close(database);
}

void UserData::AddMeeting(Meeting *meeting, sqlite3 *database)
{
    bool close = false;
    if (database == nullptr)
    {
        OpenDatabase(&database);
        close = true;
    }

    std::string contactString = "";
    if (meeting->GetContact() != NULL)
    {
        contactString = meeting->GetContact()->GetNameString();
    }

    std::string sql = "INSERT INTO MEETINGS (Name, Link, Contact) VALUES ('" + meeting->GetName() + "', " \
                "'" + meeting->GetLink() + "', '" + contactString + "');";


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
        Contact *contact = new Contact((std::string)"Joe");
        Meeting **meetings = (Meeting **) malloc(3 * sizeof(Meeting*));
        meetings[0] = new Meeting("Design Meeting", "www.thisisnotreal.com", contact);
        meetings[1] = new Meeting("Class", "www.thisisveryimportant.edu");
        meetings[2] = new Meeting("Implementation Meeting", "www.thisissuperhelpful.org", contact);
        AddMeeting(meetings, 3);
        delete(meetings[0]);
        delete(meetings[1]);
        delete(meetings[2]);
        free(meetings);
        delete(contact);
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

    // Next, load the arguments into 'data' (still as character arrays) so that it can be returned and used however desired

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