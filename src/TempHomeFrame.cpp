// TempHomeFrame.cpp
// MS: 3/14/21 - initial code
// MS: 5/4/21 - added intro/credits screen
// MS: 5/5/21 - added settings screen
// MS: 5/15/21 - added text describing each main window and buttons to open them
// MS: 5/15/21 - added help screen

// This is just a temporary home screen that can open the others, meant to be
// replaced and expanded at a later date.

#include "TempHomeFrame.h"

TempHomeFrame::TempHomeFrame(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Daily Hub - Home", id, pos, wxSize(500, 450), false)
{
    hub = _hub;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_ABOUT, "&About");
    menuFile->Append(wxID_PROPERTIES, "&Settings");
    menuFile->Append(wxID_HELP, "&Help");
    //menuFile->Append(ID_CloseFrame, "&Close window");
    menuFile->Append(wxID_EXIT, "&Quit");

    wxMenu *menuWindow = new wxMenu;
    menuWindow->Append(ID_OpenMVHead, "&Open Meeting View");
    menuWindow->Append(ID_OpenCVHead, "&Open Contact View");
    menuWindow->Append(ID_OpenCalViewHead, "&Open Calendar View");
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuWindow, "&Windows");
    SetMenuBar( menuBar );

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxFont font;

    topSizer->Add(10, 15);

    wxStaticText *mLabel = new wxStaticText(this, 0, "Meeting View:");
    font = mLabel->GetFont();
    font.MakeBold();
    mLabel->SetFont(font);
    topSizer->Add(mLabel, wxSizerFlags(0).Center().Border());
    topSizer->Add(new wxStaticText(this, 0, "View, manage, and create new meetings in a list format."), wxSizerFlags(0).Center().Border());
    topSizer->Add(new wxButton(this, ID_OpenMVHead, "Open"), wxSizerFlags(0).Center().Border());

    topSizer->Add(10, 20);

    wxStaticText *conLabel = new wxStaticText(this, 0, "Contact View:");
    conLabel->SetFont(font);
    topSizer->Add(conLabel, wxSizerFlags(0).Center().Border());
    topSizer->Add(new wxStaticText(this, 0, "Manage your list of contacts that can be assigned to meetings."), wxSizerFlags(0).Center().Border());
    topSizer->Add(new wxButton(this, ID_OpenCVHead, "Open"), wxSizerFlags(0).Center().Border());

    topSizer->Add(10, 20);

    wxStaticText *calLabel = new wxStaticText(this, 0, "Calendar View:");
    calLabel->SetFont(font);
    topSizer->Add(calLabel, wxSizerFlags(0).Center().Border());
    topSizer->Add(new wxStaticText(this, 0, "Navigate your scheduled meetings in a calendar format."), wxSizerFlags(0).Center().Border());
    topSizer->Add(new wxButton(this, ID_OpenCalViewHead, "Open"), wxSizerFlags(0).Center().Border());

    SetSizer(topSizer);
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
void TempHomeFrame::OnOpenCalViewHead(wxCommandEvent& event)
{
	hub->OpenFrame(FrameType::CalViewHead);
}
	
void TempHomeFrame::OnOpenAbout(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::About);
}

void TempHomeFrame::OnOpenHelp(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::Help);
}

void TempHomeFrame::OnOpenSettings(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::Settings);
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
    EVT_MENU(ID_OpenCalViewHead, TempHomeFrame::OnOpenCalViewHead)
    EVT_MENU(wxID_ABOUT, TempHomeFrame::OnOpenAbout)
    EVT_MENU(wxID_HELP, TempHomeFrame::OnOpenHelp)
    EVT_MENU(wxID_PROPERTIES, TempHomeFrame::OnOpenSettings)
    EVT_MENU(ID_CloseFrame,  TempHomeFrame::OnExit)
    EVT_MENU(wxID_EXIT, TempHomeFrame::OnQuit)
    EVT_BUTTON(ID_OpenMVHead, TempHomeFrame::OnOpenMVHead)
    EVT_BUTTON(ID_OpenCVHead, TempHomeFrame::OnOpenCVHead)
    EVT_BUTTON(ID_OpenCalViewHead, TempHomeFrame::OnOpenCalViewHead)
    EVT_CLOSE(TempHomeFrame::OnClosed)
wxEND_EVENT_TABLE()
