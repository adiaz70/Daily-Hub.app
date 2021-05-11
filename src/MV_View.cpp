// MV_View.cpp -- the frame where an individual Meeting can be viewed and notes can be taken
// Maintained by: Marcus Schmidt
// Created on 4/12/21
// Last edited on 5/11/21

#include "MV_View.h"
#include "wx/hyperlink.h"
#include "UserData.h"
#include "Date.h"

//***************************
// Public member functions. *
//***************************

MV_View::MV_View(Meeting *meeting, const int id, const wxPoint& pos, DailyHub* _hub)
        : HubFrame(wxString(meeting->GetName() + ((meeting->GetContact().length() != 0) ? " - " + meeting->GetContact() : "")),
        id, pos, wxDefaultSize)
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

    wxFlexGridSizer *infoSizer = new wxFlexGridSizer(4, 2, 15, 0);

    // Add the meeting link information
    infoSizer->Add(new wxStaticText(this, 0, "Meeting Link:"), wxSizerFlags(0).Border(wxLEFT | wxUP, 15));
    if (meeting->GetLink().length() != 0)
    {
        std::string linkLabel = meeting->GetLink();
        if (linkLabel.length() > 40)
            linkLabel = linkLabel.substr(0, 37) + "...";

        infoSizer->Add(new wxHyperlinkCtrl(this, 0, wxString(linkLabel), wxString(meeting->GetLink())),
                        wxSizerFlags(0).Border(wxLEFT | wxUP, 5));
    }
    else
    {
        wxStaticText *noLinkLabel = new wxStaticText(this, 0, "none");
        wxFont font = noLinkLabel->GetFont();
        font.MakeItalic();
        noLinkLabel->SetFont(font);
        infoSizer->Add(noLinkLabel, wxSizerFlags(0).Border(wxLEFT | wxUP, 15));
    }

    // Add the meeting time information
    infoSizer->Add(new wxStaticText(this, 0, "Meeting Time:"), wxSizerFlags(0).Border(wxLEFT, 15));
    int *timeArray = meeting->GetMeetingTime()->GetTimes();
    std::string time = std::to_string(timeArray[0]) + ":";
    if (timeArray[1] < 10)
        time += "0";
    time += std::to_string(timeArray[1]);
    if (meeting->GetMeetingTime()->IsStartAM())
        time += " AM   to   ";
    else
        time += " PM   to   ";
    time += std::to_string(timeArray[2]) + ":";
    if (timeArray[3] < 10)
        time += "0";
    time += std::to_string(timeArray[3]);
    if (meeting->GetMeetingTime()->IsEndAM())
        time += " AM";
    else
        time += " PM";
    infoSizer->Add(new wxStaticText(this, 0, wxString(time)), wxSizerFlags(0).Border(wxLEFT, 15));

    // Add the date range information (if the meeting is recurring)
    int *dateArray;
    std::string formattedDate;
    if (meeting->IsRecurring())
    {
        //wxBoxSizer *secondDateSizer = new wxBoxSizer(wxHORIZONTAL);
        infoSizer->Add(new wxStaticText(this, 0, "Date Range:"), wxSizerFlags(0).Border(wxLEFT, 15));
        dateArray = meeting->GetFirstDate();
        formattedDate = std::to_string(dateArray[0]) + "/" + std::to_string(dateArray[1]) + "/" + std::to_string(dateArray[2]);
        dateArray = meeting->GetSecondDate();
        formattedDate += "   to   " + std::to_string(dateArray[0]) + "/" + std::to_string(dateArray[1]) + "/" + std::to_string(dateArray[2]);
        infoSizer->Add(new wxStaticText(this, 0, wxString(formattedDate)), wxSizerFlags(0).Border(wxLEFT, 15));
    }

    //wxBoxSizer *firstDateSizer = new wxBoxSizer(wxHORIZONTAL);
    infoSizer->Add(new wxStaticText(this, 0, "Meeting Date:"), wxSizerFlags(0).Border(wxLEFT, 15));

    // Add all of the relevant info for date of this current meeting (if one-time, it's just the scheduled date)
    // (if a recurring meeting, it could be any instance of the meeting).
    wxBoxSizer *selectedDateSizer = new wxBoxSizer(wxHORIZONTAL);
    // If the meeting is recurring, it needs buttons on either side of the label to change the current one to the next or previous instance
    if (meeting->IsRecurring())
    {
        wxButton *previousButton = new wxButton(this, ID_MainButton, "Previous", wxDefaultPosition, wxSize(60, 25), wxBU_EXACTFIT);
        wxFont font = previousButton->GetFont();
        font.MakeSmaller();
        previousButton->SetFont(font);
        selectedDateSizer->Add(previousButton, wxSizerFlags(0).Center());
    }
    // Regardless, add the formatted date of this instance
    // Do this by creating a copy of the meeting's first date in newly allocated memory that can be updated later
    currentDate = (int *) malloc(3 * sizeof(int));
    int *firstDate = meeting->GetFirstDate();
    currentDate[0] = firstDate[0];
    currentDate[1] = firstDate[1];
    currentDate[2] = firstDate[2];
    formattedDate = std::to_string(currentDate[0]) + "/" + std::to_string(currentDate[1]) + "/" + std::to_string(currentDate[2]);
    meetingDate = new wxStaticText(this, 0, wxString(formattedDate));
    selectedDateSizer->Add(meetingDate, wxSizerFlags(0).Border(wxLEFT, 15));
    // Then add the second button on the other side of the label
    if (meeting->IsRecurring())
    {
        wxButton *nextButton = new wxButton(this, ID_SecondaryButton, "Next", wxDefaultPosition, wxSize(50, 25), wxBU_EXACTFIT);
        wxFont font = nextButton->GetFont();
        font.MakeSmaller();
        nextButton->SetFont(font);
        selectedDateSizer->Add(nextButton, wxSizerFlags(0).Center().Border(wxLEFT, 15));
    }
    // And add everything to the sizer grid
    infoSizer->Add(selectedDateSizer, wxSizerFlags(0).Border(wxLEFT, 15));

    topSizer->Add(infoSizer, wxSizerFlags(0).Center());

    notes = new wxTextCtrl(this, 0, UserData::GetNotes(meetingID), wxDefaultPosition, wxSize(300, 150), wxTE_MULTILINE);
    topSizer->Add(notes, wxSizerFlags(1).Border(wxALL, 15).Expand());

    SetSizerAndFit(topSizer);

    // Finally, if this is a recurring meeting, build a shift table for the date based on which days of the week it needs to occur
    bool *recurringDays = meeting->GetRecurringDays();
    for (int i = 0; i < 7; i++)
    {
        // If this day of the week is not one that the meeting is meant to recur on, then there is no shift
        if (!recurringDays[i])
            dateShift[i] = 0;
        // If this *is* a day of the week that the meeting is meant to recur
        else
        {
            // Set the shift to a full week to start with
            dateShift[i] = 7;
            // Then, look through all of the remaining days of the week (looping around to the start of the week as needed)
            for (int j = 1; j < 6; j++)
            {
                // And check if for another day that the meeting is set to occur on
                if (recurringDays[(i + j) % 7])
                {
                    // If one is found, set the shift to equal the difference between them and break the loop
                    dateShift[i] = j;
                    break;
                }
            }
        }
    }

    //****************************************************************************************************
    // To-do: make sure that this current date is on one of the days actually designated for the meeting *
    //****************************************************************************************************
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

void MV_View::OnOpenMVHead(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::MVHead);
}

void MV_View::OnPreviousMeeting(wxCommandEvent& event)
{
    // Need to have another table for backward shift...
}

void MV_View::OnNextMeeting(wxCommandEvent& event)
{
    int *newDate = Date::ShiftDate(currentDate, dateShift[Date::DayOfWeek(currentDate)]);
    free(currentDate);
    currentDate = newDate;

    //***************************************************************************
    // To-do: check that this new date does not exceed the meeting's date range *
    //***************************************************************************

    std::string dateString = std::to_string(currentDate[0]) + "/" + std::to_string(currentDate[1]) + "/" + std::to_string(currentDate[2]);
    meetingDate->SetLabel(wxString(dateString));
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
    EVT_MENU(ID_OpenMVHead, MV_View::OnOpenMVHead)
    EVT_MENU(ID_CloseFrame, MV_View::OnExit)
    EVT_MENU(wxID_EXIT, MV_View::OnQuit)
    EVT_BUTTON(ID_MainButton, MV_View::OnPreviousMeeting)
    EVT_BUTTON(ID_SecondaryButton, MV_View::OnNextMeeting)
    EVT_CLOSE(MV_View::OnClosed)
wxEND_EVENT_TABLE()