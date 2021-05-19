/***************************************************************
 * Name:      CalendarMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Alyssa Diaz ()
 * Last Modification:   2021-05-16
 * Copyright: Alyssa Diaz ()
 * License:
 **************************************************************/
 
 // g++ *.cpp *.h `wx-config --cxxflags --libs` -o test
/*
//#include<DailyHub.cpp>
#include "CalendarMain.h"
#include <wx/msgdlg.h>

// Calendar  View:: DailyHub
CalendarMain::CalendarMain(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub)
         : HubFrame("Daily Hub - Calendar", id, pos, size)
{

   hub= _hub;

//(*InternalHeaders(Daily_HubFrame)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(Daily_HubFrame)
const long CalendarMain::ID_CALENDARCTRL1 = wxNewId();
const long CalendarMain::idMenuQuit = wxNewId();
const long CalendarMain::idMenuAbout = wxNewId();
const long CalendarMain::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CalendarMain,wxFrame)
    //(*EventTable(Daily_HubFrame)
    //*)
    wxBEGIN_EVENT_TABLE(CalendarMain, wxFrame)
    EVT_MENU(ID_OpenTempHome, CalendarMain::OnOpenHome)
    EVT_MENU(ID_OpenCalendarMain, CalendarMain::OnOpenCalendarMain)
    EVT_MENU(ID_OpenCreateCalendar, CalendarMain::OnCreate)
    EVT_MENU(ID_CloseFrame, CalendarMain::OnExit)
    EVT_MENU(wxID_EXIT, CalendarMain::OnQuit)
    EVT_CLOSE(CalendarMain::OnClosed)
wxEND_EVENT_TABLE()
END_EVENT_TABLE()

//CalendarMain::CalendarMain(wxWindow *parent,wxWindowID id)
CalendarMain::CalendarMain(wxWindow* parent,wxWindowID id = -1);
{
    //(*Initialize(CalendarMain)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;


    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    CalendarCtrl1 = new wxCalendarCtrl(this, ID_CALENDARCTRL1, wxDefaultDateTime, wxPoint(160,72), wxDefaultSize, wxCAL_SUNDAY_FIRST, _T("ID_CALENDARCTRL1"));
    CalendarCtrl1->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    CalendarCtrl1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_CALENDARCTRL1,wxEVT_CALENDAR_SEL_CHANGED,(wxObjectEventFunction)&CalendarMain::OnCalendarCtrl1Changed);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CalendarMain::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CalendarMain::OnAbout);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&CalendarMain::OnClose);
    //*)
}

CalendarMain::~CalendarMain()
//FrameType CalendarMain::GetFrameType()
{
    //(*Destroy(Daily_HubFrame)
    //*)
    //return FrameType::CalendarMain;
}



void CalendarMain::OnOpenHome(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::TempHome);
}

void CalendarMain::OnOpenCalendarMain(wxCommandEvent& event)
{
    hub->OpenFrame(FrameType::CalendarMain);  //open unsplit window
}

void CalendarMain::OnCreate(wxCommandEvent& event)
{
    hub->OpenUniqueFrame(FrameType::CreateCalendar);
}

void CalendarMain::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void CalendarMain::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void CalendarMain::OnCalendarCtrl1Changed(wxCalendarEvent& event)
{
}

void CalendarMain::OnClosed(wxCloseEvent& event)
{
if(!forgotten)
	hub->ForgetFrame(this);
Destroy();
}
void CalendarMain::OnQuit(wxCommandEvent& event)
{
    hub->CloseAll();
}*/
