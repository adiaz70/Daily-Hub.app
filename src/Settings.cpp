// Settings.cpp
// MS: 5/5/21 - initial code
// MS: 5/6/21 - some bug fixes/improvements
// MS: 5/12/21 - attempted to improve runtime safety

#include "Settings.h"
#include "UserData.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

//**************************
// Public member functions *
//**************************

std::string Settings::GetDatabasePath() { return databasePath; }

void Settings::SetDatabasePath(std::string path)
{
    // Update the filepath
    databasePath = path;
    // And make sure that it ends in a forward slash
    if (databasePath[databasePath.length() - 1] != '/')
        databasePath += "/";

    // Then update the information in the settings database by first opening it
    sqlite3 *database;
    path = settingsPath + "user_data.db";
    // Print an error if there is a problem opening it
    if (sqlite3_open(path.c_str(), &database) != 0)
    {
        printf("An error occurred opening the database: %s\n", sqlite3_errmsg(database));
        return;
    }
    // And then update its contents
    std::string sql = "UPDATE SETTINGS SET DatabasePath = '" + databasePath + "';";
    sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    sqlite3_close(database);

    // Finally, alert the database that it needs to refresh because it might have totally different contents, now
    UserData::RefreshDatabase();
}

void Settings::ResetDatabasePath() { SetDatabasePath(settingsPath); }

void Settings::Init()
{
    std::string userName = Settings::GetUserName();
    if (userName.length() != 0)
        settingsPath = "/home/" + userName + "/.dailyhub/";
    else
        settingsPath = ".dailyhub/";

    databasePath = Settings::FetchDatabasePath();
}

//***************************
// Private member functions *
//***************************

std::string Settings::FetchDatabasePath()
{
    sqlite3 *database;
    std::string path = settingsPath + "user_data.db";

    if (sqlite3_open(path.c_str(), &database) != 0)
    {
        // If there was an error, it's most likely because this is the first program launch and the directory has not been created yet.
        // So, create the directory and try again
        mkdir(settingsPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        // If it *still* doesn't work... then we have big problems, lol
        if (sqlite3_open(path.c_str(), &database) != 0)
        {
            printf("An error occurred opening the database: %s\n", sqlite3_errmsg(database));
            return "";
        }
    }

    path = "";
    // The first (and only) row in the database should be the user's desired filepath to the database
    sqlite3_exec(database, "SELECT * FROM SETTINGS ORDER BY ROWID ASC LIMIT 1;", PathCallback, (void *) &path, NULL);

    // If nothing was found, then it's (most likely) because this is the first launch and it needs to be created
    if (path.length() == 0)
    {
        // So, create the table and insert the default filepath that can be changed later
        sqlite3_exec(database, "CREATE TABLE SETTINGS (DatabasePath TEXT);", NULL, NULL, NULL);
        std::string sql = "INSERT INTO SETTINGS (DatabasePath) VALUES ('" + settingsPath + "');";
        sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);

        return settingsPath;
    }

    sqlite3_close(database);
    return path;
}

int Settings::PathCallback(void *data, int argc, char **argv, char **colName)
{
    std::string *path = (std::string *) data;
    if (argc > 0)
        *path = argv[0];
    else
        *path = "";

    return 0;
}

std::string Settings::GetUserName()
{
    char *userName = cuserid(nullptr);

    if (userName == nullptr)
        return "";
    else
    {
        std::string name = userName;
        return name;
    }
}

// 'settingsPath' is where there should *always* be a database with *at least* the filepath to the actual database
std::string Settings::settingsPath = "";
// 'databasePath' is the filepath to the actual database that equals 'settingsPath' by default but can be changed by the user.
// This is set when DailyHub::OnInit() executes and calls Settings::Init()
std::string Settings::databasePath = "";