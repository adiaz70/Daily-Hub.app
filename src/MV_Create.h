// MV_Create.h -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 4/10/21

#ifndef MV_CREATE_H
#define MV_CREATE_H

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"

class MV_Create: public HubFrame
{
public:
    MV_Create(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnCreate(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnChooseContact(wxCommandEvent& event);
    void OnRecurring(wxCommandEvent& event);
    void ToggleAM_PM(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);

    wxBoxSizer *topSizer;
    wxTextCtrl *nameTxt;
    wxTextCtrl *linkTxt;
    wxTextCtrl *meetingTime[4]; // 0 is start hour, 1 is start minute, 2 is end hour, 3 is end minute
    wxToggleButton *meetingAM_PM[4]; // 0 is the start time AM, 1 is start time PM, 2 and 3 repeat with end time AM/PM
    wxStaticText *contactName;
    wxCheckBox *recurringDays[7];

    DailyHub *hub;
    bool recurring = false;
    bool isStartAM = true;
    bool isEndAM = true;

    wxDECLARE_EVENT_TABLE();
};

#endif