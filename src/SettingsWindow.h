// SettingsWindow.h
// MS: 5/5/21 - initial code

#ifndef SETTINGS_WINDOW
#define SETTINGS_WINDOW

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"

class SettingsWindow: public HubFrame
{
public:
    SettingsWindow(const int id, const wxPoint& pos, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnClosed(wxCloseEvent& event);
    DailyHub* hub;

    wxDECLARE_EVENT_TABLE();
};

#endif