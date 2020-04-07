#ifndef SSMS_CLI_H_
#define SSMS_CLI_H_

#include "User.h"

#include <list>

namespace ssms{
class Cli
{
  public:
    static void start(void);
    std::string getPrompt();
    std::string setPrompt();
  private:
    Cli();
    virtual ~Cli();
    void processIo();
    void printHelpText();
    bool createUser();
    bool deleteUser();
    bool logIn(); // remove
    int getch();
    std::string inputPassword(const char *prompt);
    bool userLoggedIn;
    std::string propmt;
    std::map <std::string, User> users;
    User *loggedInUser;
};
}
#endif

