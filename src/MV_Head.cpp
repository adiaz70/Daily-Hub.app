// MV_Head.cpp -- the 'head' (or primary frame) for the Meeting View
// Maintained by: Marcus Schmidt
// Created on 3/14/21
// Last edited on 5/15/21

#include "MV_Head.h"
#include "UserData.h"
#include <string>

//***************************
// Public member functions. *
//***************************

MV_Head::MV_Head(const int id, const wxPoint& pos, DailyHub *_hub)
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

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(meetingMenu, "&Meetings");
    SetMenuBar(menuBar);

    // Now that the menu bars have been set, the window itself can be created
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    // Start by storing a copy of all of the currently scheduled meetings
    meetings = UserData::GetMeetings();
    // Create the wxListCtrl that will display the meeting information and allow one to be selected
    meetingsList = new wxListCtrl(this, 0, wxDefaultPosition, wxSize(510, 250), wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_VRULES);
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
    buttonSizer->Add(new wxButton(this, ID_OpenMVCreate, "New"), wxSizerFlags(0).Border(wxLEFT | wxDOWN | wxRIGHT, 10));
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border(wxUP, 5));

    topSizer->Add(5, 10);

    SetSizerAndFit(topSizer);
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

void MV_Head::OnActivate(wxActivateEvent& event)
{
    // Whenever the window was activated in some way
    if (event.GetActive())
    {
        // Compare the last time the list was updated with the last time the database was accessed,
        // and update the list if the database was accessed more recently
        if (lastListUpdateTime < UserData::GetLastAccessTime())
        {
            // Update the time
            lastListUpdateTime = UserData::GetLastAccessTime();

            // Free the memory from the old list of meetings
            for (int i = 0; i < meetings.size(); i++)
                delete(meetings[i]);
            meetingsList->DeleteAllItems();

            // And create a new list
            meetings = UserData::GetMeetings();
            for (int i = 0; i < meetings.size(); i++)
                FillColumn(meetings[i], i);
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
                if (daysString.length() != 0)
                    daysString += ", ";

                daysString += dayNames[i];
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
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MV_Head::OnDoubleClick)
    EVT_BUTTON(ID_OpenMVView, MV_Head::OnOpenMeeting)
    EVT_BUTTON(ID_OpenMVCreate, MV_Head::OnCreate)
    EVT_BUTTON(wxID_EDIT, MV_Head::OnEdit)
    EVT_BUTTON(wxID_DELETE, MV_Head::OnDelete)
    EVT_ACTIVATE(MV_Head::OnActivate)
    EVT_CLOSE(MV_Head::OnClosed)
wxEND_EVENT_TABLE()