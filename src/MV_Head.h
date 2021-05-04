// MV_Head.h -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// Created on 3/14/21
// Last edited on 5/1/21

#ifndef MV_HEAD_H
#define MV_HEAD_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>
#include "DailyHub.h"
#include "HubFrame.h"
#include "Meeting.h"
#include "enum_FrameType"
#include "enum_EventID"

class MV_Head: public HubFrame
{
public:
    MV_Head(const int id, const wxPoint& pos, DailyHub* _hub);
    FrameType GetFrameType();

private:
    // Event functions
    void OnOpenHome(wxCommandEvent& event);
    void OnOpenMeeting(wxCommandEvent& event);
    void OnDoubleClick(wxListEvent& event);
    void OnCreate(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnActivate(wxActivateEvent& event);

    // Miscellaneous functions
    ~MV_Head();
    void FillColumn(Meeting *meeting, int startIndex = 0);
    void OpenMeeting();

    // Development functions
    void OnResetDatabase(wxCommandEvent& event);
    void OnPrintDatabase(wxCommandEvent& event);
    void OnPrintContacts(wxCommandEvent& event);

    DailyHub *hub;
    wxListCtrl *meetingsList;
    std::vector<Meeting *> meetings;

    wxDECLARE_EVENT_TABLE();
};

#endif
