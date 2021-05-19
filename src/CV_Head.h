// CV_Head.cpp 
// Maintained by: Paula Rodriguez
// 3/16/21

#ifndef CV_HEAD_H
#define CV_HEAD_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"

class CV_Head: public HubFrame
{
public:
    CV_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub);
    FrameType GetFrameType();

private:
    void OnOpenHome(wxCommandEvent& event);
    void OnOpenCVHead(wxCommandEvent& event);
    void OnCreate(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClosed(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    DailyHub* hub;

    wxListCtrl *contactsList;
    wxListCtrl *contactInfo;
    std::vector<std::string> contacts;

    wxDECLARE_EVENT_TABLE();
};

#endif