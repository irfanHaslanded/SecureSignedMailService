#ifndef SSMS_CLI_H_
#define SSMS_CLI_H_

#include "User.h"

#include <list>

namespace ssms{
class Cli
{
  public:
    static void start(void);

  private:
    Cli();
    virtual ~Cli();
    void processIo();
    void printHelpText();
    std::string getPrompt();
    bool createUser(User &user);
    bool deleteUser(User &user);
    bool logIn(std::string username, std::string password);
    std::map <std::string, User> users;
    User *loggedInUser;
};
}
#endif

