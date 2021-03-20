// MV_Head.cpp -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// Created on 3/14/21
// Last edited on 3/20/21

#include "MV_Head.h"

//***************************
// Public member functions. *
//***************************

MV_Head::MV_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - Meeting #" + wxString() << id, id, pos, size)
{
    hub = _hub;

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(ID_OpenTempHome, "&Open Home");
    fileMenu->Append(ID_CloseFrame, "&Close window");
    fileMenu->Append(wxID_EXIT, "&Quit");

    wxMenu *meetingMenu = new wxMenu;
    meetingMenu->Append(ID_OpenMVCreate, "&Create new meeting");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(meetingMenu, "&Meetings");
    SetMenuBar(menuBar);
}

FrameType MV_Head::GetFrameType()
{
    return FrameType::MVHead;
}

//****************************
// Private member functions. *
//****************************

void MV_Head::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

void MV_Head::OnCreate(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::MVCreate);
}

// This is called when the menu option to close the window is selected
void MV_Head::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void MV_Head::OnClosed(wxCloseEvent& event)
{
    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

void MV_Head::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}

wxBEGIN_EVENT_TABLE(MV_Head, wxFrame)
    EVT_MENU(ID_OpenTempHome, MV_Head::OnOpenHome)
    EVT_MENU(ID_OpenMVCreate, MV_Head::OnCreate)
    EVT_MENU(ID_CloseFrame, MV_Head::OnExit)
    EVT_MENU(wxID_EXIT, MV_Head::OnQuit)
    EVT_CLOSE(MV_Head::OnClosed)
wxEND_EVENT_TABLE()
