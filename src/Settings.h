// Settings.h
// MS: 5/5/21 - initial code
// MS: 5/12/21 - settingsPath is no longer constant

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
    static void ResetDatabasePath();

    static void Init();

private:
    Settings() { }
    static std::string FetchDatabasePath();
    static int PathCallback(void *data, int argc, char **argv, char **colName);
    static std::string GetUserName();

    static std::string settingsPath;
    static std::string databasePath;
};

#endif