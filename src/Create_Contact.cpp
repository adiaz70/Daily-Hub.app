// Create_Contact.cpp
// Maintained by: Paula Rodriguez
// Created 4/14/21

#include "Create_Contact.h"
#include "UserData.h"

Create_Contact::Create_Contact(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub) : HubFrame("Create New Contact", id, pos, size, false)
{
    hub = _hub;

    wxSizerFlags labelFlags(0);
    labelFlags.Center().Right().Border();

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *dataSizer = new wxFlexGridSizer(3, 2, 0, 5);

    dataSizer->Add(new wxStaticText(this, GetID(), "Name: ", wxDefaultPosition, wxDefaultSize, 0, ""), labelFlags);
    name = new wxTextCtrl(this, GetID(), "", wxDefaultPosition, wxSize(300,25), wxTE_DONTWRAP);
    name->SetMaxLength(35);
    dataSizer->Add(name, 1, wxEXPAND | wxALL, 10);

    dataSizer->Add(new wxStaticText(this, GetID(), "E-Mail: ", wxDefaultPosition, wxDefaultSize, 0, ""), labelFlags);
    email = new wxTextCtrl(this, GetID(), "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    email->SetMaxLength(35);
    dataSizer->Add(email, 1, wxEXPAND| wxALL, 10);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), wxSizerFlags(0).Border(wxALL, 10));
    buttonSizer->Add(new wxButton(this, wxID_OK, "Confirm"), wxSizerFlags(0).Border(wxALL, 10));

    mainSizer->Add(dataSizer, wxSizerFlags(0).Center());
    mainSizer->Add(buttonSizer, wxSizerFlags(0).Center());

    SetSizer(mainSizer);
}

FrameType Create_Contact::GetFrameType()
{
    return FrameType::CreateContact;
}

void Create_Contact::OnCreate(wxCommandEvent& event)
{

    UserData::AddContact(name->GetValue().ToStdString());


    Close(true);
}

void Create_Contact::OnCancel(wxCommandEvent& event)
{
    Close(false);
}

void Create_Contact::OnClosed(wxCloseEvent& event)
{
    if(event.CanVeto() && (name->GetLineLength(0) > 0 || email->GetLineLength(0) > 0))
    {
        wxMessageDialog *closeDialog = new wxMessageDialog(this, "This window contains unsaved data.\nAre you sure you want to close it?",
        "", wxICON_EXCLAMATION | wxOK |wxCANCEL, wxDefaultPosition);

        if(closeDialog->ShowModal() == wxID_CANCEL)
        {
            event.Veto();
            return;
        }
    }

    if(!forgotten)
        hub->ForgetFrame(this);

    if(hub->FrameCount() == 0)
        hub->OpenFrame(FrameType::CVHead);

    Destroy();
}

wxBEGIN_EVENT_TABLE(Create_Contact, wxFrame)
    EVT_BUTTON(wxID_CANCEL, Create_Contact::OnCancel)
    EVT_BUTTON(wxID_OK, Create_Contact::OnCreate)
    EVT_CLOSE(Create_Contact::OnClosed)
wxEND_EVENT_TABLE()