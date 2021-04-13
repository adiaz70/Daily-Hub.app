// MV_View.h -- the frame where an individual Meeting can be viewed and notes can be taken
// Maintained by: Marcus Schmidt
// Created on 4/12/21
// Last edited on 4/12/21

#ifndef MV_VIEW_H
#define MV_VIEW_H

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "Meeting.h"
#include "enum_FrameType"
#include "enum_EventID"

class MV_View: public HubFrame
{
public:
    MV_View(Meeting *_meeting, const int id, const wxPoint& pos, DailyHub *_hub);
    FrameType GetFrameType();

private:
    void OnOpenHome(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    DailyHub *hub;
    Meeting *meeting;

    wxDECLARE_EVENT_TABLE();
};

#endif