// MV_Create.cpp -- the frame for creating a new meeting
// Maintained by: Marcus Schmidt
// Created on 3/20/21
// Last edited on 3/20/21

#include "MV_Create.h"

//***************************
// Public member functions. *
//***************************

MV_Create::MV_Create(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - New Meeting", id, pos, size)
{
    hub = _hub;

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(375, 350), wxTE_MULTILINE), 1, wxEXPAND | wxALL, 10);

    wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
    button_sizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 10);//wxSizerFlags(0).Align().Border(wxALL, 10));
    button_sizer->Add(new wxButton(this, wxID_OK, "Create"), 0, wxALL, 10);//wxSizerFlags(0).Align().Border(wxALL, 10));

    sizer->Add(button_sizer, wxSizerFlags(0).Center() );
    
    SetSizerAndFit(sizer);

    // Source: https://docs.wxwidgets.org/3.0/overview_sizer.html
    /*topsizer->Add(new wxTextCtrl( this, -1, "My text.", wxDefaultPosition, wxSize(100,60), wxTE_MULTILINE),
        1,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        10 );         // set border width to 10*/

    // Other resource(s):
    // https://wiki.wxwidgets.org/Writing_Your_First_Application-Using_The_WxTextCtrl
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