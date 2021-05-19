// HelpWindow.cpp
// MS: 5/15/21 - initial code

#include "HelpWindow.h"

//**************************
// Public member functions *
//**************************

HelpWindow::HelpWindow(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Help", id, pos, wxSize(500, 345))
{
    hub = _hub;

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxFont font;

    topSizer->Add(5, 10);

    wxStaticText *uninstallLabel = new wxStaticText(this, 0, "How do I uninstall the Daily Hub?");
    font = uninstallLabel->GetFont();
    font.MakeBold();
    uninstallLabel->SetFont(font);
    topSizer->Add(uninstallLabel, wxSizerFlags(0).Center().Border(wxLEFT | wxUP | wxRIGHT, 15));
    topSizer->Add(5, 10);
    wxStaticText *uninstallOne = new wxStaticText(this, 0, "The only files the Daily Hub places on your computer are the program executable and the database. Both can simply be moved to the trash and deleted.");
    uninstallOne->Wrap(2000);
    topSizer->Add(uninstallOne, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 15));
    wxStaticText *uninstallTwo = new wxStaticText(this, 0, "By default, the database is located in the Home directory in a hidden folder titled \"dailyhub\". You can check it's current location by opening the Settings window.");
    uninstallTwo->Wrap(2000);
    topSizer->Add(uninstallTwo, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 15));

    wxStaticText *emptyLabel = new wxStaticText(this, 0, "The Meeting View doesn't show any of the meetings I create.");
    emptyLabel->SetFont(font);
    topSizer->Add(emptyLabel, wxSizerFlags(0).Center().Border(wxLEFT | wxUP | wxRIGHT, 15));
    topSizer->Add(5, 10);
    wxStaticText *emptyOne = new wxStaticText(this, 0, "Open Settings, press \"Clear Database\", and try again.");
    emptyOne->Wrap(2000);
    topSizer->Add(emptyOne, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 15));

    SetSizer(topSizer);

    SetMinSize(wxSize(500, 345));
}

FrameType HelpWindow::GetFrameType()
{
    return FrameType::Help;
}

//***************************
// Private member functions *
//***************************

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void HelpWindow::OnClosed(wxCloseEvent& event)
{
    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

wxBEGIN_EVENT_TABLE(HelpWindow, wxFrame)
    EVT_CLOSE(HelpWindow::OnClosed)
wxEND_EVENT_TABLE()