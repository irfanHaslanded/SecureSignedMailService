#ifndef SSMS_CLI_H_
#define SSMS_CLI_H_

#include "User.h"

#include <list>
#include <unordered_map>
#include <functional>

typedef std::unordered_map<std::string, std::function<void()>> menu_map;

namespace ssms{
class Cli
{
  public:
    static void start(void);
    std::string getPrompt();
    std::string setPrompt();
    User *getLoggedInUser();
    void printHelpText();
    void printWelcomeScreen();
    void interpretInput(std::string input);
    void setMenuLoggedInUser();
    void setMenuNotLoggedInUser();
  private:
    Cli();
    virtual ~Cli();

    void processIo();
    bool createUser();
    bool deleteUser();
    bool logIn(); // remove
    int getch();
    std::string inputPassword(const char *prompt);
    bool userLoggedIn;
    std::string propmt;
    std::map <std::string, User> users;
    menu_map menuItems;
    User *loggedInUser;
    static struct option long_options[];
};
}
#endif

