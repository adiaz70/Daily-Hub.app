// AboutWindow.h
// MS: 5/4/21 - initial code

#ifndef ABOUT_WINDOW
#define ABOUT_WINDOW

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"

class AboutWindow: public HubFrame
{
public:
    AboutWindow(const int id, const wxPoint& pos, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnClosed(wxCloseEvent& event);
    DailyHub* hub;

    wxDECLARE_EVENT_TABLE();
};

#endif