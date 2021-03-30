// CV_Head.cpp 
// Maintained by: Paula Rodriguez
// 3/16/21

// g++ *.cpp *.h `wx-config --cxxflags --libs` -o test

#include "CV_Head.h"

CV_Head::CV_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - Contacts", id, pos, size)
{
    hub = _hub;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenTempHome, "&Open Home");
    menuFile->Append(ID_OpenCVHead, "&Open New Contacts Window");
    menuFile->Append(ID_CloseFrame, "&Close Window");
    menuFile->Append(wxID_EXIT, "&Quit");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);
}

FrameType CV_Head::GetFrameType()
{
    return FrameType::CVHead;
}

void CV_Head::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

void CV_Head::OnOpenCVHead(wxCommandEvent& event)
{
    hub->OpenFrame(FrameType::CVHead);
}

void CV_Head::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void CV_Head::OnClosed(wxCloseEvent& event)
{
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

void CV_Head::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}

wxBEGIN_EVENT_TABLE(CV_Head, wxFrame)
    EVT_MENU(ID_OpenTempHome, CV_Head::OnOpenHome)
    EVT_MENU(ID_OpenCVHead, CV_Head::OnOpenCVHead)
    EVT_MENU(ID_CloseFrame, CV_Head::OnExit)
    EVT_MENU(wxID_EXIT, CV_Head::OnQuit)
    EVT_CLOSE(CV_Head::OnClosed)
wxEND_EVENT_TABLE()