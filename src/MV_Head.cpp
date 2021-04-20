// MV_Head.cpp -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// Created on 3/14/21
// Last edited on 4/20/21

#include "MV_Head.h"
#include "UserData.h"
#include <vector>

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
    meetingMenu->Append(ID_OpenMVView, "&Open random meeting");

    //******************************
    // Testing code - remove later *
    //******************************
    meetingMenu->Append(wxID_YES, "&Reset database");
    meetingMenu->Append(wxID_OK, "&Print database");
    meetingMenu->Append(wxID_YESTOALL, "&Print contacts");

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

void MV_Head::OnOpenMeeting(wxCommandEvent& event)
{
    std::vector<Meeting *> meetings = UserData::GetMeetings();
    hub->OpenFrame(FrameType::MVView, (void *) meetings[rand() % meetings.size()]);
    
    for (int i = 0; i < meetings.size(); i++)
    {
        delete(meetings[i]);
    }
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

void MV_Head::OnResetDatabase(wxCommandEvent& event)
{
    UserData::ResetDatabase(true);
}

void MV_Head::OnPrintDatabase(wxCommandEvent& event)
{
    // Calling UserData::GetMeetings(true) will print every meeting in the database, but it also creates a bunch of objects in memory
    std::vector<Meeting *> meetings = UserData::GetMeetings(true);
    // So, go through and delete them all
    for (int i = 0; i < meetings.size(); i++)
    {
        delete(meetings[i]);
    }
}

void MV_Head::OnPrintContacts(wxCommandEvent& event)
{
    UserData::GetContacts(true);
}

wxBEGIN_EVENT_TABLE(MV_Head, wxFrame)
    EVT_MENU(ID_OpenTempHome, MV_Head::OnOpenHome)
    EVT_MENU(ID_OpenMVView, MV_Head::OnOpenMeeting)
    EVT_MENU(ID_OpenMVCreate, MV_Head::OnCreate)
    EVT_MENU(ID_CloseFrame, MV_Head::OnExit)
    EVT_MENU(wxID_EXIT, MV_Head::OnQuit)
    EVT_MENU(wxID_YES, MV_Head::OnResetDatabase)
    EVT_MENU(wxID_OK, MV_Head::OnPrintDatabase)
    EVT_MENU(wxID_YESTOALL, MV_Head::OnPrintContacts)
    EVT_CLOSE(MV_Head::OnClosed)
wxEND_EVENT_TABLE()