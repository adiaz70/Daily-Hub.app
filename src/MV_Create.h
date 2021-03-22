// MV_Create.h -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 3/22/21

#ifndef MV_CREATE_H
#define MV_CREATE_H

#include <wx/wx.h>
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
    void OnClosed(wxCloseEvent& event);
    DailyHub *hub;
    wxTextCtrl *nameTxt;
    wxTextCtrl *contactTxt;
    wxTextCtrl *linkTxt;

    wxDECLARE_EVENT_TABLE();
};

#endif