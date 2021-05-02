// MV_Create.cpp -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 4/20/21

// Resources:
// https://docs.wxwidgets.org/3.0/overview_sizer.html
// https://docs.wxwidgets.org/3.0/classwx_flex_grid_sizer.html
// https://docs.wxwidgets.org/3.0/classwx_sizer_flags.html
// https://docs.wxwidgets.org/3.0/classwx_text_ctrl.html

#include "MV_Create.h"
#include "Meeting.h"
#include "MeetingTime.h"
#include "UserData.h"
#include <vector>

//***************************
// Public member functions. *
//***************************

MV_Create::MV_Create(const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame("Daily Hub - New Meeting", id, pos, wxSize(460, 405), false)
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
    meetingTimeEntries[0] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTimeEntries[0]->SetMaxLength(2);
    startTimeSizer->Add(meetingTimeEntries[0], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
    startTimeSizer->Add(new wxStaticText(this, 0, ":", wxDefaultPosition, wxDefaultSize), wxSizerFlags(0).Center().Border(wxLEFT, 1));
    meetingTimeEntries[1] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTimeEntries[1]->SetMaxLength(2);
    startTimeSizer->Add(meetingTimeEntries[1], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));
    // Add the AM/PM buttons
    meetingAM_PM[0] = new wxToggleButton(this, ID_ToggleButton, "AM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    meetingAM_PM[0]->SetValue(true);
    startTimeSizer->Add(meetingAM_PM[0], wxSizerFlags(0).Center().Border(wxLEFT, 10));
    meetingAM_PM[1] = new wxToggleButton(this, ID_ToggleButton, "PM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    startTimeSizer->Add(meetingAM_PM[1], wxSizerFlags(0).Center());
    dataSizer->Add(startTimeSizer, wxSizerFlags(0));

    // Repeat with the end time by starting with a label
    dataSizer->Add(new wxStaticText(this, 0, "End Time:", wxDefaultPosition, wxDefaultSize), labelFlags);
    wxBoxSizer *endTimeSizer = new wxBoxSizer(wxHORIZONTAL);
    // Adding the two text fields with a colon between them
    meetingTimeEntries[2] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTimeEntries[2]->SetMaxLength(2);
    endTimeSizer->Add(meetingTimeEntries[2], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
    endTimeSizer->Add(new wxStaticText(this, 0, ":", wxDefaultPosition, wxDefaultSize), wxSizerFlags(0).Center().Border(wxLEFT, 1));
    meetingTimeEntries[3] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
    meetingTimeEntries[3]->SetMaxLength(2);
    endTimeSizer->Add(meetingTimeEntries[3], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));
    // And adding the AM/PM buttons
    meetingAM_PM[2] = new wxToggleButton(this, ID_ToggleButton, "AM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    meetingAM_PM[2]->SetValue(true);
    endTimeSizer->Add(meetingAM_PM[2], wxSizerFlags(0).Center().Border(wxLEFT, 10));
    meetingAM_PM[3] = new wxToggleButton(this, ID_ToggleButton, "PM", wxPoint(0, 15), wxSize(40, 25), wxBU_EXACTFIT);
    endTimeSizer->Add(meetingAM_PM[3], wxSizerFlags(0).Center());
    dataSizer->Add(endTimeSizer, wxSizerFlags(0));

    // Add all of these data entry fields to the top sizer
    topSizer->Add(dataSizer, wxSizerFlags(0).Center());

    // Create the checkbox to indicate a recurring meeting and add it to the top sizer
    wxCheckBox *recurringCheckBox = new wxCheckBox(this, ID_ToggleCheckBox, "This is a recurring meeting");
    topSizer->Add(recurringCheckBox, wxSizerFlags(0).Center().Border(wxUP, 10));

    // Set up a list of checkboxes for every day of the week and add each one to an array that can be accessed later
    wxBoxSizer *recurringDaysSizer = new wxBoxSizer(wxHORIZONTAL);
    wxString days[7] = { wxString("Mon"), wxString("Tue"), wxString("Wed"), wxString("Thur"), wxString("Fri"), wxString("Sat"), wxString("Sun") };
    for (int i = 0; i < 7; i++)
    {
        recurringDays[i] = new wxCheckBox(this, 0, days[i]);
        recurringDaysSizer->Add(recurringDays[i], wxSizerFlags(0).Border(wxRIGHT, 5));
    }

    topSizer->Add(recurringDaysSizer, wxSizerFlags(0).Center().Border(wxLEFT | wxRIGHT | wxUP, 15));
    topSizer->Hide(recurringDaysSizer);
    topSizer->Layout();

    // Add a little empty space to help with the window layout
    topSizer->Add(20, 5);

    // Create the text entry fields for entering the date of a meeting that does not recur
    wxBoxSizer *singleDateSizer = new wxBoxSizer(wxHORIZONTAL);
    singleDateSizer->Add(new wxStaticText(this, 0, "Meeting Date:"), labelFlags);
    singleDateSizer->Add(5, 5);
    for (int i = 0; i < 3; i++)
    {
        singleDateEntries[i] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
        singleDateEntries[i]->SetMaxLength(2);
        if (i == 0)
            singleDateSizer->Add(singleDateEntries[i], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
        else
            singleDateSizer->Add(singleDateEntries[i], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));

        if (i != 2)
            singleDateSizer->Add(new wxStaticText(this, 0, "/"), wxSizerFlags(0).Center().Border(wxLEFT | wxRIGHT, 2));
    }
    topSizer->Add(singleDateSizer, wxSizerFlags(0).Border(wxLEFT, 35));

    // Also create the text entry fields for the start and end dates of a recurring meeting
    // Start with the start date
    wxBoxSizer *startDateSizer = new wxBoxSizer(wxHORIZONTAL);
    startDateSizer->Add(new wxStaticText(this, 0, "Start Date:"), labelFlags);
    startDateSizer->Add(5, 5);
    for (int i = 0; i < 3; i++)
    {
        startDateEntries[i] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
        startDateEntries[i]->SetMaxLength(2);
        if (i == 0)
            startDateSizer->Add(startDateEntries[i], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
        else
            startDateSizer->Add(startDateEntries[i], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));

        if (i != 2)
            startDateSizer->Add(new wxStaticText(this, 0, "/"), wxSizerFlags(0).Center().Border(wxLEFT | wxRIGHT, 2));
    }
    topSizer->Add(startDateSizer, wxSizerFlags(0).Border(wxLEFT, 55));
    // Then repeat with the end date
    wxBoxSizer *endDateSizer = new wxBoxSizer(wxHORIZONTAL);
    endDateSizer->Add(new wxStaticText(this, 0, "End Date:"), labelFlags);
    endDateSizer->Add(5, 5);
    for (int i = 0; i < 3; i++)
    {
        endDateEntries[i] = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(35, 25), wxTE_DONTWRAP, validator);
        endDateEntries[i]->SetMaxLength(2);
        if (i == 0)
            endDateSizer->Add(endDateEntries[i], wxSizerFlags(0).Center().Border(wxUP | wxLEFT | wxDOWN, 10));
        else
            endDateSizer->Add(endDateEntries[i], wxSizerFlags(0).Center().Border(wxUP | wxDOWN, 10));

        if (i != 2)
            endDateSizer->Add(new wxStaticText(this, 0, "/"), wxSizerFlags(0).Center().Border(wxLEFT | wxRIGHT, 2));
    }
    topSizer->Add(endDateSizer, wxSizerFlags(0).Border(wxLEFT, 65));
    // Then hide both of them until the meeting is marked as recurring
    topSizer->Hide(startDateSizer);
    topSizer->Hide(endDateSizer);
    topSizer->Layout();

    // Add the buttons at the bottom of the window and add them to the top sizer
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), wxSizerFlags(0).Border(wxLEFT | wxDOWN | wxRIGHT, 10));
    buttonSizer->Add(new wxButton(this, wxID_OK, "Create"), wxSizerFlags(0).Border(wxLEFT | wxDOWN | wxRIGHT, 10));
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
    //*******************************************************************************************************************
    // Before the meeting can be created, run a series of checks on the data and notify the user if something is wrong. *
    //*******************************************************************************************************************

    // Confirm that the meeting name has been entered
    if (nameTxt->GetLineLength(0) == 0)
    {
        wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "Please enter the name of the meeting.", "", wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

        if (warningDialog->ShowModal())
            return;
    }

    // Confirm that the start and end time for the meeting has been entered and is valid
    MeetingTime *meetingTime;
    if (meetingTimeEntries[0]->GetLineLength(0) != 0 && meetingTimeEntries[1]->GetLineLength(0) != 0 &&
        meetingTimeEntries[2]->GetLineLength(0) != 0 && meetingTimeEntries[3]->GetLineLength(0) != 0)
    {
        int times[4];
        for (int i = 0; i < 4; i++)
        {
            times[i] = std::stoi(meetingTimeEntries[i]->GetValue().ToStdString());
        }

        meetingTime = new MeetingTime(times, isStartAM, isEndAM);

        if (!meetingTime->IsValidTime())
        {
            wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "The start or end time is not valid.\nPlease review and try again.", "",
                wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

            if (warningDialog->ShowModal())
            {
                delete(meetingTime);
                return;
            }
        }
    }
    else
    {
        wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "Please enter the start and end time.", "", wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

        if (warningDialog->ShowModal())
            return;
    }

    // The remainder of items that need to be validated depend on whether or not the meeting is recurring
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

        // Then, check that the start and end dates have been entered in full
        int startDate[3];
        int endDate[3];
        for (int i = 0; i < 3; i++)
        {
            if (startDateEntries[i]->GetLineLength(0) == 0 || endDateEntries[i]->GetLineLength(0) == 0)
            {
                wxMessageDialog *warningDialog = new wxMessageDialog(this,
                    "Please enter a start and end date for this meeting.", "", wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

                if (warningDialog->ShowModal())
                    return;
            }

            startDate[i] = std::stoi(startDateEntries[i]->GetValue().ToStdString());
            endDate[i] = std::stoi(endDateEntries[i]->GetValue().ToStdString());
        }

        // Check that both of these dates are valid and occur in the proper order
        if (!IsValidDate(startDate) || !IsValidDate(endDate) || 
            startDate[2] > endDate[2] || (startDate[0] > endDate[0] && startDate[2] == endDate[2]) ||
            (startDate[1] >= endDate[1] && startDate[0] == endDate[0] && startDate[2] == endDate[2]))
        {
            wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "The start or end date is invalid.\nPlease review and try again.", "",
                wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

            if (warningDialog->ShowModal())
                return;
        }

        meeting = new Meeting(nameTxt->GetValue().ToStdString(), linkTxt->GetValue().ToStdString(),
                              contactName->GetLabel().ToStdString() != "none" ? contactName->GetLabel().ToStdString() : "",
                              meetingTime, startDate, endDate, _recurringDays);
    }
    else
    {
        // Validate that the single date for the meeting is fully entered, and warn the user if not
        int date[3];
        for (int i = 0; i < 3; i++)
        {
            if (singleDateEntries[i]->GetLineLength(0) == 0)
            {
                wxMessageDialog *warningDialog = new wxMessageDialog(this,
                    "Please enter a date for this meeting.", "", wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

                if (warningDialog->ShowModal())
                    return;
            }

            date[i] = std::stoi(singleDateEntries[i]->GetValue().ToStdString());
        }

        // Also check that the date is a valid one, and warn the user if not
        if (!IsValidDate(date))
        {
            wxMessageDialog *warningDialog = new wxMessageDialog(this,
                "The meeting date is invalid.\nPlease review and try again.", "",
                wxICON_EXCLAMATION | wxOK, wxDefaultPosition);

            if (warningDialog->ShowModal())
                return;
        }

        meeting = new Meeting(nameTxt->GetValue().ToStdString(), linkTxt->GetValue().ToStdString(),
                              contactName->GetLabel().ToStdString() != "none" ? contactName->GetLabel().ToStdString() : "", 
                              meetingTime, date);
    }

    UserData::AddMeeting(meeting);
    //delete(meetingTime); // (this should be covered in the meeting destructor)
    delete(meeting);

    Close(true);
}

void MV_Create::OnChooseContact(wxCommandEvent& event)
{
    std::vector<std::string> contactStrings = UserData::GetContacts();
    wxString contacts[contactStrings.size() + 2];
    contacts[0] = wxString("No Contact");
    contacts[1] = wxString("New Contact");
    for (int i = 0; i < contactStrings.size(); i++)
    {
        contacts[i + 2] = wxString(contactStrings[i]);
    }

    wxSingleChoiceDialog *contactDialog = new wxSingleChoiceDialog(this, "Select the contact", "", contactStrings.size() + 2, contacts);
    if (contactDialog->ShowModal() == wxID_OK)
    {
        if (contactDialog->GetSelection() == 1)
        {
            // Time for dialogs within dialogs! Ask the user to enter the name of the new contact.
            wxTextEntryDialog *newContactDialog = new wxTextEntryDialog(this, "Enter the name of the new contact.", "");
            newContactDialog->SetMaxLength(30);
            if (newContactDialog->ShowModal() == wxID_OK)
            {
                if (newContactDialog->GetValue().ToStdString() != "none")
                {
                    UserData::AddContact(newContactDialog->GetValue().ToStdString());
                    contactName->SetLabel(newContactDialog->GetValue());
                }
            }
        }
        else if (contactDialog->GetSelection() == 0)
            contactName->SetLabel("none");
        else
            contactName->SetLabel(contacts[contactDialog->GetSelection()]);
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
        topSizer->Hide((size_t) 4);
        topSizer->Show((size_t) 5);
        topSizer->Show((size_t) 6);
        SetSize(wxDefaultCoord, wxDefaultCoord, 460, 485);
    }
    else
    {
        recurring = false;
        topSizer->Hide((size_t) 2);
        topSizer->Show((size_t) 4);
        topSizer->Hide((size_t) 5);
        topSizer->Hide((size_t) 6);
        SetSize(wxDefaultCoord, wxDefaultCoord, 460, 405);
    }

    // Fix the layout of the sizers to adapt to the shown/hidden list of checkboxes (must be called after every adjustment)
    topSizer->Layout();
}

// When any of the AM/PM buttons are toggled, make sure the its pair is also toggled so that only one is selected at a time
void MV_Create::ToggleAM_PM(wxCommandEvent& event)
{
    if (meetingAM_PM[0]->GetValue() + meetingAM_PM[1]->GetValue() != 1)
    {
        // Only alter the button states if the clicked toggle button was previously unselected
        // Otherwise, revert them to their previous states
        if (event.IsChecked())
            isStartAM = !isStartAM;

        meetingAM_PM[0]->SetValue(isStartAM);
        meetingAM_PM[1]->SetValue(!isStartAM);
    }
    else
    {
        // Only alter the button states if the clicked toggle button was previously unselected
        // Otherwise, revert them to their previous states
        if (event.IsChecked())
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

// Assuming MM/DD/YY format, because America, y'know?
bool MV_Create::IsValidDate(int date[3])
{
    // I'm not going to worry right now about validating the date beyond these general number ranges.
    // Are there a number of months with fewer days than 31? Yes, for sure. But this is close enough to
    // avoid really weird inputs, at least. Also, make sure the date isn't in a past year, just because.
    if (date[0] <= 12 && date[0] > 0 && date[1] <= 31 && date[1] > 0 && date[2] >= 21)
        return true;
    else
        return false;
}

wxBEGIN_EVENT_TABLE(MV_Create, wxFrame)
    EVT_BUTTON(wxID_CANCEL, MV_Create::OnCancel)
    EVT_BUTTON(wxID_OK, MV_Create::OnCreate)
    EVT_BUTTON(ID_MainButton, MV_Create::OnChooseContact)
    EVT_TOGGLEBUTTON(ID_ToggleButton, MV_Create::ToggleAM_PM)
    EVT_CHECKBOX(ID_ToggleCheckBox, MV_Create::OnRecurring)
    EVT_CLOSE(MV_Create::OnClosed)
wxEND_EVENT_TABLE()