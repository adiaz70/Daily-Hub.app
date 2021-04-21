// MV_View.cpp -- the frame where an individual Meeting can be viewed and notes can be taken
// Maintained by: Marcus Schmidt
// Created on 4/12/21
// Last edited on 4/21/21

#include "MV_View.h"
#include "wx/hyperlink.h"
#include "UserData.h"

//***************************
// Public member functions. *
//***************************

MV_View::MV_View(Meeting *meeting, const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame(wxString(meeting->GetName()), id, pos, wxDefaultSize)
{
    hub = _hub;
    meetingID = meeting->GetID();
    if (meetingID == -1)
    {
        Close(true);
        return;
    }

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(ID_OpenTempHome, "&Open Home");
    fileMenu->Append(ID_OpenMVHead, "&Open Meeting View");
    fileMenu->Append(ID_CloseFrame, "&Close window");
    fileMenu->Append(wxID_EXIT, "&Quit");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *linkSizer = new wxBoxSizer(wxHORIZONTAL);
    linkSizer->Add(new wxStaticText(this, 0, "Meeting Link:"), wxSizerFlags(0).Center().Border(wxLEFT, 15));
    linkSizer->Add(new wxHyperlinkCtrl(this, 0, wxString(meeting->GetLink()), wxString(meeting->GetLink())),
                   wxSizerFlags(0).Center().Border(wxLEFT, 5));
    topSizer->Add(linkSizer, wxSizerFlags(0).Center().Border(wxUP, 15));

    notes = new wxTextCtrl(this, 0, UserData::GetNotes(meetingID), wxDefaultPosition, wxSize(150, 150), wxTE_MULTILINE);
    topSizer->Add(notes, wxSizerFlags(1).Border(wxALL, 15).Expand());

    SetSizerAndFit(topSizer);
}

FrameType MV_View::GetFrameType()
{
    return FrameType::MVView;
}

//****************************
// Private member functions. *
//****************************

void MV_View::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

// This is called when the menu option to close the window is selected
void MV_View::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void MV_View::OnClosed(wxCloseEvent& event)
{
    // Save the notes taken in the database
    UserData::SaveNotes(meetingID, notes->GetValue().ToStdString());

    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);

    Destroy();
}

void MV_View::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}

wxBEGIN_EVENT_TABLE(MV_View, wxFrame)
    EVT_MENU(ID_OpenTempHome, MV_View::OnOpenHome)
    EVT_MENU(ID_CloseFrame, MV_View::OnExit)
    EVT_MENU(wxID_EXIT, MV_View::OnQuit)
    EVT_CLOSE(MV_View::OnClosed)
wxEND_EVENT_TABLE()