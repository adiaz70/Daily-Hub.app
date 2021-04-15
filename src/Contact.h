// Contact.h
// MS: 4/2/21 - initial code
// MS: 4/7/21 - changed private variable to be wxString for ease of use elsewhere in the program
// MS: 4/15/21 - added another function that does return a std::string for the SQLite code to use

#ifndef CONTACT_H
#define CONTACT_H

#include <wx/string.h>
#include <string>

class Contact
{
public:
    Contact(std::string _name) { name = wxString(_name); }
    Contact(wxString _name) { name = _name; }
    wxString GetName() { return name; }
    std::string GetNameString() { return name.ToStdString(); }

private:
    wxString name;
};

#endif