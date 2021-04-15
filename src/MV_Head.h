// MV_Head.h -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// Created on 3/14/21
// Last edited on 4/12/21

#ifndef MV_HEAD_H
#define MV_HEAD_H

#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"

class MV_Head: public HubFrame
{
public:
    MV_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnOpenHome(wxCommandEvent& event);
    void OnOpenMeeting(wxCommandEvent& event);
    void OnCreate(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnResetDatabase(wxCommandEvent& event);
    void OnPrintDatabase(wxCommandEvent& event);
    void OnPrintContacts(wxCommandEvent& event);
    DailyHub* hub;

    wxDECLARE_EVENT_TABLE();
};

#endif
