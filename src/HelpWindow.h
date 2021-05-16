// HelpWindow.h
// MS: 5/15/21 - initial code

#ifndef HELP_WINDOW_H
#define HELP_WINDOW_H

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"

class HelpWindow: public HubFrame
{
public:
    HelpWindow(const int id, const wxPoint& pos, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnClosed(wxCloseEvent& event);
    DailyHub* hub;

    wxDECLARE_EVENT_TABLE();
};

#endif