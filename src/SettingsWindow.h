// SettingsWindow.h
// MS: 5/5/21 - initial code
// MS: 5/11/21 - made topSizer a private member variable for later access
// MS: 5/14/21 - added functions to reset database (with and without test values)

#ifndef SETTINGS_WINDOW
#define SETTINGS_WINDOW

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"

class SettingsWindow: public HubFrame
{
public:
    SettingsWindow(const int id, const wxPoint& pos, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnChangeFilepath(wxCommandEvent& event);
    void OnResetFilepath(wxCommandEvent& event);
    void OnResetDatabase(wxCommandEvent& event);
    void OnCreateTestDatabase(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    DailyHub* hub;
    wxBoxSizer *topSizer;
    wxStaticText *filepath;

    wxDECLARE_EVENT_TABLE();
};

#endif