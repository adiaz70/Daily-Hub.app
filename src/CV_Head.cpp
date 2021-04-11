// CV_Head.cpp 
// Maintained by: Paula Rodriguez
// 3/16/21

// g++ *.cpp *.h `wx-config --cxxflags --libs` -o test

#include "CV_Head.h"
#include <wx/splitter.h>
#include <wx/sizer.h>

CV_Head::CV_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
        : HubFrame("Daily Hub - Contacts", id, pos, size)
{
    hub = _hub;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenTempHome, "&Open Home");
    menuFile->Append(ID_OpenCVHead, "&Open New Contacts Window");
    menuFile->Append(ID_CloseFrame, "&Close Window");
    menuFile->Append(wxID_EXIT, "&Quit");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);         /*create a main sizer*/
    wxSplitterWindow *splittermain = new wxSplitterWindow(this,wxID_ANY);
    mainsizer->Add(splittermain, 1, wxLEFT|wxEXPAND, 5);
       
    wxPanel *panelContact = new wxPanel(splittermain,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL|wxBORDER);   /*Create a sizer for contacts*/
    wxStaticBoxSizer *conSizer = new wxStaticBoxSizer(wxHORIZONTAL,panelContact,_T("Contacts"));

    conSizer->SetMinSize(550,-1);
    wxTextCtrl *txt1 = new wxTextCtrl( panelContact, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    conSizer->Add(txt1,1,wxALL|wxEXPAND,5);
    panelContact->SetSizer(conSizer);
       
    wxPanel *panelInfo = new wxPanel(splittermain,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL|wxBORDER);   /*create a sizer for contact infor*/
    wxStaticBoxSizer *infoSizer = new wxStaticBoxSizer(wxHORIZONTAL,panelInfo,_T("Contact Info"));

    infoSizer->SetMinSize(550,-1);
    wxTextCtrl *txt2 = new wxTextCtrl( panelInfo, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    infoSizer->Add(txt2,1,wxALL|wxEXPAND,5);
    panelInfo->SetSizer(infoSizer);
       
    splittermain->SetSashGravity(0.25);     /*This changes how much 'space' the contacts sizer will take*/
    splittermain->SplitVertically(panelContact, panelInfo);        /*put both sizers in the main sizer*/
       
    SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);
}

FrameType CV_Head::GetFrameType()
{
    return FrameType::CVHead;
}

void CV_Head::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

void CV_Head::OnOpenCVHead(wxCommandEvent& event)
{
    hub->OpenFrame(FrameType::CVHead);  //open unsplit window
}

void CV_Head::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void CV_Head::OnClosed(wxCloseEvent& event)
{
    if (!forgotten)
        hub->ForgetFrame(this);
    Destroy();
}

void CV_Head::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}

wxBEGIN_EVENT_TABLE(CV_Head, wxFrame)
    EVT_MENU(ID_OpenTempHome, CV_Head::OnOpenHome)
    EVT_MENU(ID_OpenCVHead, CV_Head::OnOpenCVHead)
    EVT_MENU(ID_CloseFrame, CV_Head::OnExit)
    EVT_MENU(wxID_EXIT, CV_Head::OnQuit)
    EVT_CLOSE(CV_Head::OnClosed)
wxEND_EVENT_TABLE()