/***************************************************************
 * Name:      CalendarMain.h
 * Purpose:   Defines Application Frame
 * Author:    Alyssa Diaz ()
 * Last Modified:   2021-05-16
 * Copyright: Alyssa Diaz ()
 * License:
 **************************************************************/

#ifndef CALENDARMAIN_H
#define CALENDARMAIN_H

//(*Headers(Daily_HubFrame)
#include <wx/calctrl.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/wx.h>
#include "DailyHub.h"
#include "HubFrame.h"
#include "enum_FrameType"
#include "enum_EventID"
//*)

class CalendarMain: public HubFrame
{
    public:

        CalendarMain(const int id, const wxPoint& pos, const wxSize& size, DailyHub* _hub);
    //FrameType GetFrameType();
    CalendarMain(wxWindow* parent,wxWindowID id = -1);
        virtual ~CalendarMain();

    private:

        //(*Handlers(Daily_HubFrame)
        void OnOpenHome(wxCommandEvent& event);
    	void OnOpenCalendarMain(wxCommandEvent& event);
    	void OnCreate(wxCommandEvent& event);
    	
        void OnAbout(wxCommandEvent& event);
        void OnCalendarCtrl1Changed(wxCalendarEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        DailyHub* hub;
        //*)

        //(*Identifiers(Daily_HubFrame)
        static const long ID_CALENDARCTRL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(Daily_HubFrame)
        wxCalendarCtrl* CalendarCtrl1;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // DAILY_HUBMAIN_H

