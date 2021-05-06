// SettingsWindow.cpp
// MS: 5/5/21 - initial code

#include "SettingsWindow.h"
#include "Settings.h"

//**************************
// Public member functions *
//**************************

SettingsWindow::SettingsWindow(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Settings", id, pos, wxSize(460, 250), false)
{
    hub = _hub;

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
    pathSizer->Add(new wxStaticText(this, 0, "Database Filepath:"), wxSizerFlags(0).Center().Border());
    filepath = new wxStaticText(this, 0, wxString(Settings::GetDatabasePath()));
    wxFont font = filepath->GetFont();
    font.MakeItalic();
    filepath->SetFont(font);
    pathSizer->Add(filepath, wxSizerFlags(0).Center().Border(wxALL, 15));
    topSizer->Add(pathSizer, wxSizerFlags(0));

    wxButton *filepathButton = new wxButton(this, ID_MainButton, "Change Filepath");
    topSizer->Add(filepathButton, wxSizerFlags(0).Center().Border());

    SetSizer(topSizer);
}

FrameType SettingsWindow::GetFrameType()
{
    return FrameType::Settings;
}

//***************************
// Private member functions *
//***************************

void SettingsWindow::OnChangeFilepath(wxCommandEvent& event)
{
    wxDirDialog *directoryDialog = new wxDirDialog(this, "Please choose the directory for user data.", wxString(Settings::GetDatabasePath()));

    if (directoryDialog->ShowModal())
    {
        Settings::SetDatabasePath(directoryDialog->GetPath().ToStdString());
        filepath->SetLabel(Settings::GetDatabasePath());
    }
}

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
    EVT_BUTTON(ID_MainButton, SettingsWindow::OnChangeFilepath)
    EVT_CLOSE(SettingsWindow::OnClosed)
wxEND_EVENT_TABLE()