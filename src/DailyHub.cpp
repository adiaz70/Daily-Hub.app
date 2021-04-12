// DailyHub.cpp
// MS: 3/14/21 - initial code
// MS: 3/20/21 - incorporated new frame, MV_Create

#include "DailyHub.h"
#include "TempHomeFrame.h"
#include "MV_Head.h"
#include "CV_Head.h"
#include "MV_Create.h"
#include "enum_EventID"

//***************************
// Public member functions  *
//***************************

bool DailyHub::OnInit()
{
    OpenUniqueFrame(FrameType::TempHome);

    return true;
}

/*int DailyHub::OnExit()
{
    // Clean up any objects here--only called after all windows are closed, but before internal wxWidgets cleaning.
    // Note: use "delete x" instead of "free(x)" for objects initialized with 'new' and not 'malloc'.
    // Also note: the instantiated frames do not need to be cleaned here because that memory is already cleaned up
    // when each frame calls "Close(true);".

    return 0; // this return value should be ignored and doesn't matter
}*/

void DailyHub::OpenUniqueFrame(FrameType id)
{
    // Check if a frame of this type already exists and raise it if so
    int match = FindFrame(id);
    if (match != -1)
    {
        frames[match]->Raise();
        return;
    }

    // If no frame of this type already exists, open one
    OpenFrame(id);
}

void DailyHub::OpenFrame(FrameType id)
{
    switch (id)
    {
        case FrameType::TempHome: frames.push_back(new TempHomeFrame(NewFrameID(), wxPoint(50, 50), wxSize(550, 440), this)); break;
        case FrameType::MVHead: frames.push_back(new MV_Head(NewFrameID(), wxPoint(50, 50), wxSize(450, 340), this)); break;
        case FrameType::MVCreate: frames.push_back(new MV_Create(NewFrameID(), wxPoint(50, 50), wxSize(460, 405), this)); break;
        case FrameType::CVHead: frames.push_back(new CV_Head(NewFrameID(), wxPoint(50, 50), wxSize(450, 340), this)); break;
        default: return;
    }

    frames.back()->Show(true);
}

void DailyHub::ForgetFrame(HubFrame* _frame)
{
    for (auto it = begin(frames); it != end(frames); ++it)
    {
        if (_frame->GetID() == (*it)->GetID())
        {
            frames.erase(it);
            break;
        }
    }
}

// MS: 3/21/21 - added code to check for open windows with unsaved data to give user a chance to stop
// https://docs.wxwidgets.org/3.0/classwx_message_dialog.html
void DailyHub::CloseAll()
{
    if (FindFrame(FrameType::MVCreate) != -1)
    {
        wxMessageDialog *quitDialog = new wxMessageDialog(NULL,
                "One or more windows might have unsaved data.\nAre you sure you want to quit?", "",
                wxICON_EXCLAMATION | wxOK | wxCANCEL, wxDefaultPosition);

        // If the user chooses the cancel button, return from the method before any windows are closed
        if (quitDialog->ShowModal() == wxID_CANCEL)
            return;
    }

    while (frames.size() > 0)
    {
        frames.back()->CloseFrame();
        frames.pop_back();
    }
}

// MS: 3/22/21 - added method
int DailyHub::FrameCount()
{
    return frames.size();
}

//****************************
// Private member functions  *
//****************************

int DailyHub::NewFrameID()
{
    static int id = 0;
    return id++;
}

int DailyHub::FindFrame(FrameType id)
{
    for (int i = 0; i < frames.size(); i++)
    {
        // If an empty element is found, skip to the next one
        if (frames[i] == NULL)
            continue;

        // If an element of the correct type is found, return it
        if (frames[i]->GetFrameType() == id)
            return i;
    }

    // If no frame of this type was found, return a -1 to indicate failure
    return -1;
}

wxIMPLEMENT_APP(DailyHub);