// TempHomeFrame.h
// MS: 3/14/21 - initial code

// This is just a temporary home screen that can open the others, meant to be
// replaced and/or expanded at a later date.

#ifndef TEMP_HOME_FRAME_H
#define TEMP_HOME_FRAME_H

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"

class TempHomeFrame: public HubFrame
{
public:
    TempHomeFrame(const int id, const wxPoint& pos, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnOpenMVHead(wxCommandEvent& event);
    void OnOpenCVHead(wxCommandEvent& event);
    void OnOpenCalViewHead(wxCommandEvent& event);
    void OnOpenAbout(wxCommandEvent& event);
    void OnOpenHelp(wxCommandEvent& event);
    void OnOpenSettings(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    DailyHub* hub;

    wxDECLARE_EVENT_TABLE();
};

#endif
