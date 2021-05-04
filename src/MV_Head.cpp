// MV_Head.cpp -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// Created on 3/14/21
// Last edited on 5/3/21

#include "MV_Head.h"
#include "UserData.h"
#include <string>

//***************************
// Public member functions. *
//***************************

MV_Head::MV_Head(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Meeting View", id, pos, wxSize(520, 340))
{
    hub = _hub;

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(ID_OpenTempHome, "&Open Home");
    fileMenu->Append(ID_CloseFrame, "&Close window");
    fileMenu->Append(wxID_EXIT, "&Quit");

    wxMenu *meetingMenu = new wxMenu;
    meetingMenu->Append(ID_OpenMVCreate, "&Create new meeting");
    meetingMenu->Append(ID_OpenMVView, "&Open selected meeting");

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

    // Now that the menu bars have been set, the window itself can be created
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    // Start by storing a copy of all of the currently scheduled meetings
    meetings = UserData::GetMeetings();
    // Create the wxListCtrl that will display the meeting information and allow one to be selected
    meetingsList = new wxListCtrl(this, 0, wxDefaultPosition, wxSize(400, 200), wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_VRULES);
    // Add the columns
    meetingsList->AppendColumn("Name:");
    meetingsList->SetColumnWidth(0, 175);
    meetingsList->AppendColumn("Contact:");
    meetingsList->SetColumnWidth(1, 150);
    meetingsList->AppendColumn("Date:");
    meetingsList->SetColumnWidth(2, 75);
    meetingsList->AppendColumn("Occurs every:");
    meetingsList->SetColumnWidth(3, 150);
    // Add each meeting to the list
    for (int i = 0; i < meetings.size(); i++)
    {
        FillColumn(meetings[i], i);
    }
    topSizer->Add(meetingsList, wxSizerFlags(1).Center().Border().Expand());

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_DELETE, "Delete"), wxSizerFlags(0).Border(wxLEFT | wxDOWN | wxRIGHT, 10));
    buttonSizer->Add(new wxButton(this, wxID_EDIT, "Edit"), wxSizerFlags(0).Border(wxLEFT | wxDOWN | wxRIGHT, 10));
    buttonSizer->Add(new wxButton(this, ID_OpenMVView, "Open"), wxSizerFlags(0).Border(wxLEFT | wxDOWN | wxRIGHT, 10));
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border(wxUP, 10));

    SetSizer(topSizer);
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
    OpenMeeting();
}

void MV_Head::OnDoubleClick(wxListEvent& event)
{
    OpenMeeting();
}

void MV_Head::OnCreate(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::MVCreate);
}

void MV_Head::OnEdit(wxCommandEvent& event)
{
    // Credit: https://wiki.wxwidgets.org/WxListCtrl
    // Find the index of the selected item, if it exists
    long itemIndex = -1;
    while ((itemIndex = meetingsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
        hub->OpenFrame(FrameType::MVCreate, (void *) meetings[itemIndex]);
    }
}

void MV_Head::OnDelete(wxCommandEvent& event)
{
    // Credit: https://wiki.wxwidgets.org/WxListCtrl
    // Find the index of the selected item, if it exists
    long itemIndex = -1;
    while ((itemIndex = meetingsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
        UserData::DeleteMeeting(meetings[itemIndex]);
        delete(meetings[itemIndex]);
        meetings.erase(meetings.begin() + itemIndex);
        meetingsList->DeleteItem(itemIndex);
    }
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

// If the window has lost focus, a meeting or contact might have been added or deleted and could throw off the meeting selection.
// So, refresh the meetings list every time the window is raised to the focus
void MV_Head::OnActivate(wxActivateEvent& event)
{
    // Unfortunately, checking GetActive() is not very good at *only* catching when the window is brought back to the focus
    // because it also grabs every interaction with the window that leaves it as the focus (such as dragging it across the screen)
    if (event.GetActive())
    {
        std::vector<Meeting *> newMeetings = UserData::GetMeetings();

        // So, iterate through every meeting and search for any differences.
        bool discrepancy = false;
        for (int i = 0; i < meetings.size(); i++)
        {
            if (meetings[i]->GetID() != newMeetings[i]->GetID())
            {
                discrepancy = true;
                break;
            }
        }
        
        //******************************************************************************************
        // To-do: This doesn't refresh if a meeting has only been edited, not added or deleted.    *
        // Might want to change it to be based off of the time of the last change to the database? *
        //******************************************************************************************

        // If there is a difference, clear and recreate the entire list
        if (discrepancy)
        {
            for (int i = 0; i < meetings.size(); i++)
                delete(meetings[i]);
            meetingsList->DeleteAllItems();

            meetings = newMeetings;
            for (int i = 0; i < meetings.size(); i++)
                FillColumn(meetings[i], i);
        }
        else
        {
            for (int i = 0; i < newMeetings.size(); i++)
                delete(newMeetings[i]);
        }
    }

    // The event shouldn't be stopped with this handler in case some other part of the program also needs it,
    // so give the go-ahead for the event to continue propogating.
    event.Skip();
}

void MV_Head::OpenMeeting()
{
    // Credit: https://wiki.wxwidgets.org/WxListCtrl
    // Find the index of the selected item, if it exists
    long itemIndex = -1;
    while ((itemIndex = meetingsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
        // Open the corresponding meeting
        hub->OpenFrame(FrameType::MVView, (void *) meetings[itemIndex]);
    }
}

MV_Head::~MV_Head()
{
    for (int i = 0; i < meetings.size(); i++)
    {
        delete(meetings[i]);
    }
}

void MV_Head::FillColumn(Meeting *meeting, int startIndex)
{
    meetingsList->InsertItem(startIndex, wxString(meeting->GetName()));
    meetingsList->SetItem(startIndex, 1, wxString(meeting->GetContact()));
    
    // Only insert the meeting's first date in the date column if it is *not* a recurring meeting,
    // and only insert the days that it occurs on if it *is* a recurring meeting
    if (!meeting->IsRecurring())
    {
        int *date = meeting->GetFirstDate();
        std::string dateString = std::to_string(date[0]) + "/" + std::to_string(date[1]) + "/" + std::to_string(date[2]);
        meetingsList->SetItem(startIndex, 2, wxString(dateString));
    }
    else
    {
        bool *days = meeting->GetRecurringDays();
        std::string dayNames[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
        std::string daysString;
        for (int i = 0; i < 7; i++)
        {
            if (days[i])
            {
                daysString += dayNames[i] + " ";
            }
        }
        meetingsList->SetItem(startIndex, 3, wxString(daysString));
    }
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
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MV_Head::OnDoubleClick)
    EVT_BUTTON(ID_OpenMVView, MV_Head::OnOpenMeeting)
    EVT_BUTTON(wxID_EDIT, MV_Head::OnEdit)
    EVT_BUTTON(wxID_DELETE, MV_Head::OnDelete)
    EVT_ACTIVATE(MV_Head::OnActivate)
    EVT_CLOSE(MV_Head::OnClosed)
wxEND_EVENT_TABLE()