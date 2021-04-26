#ifndef CV_CREATE_H
#define CV_CREATE_H

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"


class Create_Contact: public HubFrame
{
public:
    Create_Contact(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnCreate(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    DailyHub* hub;

    wxBoxSizer *mainSizer;
    wxTextCtrl *name;
    wxTextCtrl *email;

    wxDECLARE_EVENT_TABLE();
};

#endif