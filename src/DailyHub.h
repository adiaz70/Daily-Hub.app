// DailyHub.h
// MS: 3/14/21 - initial code

#ifndef DAILY_HUB_H
#define DAILY_HUB_H

#include <wx/wx.h>
#include <vector>
#include "HubFrame.h"
#include "enum_FrameType"

class DailyHub: public wxApp
{
public:
    virtual bool OnInit();
    //virtual int OnExit();

    void OpenUniqueFrame(FrameType id); // Opens a new instance of a frame if one does not already exist
    void OpenFrame(FrameType id, void *data = nullptr); // Opens a new instance of a frame regardless of whether one already exists
    void ForgetFrame(HubFrame* _frame);
    void CloseAll();

    // MS: 3/22/21 - added method
    int FrameCount();

private:
    int NewFrameID(); // Returns unique integer ID for every frame created
    int FindFrame(FrameType id); // Returns the index of the first frame of a certain type in the frames vector, or -1 if none exist
    std::vector<HubFrame*> frames;
};

#endif