// MV_Create.cpp -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 4/10/21

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
    wxFlexGridSizer *dataSizer = new wxFlexGridSizer(5, 2, 0, 5);

    // Add prompt for the meeting name and a text field w/ limited characters to answer it
    // Add(wxSizer *sizer, const wxSizerFlags &flags)
    dataSizer->Add(new wxStaticText(this, 0, "Meeting Name:", wxDefaultPosition, wxDefaultSize), labelFlags);
    nameTxt = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    nameTxt->SetMaxLength(35);
    // Add(wxSizer *sizer, int proportion=0, int flag=0, int border=0, wxObject *userData=NULL)
    dataSizer->Add(nameTxt, 1, wxEXPAND | wxALL, 10);

    // Add a prompt for the meeting link and a text field to answer it
    dataSizer->Add(new wxStaticText(this, 0, "Meeting Link:", wxDefaultPosition, wxDefaultSize), labelFlags);
    linkTxt = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    dataSizer->Add(linkTxt, 1, wxEXPAND | wxALL, 10);

    // Add two labels and a button for choosing the contact for this meeting
    wxBoxSizer *contactSizer = new wxBoxSizer(wxHORIZONTAL);
    dataSizer->Add(new wxStaticText(this, 0, "Contact (Optional):", wxDefaultPosition, wxDefaultSize), labelFlags);
    contactName = new wxStaticText(this, 0, "none", wxDefaultPosition, wxDefaultSize);
    wxFont font = contactName->GetFont();
    font.MakeItalic();
    contactName->SetFont(font);
    contactSizer->Add(contactName, 1, wxEXPAND | wxALL, 10);
    wxButton *contactButton = new wxButton(this, ID_MainButton, "Choose Contact", wxPoint(0, 15), wxSize(100, 25), wxBU_EXACTFIT);
    font = contactButton->GetFont();
    font.MakeSmaller();
    contactButton->SetFont(font);
    contactSizer->Add(contactButton, wxSizerFlags(0).Center().Border(wxLEFT, 15));
    dataSizer->Add(contactSizer, wxSizerFlags(0));

    // Create everything needed to enter the start and end time for this meeting
    wxTextValidator validator(wxFILTER_DIGITS);

    // Start with a simple label
    dataSizer->Add(new wxStaticText(this, 0, "Start Time:", wxDefaultPosition, wxDefaultSize), labelFlags);
    wxBoxSizer *startTimeSizer = new wxBoxSizer(wxHORIZONTAL);
    // Then add two empty text fields that can hold up to two digits with a colon in between them
    meetingTime[0] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTime[0]->SetMaxLength(2);
    startTimeSizer->Add(meetingTime[0], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
    startTimeSizer->Add(new wxStaticText(this, 0, ":", wxDefaultPosition, wxDefaultSize), wxSizerFlags(0).Center().Border(wxLEFT, 1));
    meetingTime[1] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTime[1]->SetMaxLength(2);
    startTimeSizer->Add(meetingTime[1], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));
    // Add the AM/PM buttons
    wxToggleButton *startAM = new wxToggleButton(this, ID_ToggleButton, "AM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    startAM->SetValue(true);
    startTimeSizer->Add(startAM, wxSizerFlags(0).Center().Border(wxLEFT, 10));
    meetingAM_PM[0] = startAM;
    wxToggleButton *startPM = new wxToggleButton(this, ID_ToggleButton, "PM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    startTimeSizer->Add(startPM, wxSizerFlags(0).Center());
    meetingAM_PM[1] = startPM;
    dataSizer->Add(startTimeSizer, wxSizerFlags(0));

    // Repeat with the end time by starting with a label
    dataSizer->Add(new wxStaticText(this, 0, "End Time:", wxDefaultPosition, wxDefaultSize), labelFlags);
    wxBoxSizer *endTimeSizer = new wxBoxSizer(wxHORIZONTAL);
    // Adding the two text fields with a colon between them
    meetingTime[2] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTime[2]->SetMaxLength(2);
    endTimeSizer->Add(meetingTime[2], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
    endTimeSizer->Add(new wxStaticText(this, 0, ":", wxDefaultPosition, wxDefaultSize), wxSizerFlags(0).Center().Border(wxLEFT, 1));
    meetingTime[3] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTime[3]->SetMaxLength(2);
    endTimeSizer->Add(meetingTime[3], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));
    // And adding the AM/PM buttons
    wxToggleButton *endAM = new wxToggleButton(this, ID_ToggleButton, "AM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    endAM->SetValue(true);
    endTimeSizer->Add(endAM, wxSizerFlags(0).Center().Border(wxLEFT, 10));
    meetingAM_PM[2] = endAM;
    wxToggleButton *endPM = new wxToggleButton(this, ID_ToggleButton, "PM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    endTimeSizer->Add(endPM, wxSizerFlags(0).Center());
    meetingAM_PM[3] = endPM;
    dataSizer->Add(endTimeSizer, wxSizerFlags(0));

    // Add all of these data entry fields to the top sizer
    topSizer->Add(dataSizer, wxSizerFlags(0).Center());

    // Create the checkbox to indicate a recurring meeting and add it to the top sizer
    wxCheckBox *recurringCheckBox = new wxCheckBox(this, ID_ToggleCheckBox, "This is a recurring meeting");
    topSizer->Add(recurringCheckBox, wxSizerFlags(0).Center().Border(wxUP, 10));

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

    topSizer->Add(recurringDaysSizer, wxSizerFlags(0).Center().Border(wxLEFT | wxRIGHT | wxUP, 15));
    topSizer->Hide(recurringDaysSizer);
    topSizer->Layout();

    // Add the buttons at the bottom of the window and add them to the top sizer
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), wxSizerFlags(0).Border(wxALL, 10));
    buttonSizer->Add(new wxButton(this, wxID_OK, "Create"), wxSizerFlags(0).Border(wxALL, 10));
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border(wxUP, 10));
    
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
    if (nameTxt->GetLineLength(0) == 0 || linkTxt->GetLineLength(0) == 0)
    {
        wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "One or more required fields are missing data.\nPlease fill out the fields and try again.", "",
                wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

        if (warningDialog->ShowModal())
            return;
    }

    //*****************************************************************
    // To-do: Validate that the entered times are logically possible. *
    //*****************************************************************

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

        meeting = new Meeting(nameTxt->GetValue().ToStdString(), linkTxt->GetValue().ToStdString(), _recurringDays);
    }
    else
    {
        meeting = new Meeting(nameTxt->GetValue().ToStdString(), linkTxt->GetValue().ToStdString());
    }

    //****************************************************************************************************************
    // To-do: Add meeting to database (this might not need an object made here, but it's proof of concept, at least) *
    //****************************************************************************************************************
    delete(meeting);

    Close(true);
}

void MV_Create::OnChooseContact(wxCommandEvent& event)
{
    // Rather than being created here, the next line should call the user data class to be given the list of contacts
    // and then simply add the "New Contact" and "No Contact" options at the beginning.
    wxString contacts[5] = { wxString("New Contact"), wxString("No Contact"), wxString("Person A"), wxString("Person B"), wxString("Person C") };

    wxSingleChoiceDialog *contactDialog = new wxSingleChoiceDialog(this, "Select the contact", "", 5, contacts);
    if (contactDialog->ShowModal() == wxID_OK)
    {
        if (contactDialog->GetSelection() == 0)
        {
            // Time for dialogs within dialogs! Ask the user to enter the name of the new contact.
            wxTextEntryDialog *newContactDialog = new wxTextEntryDialog(this, "Enter the name of the new contact.", "");
            newContactDialog->SetMaxLength(30);
            if (newContactDialog->ShowModal() == wxID_OK)
            {
                //**************************************************
                // To-do: This should create a new contact object. *
                //**************************************************
                contactName->SetLabel(newContactDialog->GetValue());
            }
        }
        else if (contactDialog->GetSelection() == 1)
        {
            //********************************************************************
            // To-do: This should remove the contact object from various things. *
            //********************************************************************
            contactName->SetLabel("none");
        }
        else
        {
            //************************************************************************
            // To-do: This should assign an actual contact object to various things. *
            //************************************************************************
            contactName->SetLabel(contacts[contactDialog->GetSelection()]);
        }
    }

    // Fix the layout of the sizers to adapt to the shown/hidden list of checkboxes (must be called after every adjustment)
    topSizer->Layout();
}

void MV_Create::OnRecurring(wxCommandEvent& event)
{
    if (event.IsChecked())
    {
        recurring = true;
        topSizer->Show((size_t) 2);
        SetSize(wxDefaultCoord, wxDefaultCoord, 460, 380);
    }
    else
    {
        recurring = false;
        topSizer->Hide((size_t) 2);
        SetSize(wxDefaultCoord, wxDefaultCoord, 460, 350);
    }

    // Fix the layout of the sizers to adapt to the shown/hidden list of checkboxes (must be called after every adjustment)
    topSizer->Layout();
}

// When any of the AM/PM buttons are toggled, make sure the its pair is also toggled so that only one is selected at a time
void MV_Create::ToggleAM_PM(wxCommandEvent& event)
{
    if (meetingAM_PM[0]->GetValue() + meetingAM_PM[1]->GetValue() != 1)
    {
        isStartAM = !isStartAM;
        meetingAM_PM[0]->SetValue(isStartAM);
        meetingAM_PM[1]->SetValue(!isStartAM);
    }
    else
    {
        isEndAM = !isEndAM;
        meetingAM_PM[2]->SetValue(isEndAM);
        meetingAM_PM[3]->SetValue(!isEndAM);
    }
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
    if (event.CanVeto() && (nameTxt->GetLineLength(0) > 0 || linkTxt->GetLineLength(0) > 0))
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

    for (int i = 0; i < 4; i++)
    {
        meetingAM_PM[i] = nullptr;
    }

    Destroy();
}

wxBEGIN_EVENT_TABLE(MV_Create, wxFrame)
    EVT_BUTTON(wxID_CANCEL, MV_Create::OnCancel)
    EVT_BUTTON(wxID_OK, MV_Create::OnCreate)
    EVT_BUTTON(ID_MainButton, MV_Create::OnChooseContact)
    EVT_TOGGLEBUTTON(ID_ToggleButton, MV_Create::ToggleAM_PM)
    EVT_CHECKBOX(ID_ToggleCheckBox, MV_Create::OnRecurring)
    EVT_CLOSE(MV_Create::OnClosed)
wxEND_EVENT_TABLE()