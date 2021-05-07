// SettingsWindow.h
// MS: 5/5/21 - initial code

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
    void OnClosed(wxCloseEvent& event);
    DailyHub* hub;
    wxStaticText *filepath;

    wxDECLARE_EVENT_TABLE();
};

#endif