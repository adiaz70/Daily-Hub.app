// HubFrame.h
// MS: 3/14/21 - initial code

#ifndef HUB_FRAME_H
#define HUB_FRAME_H

#include <wx/wx.h>
#include "enum_FrameType"

class HubFrame: public wxFrame
{
public:
    HubFrame(const wxString& title, const int _id, const wxPoint& pos, const wxSize& size)
            : wxFrame(NULL, id, title, pos, size) { id = _id; }

    void CloseFrame() { Close(true); }
    void Forget() { forgotten = true; }
    int GetID() { return id; }

    // This pure virtual function means that HubFrame is abstract and *cannot* be instantiated. There has to be
    // a derived class that overrides this function.
    virtual FrameType GetFrameType() = 0;

protected:
    bool forgotten = false;

private:
    int id; // this is almost certainly not necessary, but for some reason my brain can't figure out how to access the wxFrame ID
};

#endif