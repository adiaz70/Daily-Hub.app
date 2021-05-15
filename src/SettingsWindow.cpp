// SettingsWindow.cpp
// MS: 5/5/21 - initial code
// MS: 5/6/21 - added reset button
// MS: 5/11/21 - sizers readjust after filepath changes
// MS: 5/14/21 - added buttons to reset database (with and without test values)

#include "SettingsWindow.h"
#include "Settings.h"
#include "UserData.h"

//**************************
// Public member functions *
//**************************

SettingsWindow::SettingsWindow(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Settings", id, pos, wxSize(460, 280), false)
{
    hub = _hub;

    topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
    pathSizer->Add(new wxStaticText(this, 0, "Database Filepath:"), wxSizerFlags(0).Center().Border());
    filepath = new wxStaticText(this, 0, wxString(Settings::GetDatabasePath()));
    wxFont font = filepath->GetFont();
    font.MakeItalic();
    filepath->SetFont(font);
    pathSizer->Add(filepath, wxSizerFlags(0).Center().Border(wxALL, 15));
    topSizer->Add(pathSizer, wxSizerFlags(0).Center().Border());

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_DEFAULT, "Reset Filepath"), wxSizerFlags(0).Center().Border());
    buttonSizer->Add(new wxButton(this, ID_MainButton, "Change Filepath"), wxSizerFlags(0).Center().Border());
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center());

    topSizer->Add(new wxButton(this, wxID_RESET, "Clear Database"), wxSizerFlags(0).Center().Border(wxUP, 20));
    topSizer->Add(new wxButton(this, wxID_PREVIEW, "Create Sample Database"), wxSizerFlags(0).Center().Border(wxUP, 25));

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

    if (directoryDialog->ShowModal() != wxID_CANCEL)
    {
        Settings::SetDatabasePath(directoryDialog->GetPath().ToStdString());
        filepath->SetLabel(Settings::GetDatabasePath());
    }

    topSizer->Layout();
}

void SettingsWindow::OnResetFilepath(wxCommandEvent& event)
{
    Settings::ResetDatabasePath();
    filepath->SetLabel(Settings::GetDatabasePath());

    topSizer->Layout();
}

void SettingsWindow::OnResetDatabase(wxCommandEvent& event)
{
    UserData::ResetDatabase();
}

void SettingsWindow::OnCreateTestDatabase(wxCommandEvent& event)
{
    UserData::ResetDatabase(true);
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
    EVT_BUTTON(wxID_DEFAULT, SettingsWindow::OnResetFilepath)
    EVT_BUTTON(wxID_RESET, SettingsWindow::OnResetDatabase)
    EVT_BUTTON(wxID_PREVIEW, SettingsWindow::OnCreateTestDatabase)
    EVT_CLOSE(SettingsWindow::OnClosed)
wxEND_EVENT_TABLE()