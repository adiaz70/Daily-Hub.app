// Contact.h
// MS: 4/2/21 - initial code

#ifndef CONTACT_H
#define CONTACT_H

#include <string>

class Contact
{
public:
    Contact(std::string _name) { name = _name; }
    std::string GetName() { return name; }

private:
    std::string name;
};

#endif