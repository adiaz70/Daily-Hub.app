#include <iostream>
#include <string>

class Test
{
public:
    static std::string GetTestString() { return testString; }
private:
    static std::string GetUserName()
    {
        char *userName;
        cuserid(userName);
        return userName; // returns char* for a std::string return type
    }
    static const std::string testString;
};

std::string const Test::testString = "test w/ user '" + Test::GetUserName() + "'";

int main()
{
    std::cout << "Test string is: " << Test::GetTestString() << "\n";

    return 0;
}
