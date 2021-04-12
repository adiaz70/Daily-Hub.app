// TempHomeFrame.cpp
// MS: 3/14/21 - initial code

// This is just a temporary home screen that can open the others, meant to be
// replaced and expanded at a later date.

#include "TempHomeFrame.h"

TempHomeFrame::TempHomeFrame(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - Home", id, pos, size)
{
    hub = _hub;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenMVHead, "&Open Meeting View");
    menuFile->Append(ID_OpenCVHead, "&Open new contacts window");
    menuFile->Append(ID_CloseFrame, "&Close window");
    menuFile->Append(wxID_EXIT, "&Quit");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar( menuBar );
}

FrameType TempHomeFrame::GetFrameType()
{
    return FrameType::TempHome;
}

void TempHomeFrame::OnOpenMVHead(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::MVHead);
}

void TempHomeFrame::OnOpenCVHead(wxCommandEvent& event)
{
    hub->OpenFrame(FrameType::CVHead);
}

// This is called when the menu option to close the window is selected
void TempHomeFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void TempHomeFrame::OnClosed(wxCloseEvent& event)
{
    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

void TempHomeFrame::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}

wxBEGIN_EVENT_TABLE(TempHomeFrame, wxFrame)
    EVT_MENU(ID_OpenMVHead, TempHomeFrame::OnOpenMVHead)
    EVT_MENU(ID_OpenCVHead, TempHomeFrame::OnOpenCVHead)
    EVT_MENU(ID_CloseFrame,  TempHomeFrame::OnExit)
    EVT_MENU(wxID_EXIT, TempHomeFrame::OnQuit)
    EVT_CLOSE(TempHomeFrame::OnClosed)
wxEND_EVENT_TABLE()
