// MV_Create.cpp -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 4/7/21

// Resources:
// https://docs.wxwidgets.org/3.0/overview_sizer.html
// https://docs.wxwidgets.org/3.0/classwx_flex_grid_sizer.html
// https://docs.wxwidgets.org/3.0/classwx_sizer_flags.html
// https://docs.wxwidgets.org/3.0/classwx_text_ctrl.html

#include "MV_Create.h"
#include "Meeting.h"

//***************************
// Public member functions. *
//***************************

MV_Create::MV_Create(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - New Meeting", id, pos, size, false)
{
    hub = _hub;

    wxSizerFlags labelFlags(0);
    labelFlags.Center().Right().Border();
    
    topSizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *dataSizer = new wxFlexGridSizer(3, 2, 0, 5);

    // Add prompt for the meeting name and a text field w/ limited characters to answer it
    // Add(wxSizer *sizer, const wxSizerFlags &flags)
    dataSizer->Add(new wxStaticText(this, 0, "Meeting Name:", wxDefaultPosition, wxDefaultSize, 0, ""), labelFlags);
    nameTxt = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    nameTxt->SetMaxLength(35);
    // Add(wxSizer *sizer, int proportion=0, int flag=0, int border=0, wxObject *userData=NULL)
    dataSizer->Add(nameTxt, 1, wxEXPAND | wxALL, 10);

    // Add prompt for contact name and a text field w/ limited characters to answer it
    dataSizer->Add(new wxStaticText(this, 0, "Contact Name:", wxDefaultPosition, wxDefaultSize, 0, ""), labelFlags);
    contactTxt = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    contactTxt->SetMaxLength(35);
    dataSizer->Add(contactTxt, 1, wxEXPAND | wxALL, 10);

    // Add a prompt for the meeting link and a text field to answer it
    dataSizer->Add(new wxStaticText(this, 0, "Meeting Link:", wxDefaultPosition, wxDefaultSize, 0, ""), labelFlags);
    linkTxt = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    dataSizer->Add(linkTxt, 1, wxEXPAND | wxALL, 10);

    // Add all of these data entry fields to the top sizer
    topSizer->Add(dataSizer, wxSizerFlags(0).Center());

    // Create the checkbox to indicate a recurring meeting and add it to the top sizer
    wxCheckBox *recurringCheckBox = new wxCheckBox(this, ID_ToggleCheckBox, "This is a recurring meeting");
    topSizer->Add(recurringCheckBox, wxSizerFlags(0).Center());

    // Set up a list of checkboxes for every day of the week and add each one to an array that can be accessed later
    wxBoxSizer *recurringDaysSizer = new wxBoxSizer(wxHORIZONTAL);
    wxCheckBox *mon = new wxCheckBox(this, 0, "Mon");
    recurringDaysSizer->Add(mon, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[0] = mon;
    wxCheckBox *tue = new wxCheckBox(this, 0, "Tue");
    recurringDaysSizer->Add(tue, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[1] = tue;
    wxCheckBox *wed = new wxCheckBox(this, 0, "Wed");
    recurringDaysSizer->Add(wed, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[2] = wed;
    wxCheckBox *thur = new wxCheckBox(this, 0, "Thur");
    recurringDaysSizer->Add(thur, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[3] = thur;
    wxCheckBox *fri = new wxCheckBox(this, 0, "Fri");
    recurringDaysSizer->Add(fri, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[4] = fri;
    wxCheckBox *sat = new wxCheckBox(this, 0, "Sat");
    recurringDaysSizer->Add(sat, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[5] = sat;
    wxCheckBox *sun = new wxCheckBox(this, 0, "Sun");
    recurringDaysSizer->Add(sun, wxSizerFlags(0).Border(wxRIGHT, 5));
    recurringDays[6] = sun;

    topSizer->Add(recurringDaysSizer, wxSizerFlags(0).Center().Border(wxLEFT | wxRIGHT | wxUP, 10));
    topSizer->Hide(recurringDaysSizer);
    topSizer->Layout();

    // Add the buttons at the bottom of the window and add them to the top sizer
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), wxSizerFlags(0).Border(wxALL, 10));
    buttonSizer->Add(new wxButton(this, wxID_OK, "Create"), wxSizerFlags(0).Border(wxALL, 10));
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center());
    
    SetSizer(topSizer);
}

FrameType MV_Create::GetFrameType()
{
    return FrameType::MVCreate;
}

//****************************
// Private member functions. *
//****************************

void MV_Create::OnCreate(wxCommandEvent& event)
{
    // If a required field is empty, warn the user and quit the method early
    if (nameTxt->GetLineLength(0) == 0 || contactTxt->GetLineLength(0) == 0 || linkTxt->GetLineLength(0) == 0)
    {
        wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "One or more required fields are missing data.\nPlease fill out the fields and try again.", "",
                wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

        if (warningDialog->ShowModal())
            return;
    }

    Meeting *meeting;

    if (recurring)
    {
        // Get the true/false values from all of the checkboxes for the days of the week and store in an array
        bool _recurringDays[7] = {0};
        bool dataEntered = false;
        for (int i = 0; i < 7; i++)
        {
            _recurringDays[i] = recurringDays[i]->IsChecked();

            if (!dataEntered && _recurringDays[i])
                dataEntered = true;
        }

        // If no days were selected, alert the user and quit the function early
        if (!dataEntered)
        {
            wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "No days have been selected for this recurring meeting.\nPlease select at least one day.", "",
                wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

            if (warningDialog->ShowModal())
                return;
        }

        meeting = new Meeting(nameTxt->GetValue().ToStdString(), linkTxt->GetValue().ToStdString());
    }
    else
    {
        meeting = new Meeting(nameTxt->GetValue().ToStdString(), linkTxt->GetValue().ToStdString());
    }

    //****************************************************************************************************************
    // To-do: Add meeting to database (this might not need an object made here, but it's proof of concept, at least) *
    //****************************************************************************************************************
    //std::cout << "Name: " << meeting->GetName() << "\nLink: " << meeting->GetLink() << "\n";
    delete(meeting);

    Close(true);
}

void MV_Create::OnRecurring(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        recurring = true;
        topSizer->Show((size_t) 2);
        SetSize(wxDefaultCoord, wxDefaultCoord, 450, 280);
    }
    else
    {
        recurring = false;
        topSizer->Hide((size_t) 2);
        SetSize(wxDefaultCoord, wxDefaultCoord, 450, 250);
    }

    // Fix the layout of the sizers to adapt to the shown/hidden list of checkboxes (must be called after every adjustment)
    topSizer->Layout();
}

void MV_Create::OnCancel(wxCommandEvent& event)
{
    Close(false); // false indicates that this command can be vetoed if the user chooses
}

// This is called by "Close()" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void MV_Create::OnClosed(wxCloseEvent& event)
{
    // If any data has been entered in this window, check with the user if they really want to quit (if allowed to)
    if (event.CanVeto() && (nameTxt->GetLineLength(0) > 0 || contactTxt->GetLineLength(0) > 0 || linkTxt->GetLineLength(0) > 0))
    {
        wxMessageDialog *closeDialog = new wxMessageDialog(this,
                "This window contains unsaved data.\nAre you sure you want to close it?", "",
                wxICON_EXCLAMATION | wxOK | wxCANCEL, wxDefaultPosition);

        // If the user chooses the cancel button, return from the method before any windows are closed
        if (closeDialog->ShowModal() == wxID_CANCEL)
        {
            event.Veto();
            return;
        }
    }

    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);

    // If no other frame is currently open, open the main meeting view window so the program
    // doesn't entirely exit
    if (hub->FrameCount() == 0)
        hub->OpenFrame(FrameType::MVHead);

    for (int i = 0; i < 7; i++)
    {
        recurringDays[i] = nullptr;
    }

    Destroy();
}

wxBEGIN_EVENT_TABLE(MV_Create, wxFrame)
    EVT_BUTTON(wxID_CANCEL, MV_Create::OnCancel)
    EVT_BUTTON(wxID_OK, MV_Create::OnCreate)
    EVT_CHECKBOX(ID_ToggleCheckBox, MV_Create::OnRecurring)
    EVT_CLOSE(MV_Create::OnClosed)
wxEND_EVENT_TABLE()