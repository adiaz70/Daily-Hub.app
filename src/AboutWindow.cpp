// AboutWindow.cpp
// MS: 5/4/21 - initial code

#include "AboutWindow.h"

//**************************
// Public member functions *
//**************************

AboutWindow::AboutWindow(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("About", id, pos, wxSize(460, 250), false)
{
    hub = _hub;

    // Build window here
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(new wxStaticText(this, 0, "The Daily Hub is a Linux-based desktop app designed to help you navigate your day online.\n"), wxSizerFlags(0).Border(wxLEFT | wxUP, 15));
    
    topSizer->Add(5, 5);
    topSizer->Add(new wxStaticText(this, 0, "The Daily Hub team:"), wxSizerFlags(0).Border(wxLEFT | wxUP, 15));
    topSizer->Add(new wxStaticText(this, 0, "Alyssa Diaz"), wxSizerFlags(0).Center().Border(wxUP, 5));
    topSizer->Add(new wxStaticText(this, 0, "Paula Rodriguez"), wxSizerFlags(0).Center().Border(wxUP, 5));
    topSizer->Add(new wxStaticText(this, 0, "Marcus Schmidt"), wxSizerFlags(0).Center().Border(wxUP, 5));

    topSizer->Add(5, 20);
    topSizer->Add(new wxStaticText(this, 0, "March-May 2021, CMPS 3350 @CSUB with Nick Toothman"), wxSizerFlags(0).Center().Border());

    SetSizer(topSizer);
}

FrameType AboutWindow::GetFrameType()
{
    return FrameType::About;
}

//***************************
// Private member functions *
//***************************

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void AboutWindow::OnClosed(wxCloseEvent& event)
{
    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

wxBEGIN_EVENT_TABLE(AboutWindow, wxFrame)
    EVT_CLOSE(AboutWindow::OnClosed)
wxEND_EVENT_TABLE()