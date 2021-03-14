// MV_Head.cpp -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// 3/14/21

#include "MV_Head.h"

MV_Head::MV_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - Meeting #" + wxString() << id, id, pos, size)
{
    hub = _hub;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenTempHome, "&Open Home");
    menuFile->Append(ID_OpenMVHead, "&Open new window");
    menuFile->Append(ID_CloseFrame, "&Close window");
    menuFile->Append(wxID_EXIT, "&Quit");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);
}

FrameType MV_Head::GetFrameType()
{
    return FrameType::MVHead;
}

void MV_Head::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

void MV_Head::OnOpenMVHead(wxCommandEvent& event)
{
    hub->OpenFrame(FrameType::MVHead);
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
    EVT_MENU(ID_OpenMVHead, MV_Head::OnOpenMVHead)
    EVT_MENU(ID_CloseFrame, MV_Head::OnExit)
    EVT_MENU(wxID_EXIT, MV_Head::OnQuit)
    EVT_CLOSE(MV_Head::OnClosed)
wxEND_EVENT_TABLE()
