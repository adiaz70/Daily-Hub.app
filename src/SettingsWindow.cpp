// SettingsWindow.cpp
// MS: 5/5/21 - initial code

#include "SettingsWindow.h"

//**************************
// Public member functions *
//**************************

SettingsWindow::SettingsWindow(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Settings", id, pos, wxSize(460, 250), false)
{
    hub = _hub;
}

FrameType SettingsWindow::GetFrameType()
{
    return FrameType::Settings;
}

//***************************
// Private member functions *
//***************************

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void SettingsWindow::OnClosed(wxCloseEvent& event)
{
    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
    EVT_CLOSE(SettingsWindow::OnClosed)
wxEND_EVENT_TABLE()