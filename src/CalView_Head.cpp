// CalView_Head.cpp 
// Done by: Alyssa  Diaz

// g++ *.cpp *.h `wx-config --cxxflags --libs` -o test

#include "CalView_Head.h"
// Calendar  View:: DHub
CalView_Head::CalView_Head(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _Dhub)

         : HubFrame("Daily Hub - Calendar", id, pos, size)
{


   hub= _Dhub;
//Setting Up Menu
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenTempHome, "&Open Home");
    menuFile->Append(ID_OpenCalViewHead, "&Open New Calendar Window");
    menuFile->Append(ID_CloseFrame, "&Close Window");
    menuFile->Append(wxID_EXIT, "&Quit");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);
}
//Establishing FrameType
FrameType CalView_Head::GetFrameType()
{
    return FrameType::CalView_Head;
}
//Opening & Closing Cal View
void CalView_Head::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

void CalView_Head::OnOpenCalView_Head(wxCommandEvent& event)
{
    hub->OpenFrame(FrameType::CalView_Head);
}

void CalView_Head::OnExit(wxCommandEvent& event)
{
    close(true);
}

void CalView_Head::OnClosed(wxCloseEvent& event)
{
    if (!forgotten)
       hub->ForgetFrame(this);
    Destroy();
}

void CalView_Head::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}
//Event Table  
wxBEGIN_EVENT_TABLE(CalView_Head, wxFrame)
    EVT_MENU(ID_OpenTempHome, CalView_Head::OnOpenHome)
    EVT_MENU(ID_OpenCVHead, CalView_Head::OnOpenCalView_Head)
    EVT_MENU(ID_CloseFrame, CalView_Head::OnExit)
    EVT_MENU(wxID_EXIT, CalView_Head::OnQuit)
    EVT_CLOSE(CalView_Head::OnClosed)
wxEND_EVENT_TABLE() 
