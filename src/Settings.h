// Settings.h
// MS: 5/5/21 - initial code

#ifndef SETTINGS_H
#define SETTINGS

#include <string>
#include <sqlite3.h>

class Settings
{
public:
    // Getters
    static std::string GetDatabasePath();

    // Setters
    static void SetDatabasePath(std::string path);

private:
    Settings() { }
    static std::string FetchDatabasePath();
    static int PathCallback(void *data, int argc, char **argv, char **colName);
    static std::string GetUserName();

    static const std::string settingsPath;
    static std::string databasePath;
};

#endif