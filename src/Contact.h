// Contact.h
// MS: 4/2/21 - initial code
// MS: 4/7/21 - changed private variable to be wxString for ease of use elsewhere in the program

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

private:
    wxString name;
};

#endif