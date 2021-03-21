// MV_Create.cpp -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 3/21/21

// Resources:
// https://docs.wxwidgets.org/3.0/overview_sizer.html
// https://docs.wxwidgets.org/3.0/classwx_flex_grid_sizer.html
// https://docs.wxwidgets.org/3.0/classwx_sizer_flags.html
// https://docs.wxwidgets.org/3.0/classwx_text_ctrl.html

#include "MV_Create.h"

//***************************
// Public member functions. *
//***************************

MV_Create::MV_Create(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - New Meeting", id, pos, size, false)
{
    hub = _hub;

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxFlexGridSizer *dataSizer = new wxFlexGridSizer(2, 2, 0, 5);
    // Add(wxSizer *sizer, const wxSizerFlags &flags)
    dataSizer->Add(new wxStaticText(this, GetID(), "Meeting Name:", wxDefaultPosition, wxDefaultSize, 0, ""), wxSizerFlags(0).Center().Right().Border());

    wxTextCtrl *meetingTxt = new wxTextCtrl(this, GetID(), "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP);
    meetingTxt->SetMaxLength(35);
    // Add(wxSizer *sizer, int proportion=0, int flag=0, int border=0, wxObject *userData=NULL)
    dataSizer->Add(meetingTxt, 1, wxEXPAND | wxALL, 10);

    dataSizer->Add(new wxStaticText(this, GetID(), "Meeting Link:", wxDefaultPosition, wxDefaultSize, 0, ""), wxSizerFlags(0).Center().Right().Border());
    dataSizer->Add(new wxTextCtrl(this, GetID(), "", wxDefaultPosition, wxSize(300, 25), wxTE_DONTWRAP), 1, wxEXPAND | wxALL, 10);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), wxSizerFlags(0).Border(wxALL, 10));
    buttonSizer->Add(new wxButton(this, wxID_OK, "Create"), wxSizerFlags(0).Border(wxALL, 10));

    topSizer->Add(dataSizer, wxSizerFlags(0).Center());
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

void MV_Create::OnCancel(wxCommandEvent& event)
{
    Close(true);
}

// This is called by "Close(true)" and when the frame closes for literally any other reason
// (such as pressing X on the window)
void MV_Create::OnClosed(wxCloseEvent& event)
{
    // Make sure the app has forgotten this frame before destroying it
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

wxBEGIN_EVENT_TABLE(MV_Create, wxFrame)
    EVT_BUTTON(wxID_CANCEL, MV_Create::OnCancel)
    EVT_BUTTON(wxID_OK, MV_Create::OnCancel)
    EVT_CLOSE(MV_Create::OnClosed)
wxEND_EVENT_TABLE()