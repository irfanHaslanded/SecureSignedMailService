/*
*         Cli.cpp
* Main user interface for this SSMS application.
* Runs an infinite loop waiting for user input,
* the cli has a menu (wip) that is dynamic in the sense that
* it changes depending on user privileges. By typing keywords
* in the commandline the menu matches the input to the corresponing
* function to be executed.
*
* Created by Team Coral 2020
*/


#include <iostream>
#include <iterator>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <termios.h>

#include "Cli.h"

namespace ssms {

static Cli *cli = NULL;
static bool run = true;

/*
* Cli::start
* Main entry point, called from main.
* Initializes a cli object, with menu in default state
* and prints welcome screen/info message.
*
*/
void Cli::start(void) {

  cli = new Cli();
  cli->setMenuNotLoggedInUser();
  cli->printWelcomeScreen();

  while(run) {
    cli->processIo();
  }
}

Cli::Cli() {
}

Cli::~Cli() {
}

/*
* Cli::processIo
*
* Main idle state. Wait for user input and
* when received interpret input.
* getline used instead of cin >> to not break if
* there are spaces present in the input.
*/
void Cli::processIo() {

  std::cout << Cli::getPrompt();
  char input[100];

  std::cin.getline(input, sizeof(input));

  interpretInput(input);
}

/*
* Cli::printHelpText
*
* Prints currently available commands, depending
* on user role and program state. (WIP)
*/

void Cli::printHelpText() {

  std::cout << "Available commands:" <<std::endl;
    //not logged in
    if(cli->loggedInUser == nullptr) {
      std::cout << "------------------" <<std::endl;
      std::cout <<"Help" << std::endl;
      std::cout <<"Create User" << std::endl;
      std::cout <<"Delete User" << std::endl;
      std::cout <<"Log in" << std::endl;
      std::cout <<"Exit" << std::endl;
    }
    //User logged in, could be done nicer.
    else if(cli->loggedInUser != nullptr)
    {
      std::cout << "Help"<< std::endl;
      std::cout << "List Users"<< std::endl;
      std::cout << "Send Message"<< std::endl;
      std::cout << "Show Inbox"<< std::endl;
      std::cout << "Show Outbox"<< std::endl;
      std::cout << "Log Out"<< std::endl;
      std::cout << "Exit"<< std::endl;
    }

  }


/*
* Cli::printWelcomeScreen
*
* Prints welcome screen (called once at startup)
*/
  void Cli::printWelcomeScreen() {

  std::cout << " =============\nWelcome to SSMS\n ============= \n" << std::endl;
  std::cout <<"Created by Team Coral 2020" << std::endl;
  std::cout <<"Irfan, Gopi, Karthik, Naief, Oskar, Tamas \n\n" << std::endl;
  std::cout <<"Type command, Help or ? for help \n" << std::endl;
  }

/*
* Cli::getPrompt
* Returns a string with the current prompt.
* as of now it simply adds the username in the case
* where a user is logged in.
*/
std::string Cli::getPrompt() {
  std::string tmpPrompt;

  if(cli->loggedInUser){
    tmpPrompt = "(" + cli->loggedInUser->toString() + ")";
  }

  tmpPrompt += '>';

  return tmpPrompt;
}

/*
* Cli::createUSer
*
* Creates a user (duh) by calling the User class,
* the users are identified by a unique ID, and added
* to the main list of users, also stored in the User class.
*/
bool Cli::createUser() {
  std::string userId, password;

  std::cout<<"Enter your User Id:";
  std::cin>>userId;
  password = Cli::inputPassword("Please enter the password: ");

  try {
    auto user = User::create(userId);
    user->setPassword(password);
    std::cout<<"User Created"<<std::endl;
    return true;
  } catch (std::exception& e) {
    std::cout<<e.what()<<std::endl;
    return false;
  }
}

/*
* Cli::createUSer
*
* Deletes a user (spoiler!) by calling the corresponding function in
* the User class,.
* Deleting a user simply means removing the object from the list of
* users in the User class, freeing up that user ID.
*/
bool Cli::deleteUser() {

  std::string userId;
  std::string password;

  std::cout<<"Enter userId to delete:"<< std::endl;
  std::cin>>userId;
  //check if user exist.
  if (User::get(userId) == nullptr)
  {
    std::cout<<"User doesnt exist."<< std::endl;
  }
  else
  {
    //user already exists
    auto user = User::get(userId);
    std::cout<<"Please enter password:"<< std::endl;
    std::cin>>password;
    if(user->checkPassword(password))
    {
      user->remove(userId);
      std::cout<<"User Deleted"<< std::endl;
    }
    else
    {
      std::cout<<"Wrong Password. Cant delete user."<< std::endl;
    }

  }
  return true;
}


/*
* Cli::logIn
*
* Opens the log-in screen to log in as a particular
* (existing/pre-created) user.
*/

bool Cli::logIn()
{
  std::string userId;
  std::string password;

  std::cout<<"Enter your userId:"<< std::endl;
  std::cin>>userId;
  //check that user exist.
  if (User::get(userId) == nullptr)
  {
    std::cout<<"User doesnt exist."<< std::endl;
    return false;
  }
  else
  {
    auto user = User::get(userId);
    password = Cli::inputPassword("Please enter the password: ");
    if(user->checkPassword(password))
    {
      cli->loggedInUser = user;
      cli->setMenuLoggedInUser();
      return true;
    }
    else
    {
      std::cout<<"Wrong Password."<< std::endl;
      return false;
    }

  }

}

/*
* Cli::logOut
*
* Logs out the current user,
* and switches the menu to the appropriate state.
*/
void Cli::logOut()
{
  cli->setMenuNotLoggedInUser();
  cli->loggedInUser = nullptr;
}

/*
* Cli::inputPassword
* Special function to mask input when typing passwords
*
*/

std::string Cli::inputPassword(const char *prompt)
{
  const char BACKSPACE=127;
  const char RETURN=10;

  std::string password;
  unsigned char ch=0;

  std::cout<<prompt<<std::endl;
  std::cout<<">>";
  ch=getch();
  while((ch=getch())!=RETURN)
  {
     if(ch==BACKSPACE)
     {
        if(password.length()!=0)
        {
          std::cout<<"\b \b";
          password.resize(password.length()-1);
        }
     }
     else
     {
       password+=ch;
       std::cout<<'*';
     }
  }
  std::cout<<std::endl;

  return password;
}

/* Cli::getch
* Get char function for the purposes of this application
* TODO: Naief please elaborate on this functionality.
*/


int Cli::getch()
{
  int ch;
  struct termios t_old, t_new;

  tcgetattr(STDIN_FILENO, &t_old);
  t_new = t_old;
  t_new.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
  return ch;
}

/*
*  Cli::interpretInput
*  Takes input from the main loop via processIo().
*  This function formats and matches the input vs the given keys
*  (which are tied to commands/functions) in the menuItems map.
*
*  If valid command in current state -> execute command
*  if not -> print invalid input msg
*/

 void Cli::interpretInput(std::string input)
 {
    //Make input lowercase and remove spaces, purely for easier handling
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());

    if ( cli->menuItems.find(input) == cli->menuItems.end() ) {
        std::cout<< input << std::endl;
        std::cout << "Invalid input. type help or ? for a list of commands." << std::endl;
        return;
    } else {
      cli->menuItems.at(input)();
    }
 };

/*
*   Cli::listUsers
*   Prints all existing users by accessing the list of users from
*   the User class.
*/

  void Cli::listUsers()
  {
    auto userList = cli->loggedInUser->getIdList();

    std::cout<<"Registered Users:" << std::endl;
    std::copy(userList.begin(), userList.end(),
              std::ostream_iterator<std::string>(std::cout,"\n"));
  };


/*
*   Cli::sendMessage
*   (logged in users only) creates and sends a message to a
*   given recipient. Both are inputed via command line
*/

  void Cli::sendMessage()
  {
    std::string recipient_id;
    std::cout<<"To: ";
    std::cout.flush();
    std::cin >> recipient_id;

    const char message_terminator = '$';
    std::string text_message;
    std::cout<<"Message (enter '" << message_terminator << "' to close the message):"<< std::endl;
    std::getline(std::cin, text_message, message_terminator);
    std::cout<< recipient_id << ": " << text_message << std::endl;

    if (cli->loggedInUser->sendMessage(recipient_id, text_message)) {
      std::cout << "Message sent" << std::endl;
    } else {
      std::cout << "No such user" << std::endl;
    }
  }

/*
*   Cli::showInbox
*   (logged in users only). Displays all received messages for the current user,
*   and who has sent it.
*/

  void Cli::showInbox(){
    auto sizeInbox = cli->loggedInUser->showInbox();
  };

/*
*   Cli::showSentMessage
*   (logged in users only). Displays all sent messages for the current user,
*   and to whom it was sent.
*/
  void Cli::showSentMessages(){
    auto sizeInbox = cli->loggedInUser->showOutbox();
  };


/* Cli::quit
* Breaks the main loop in CLI. As for now this
* makes us return to main and terminate.
*/

  void Cli::quit()
  {
    run = false;
    std::cout << "Exit." << std::endl;
  }

// TODO:
// Make separate class with objects containing function/key pairs
// as well as unique help messages. Makes it easier to maintain and
// to have users with different priviligies
//  Also helps making separate pages/sub menus

  /*
  *   Cli::setMenuNotLoggedInUser
  *
  *   Sets the available commands for non logged in users.
  *   each input maps to a function call in the cli object
  */
  void Cli::setMenuNotLoggedInUser()
  {
    menu_map initMap;
    cli->menuItems = initMap;
    cli->menuItems.emplace("help", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("?", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("createuser", std::bind(&Cli::createUser, cli));
    cli->menuItems.emplace("deleteuser", std::bind(&Cli::deleteUser, cli));
    cli->menuItems.emplace("login", std::bind(&Cli::logIn, cli));
    cli->menuItems.emplace("exit", std::bind(&Cli::quit, cli));
    cli->menuItems.emplace("quit", std::bind(&Cli::quit, cli));
  };

  /*
  *   Cli::setMenuLoggedInUser
  *
  *   Sets the available commands for logged in users.
  *   each input maps to a function call in the cli object
  */
  void Cli::setMenuLoggedInUser()
  {
    menu_map initMap;
    cli->menuItems = initMap;
    cli->menuItems.emplace("help", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("?", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("listusers", std::bind(&Cli::listUsers, cli));
    cli->menuItems.emplace("sendmessage", std::bind(&Cli::sendMessage, cli));
    cli->menuItems.emplace("showinbox", std::bind(&Cli::showInbox, cli));
    cli->menuItems.emplace("showoutbox", std::bind(&Cli::showSentMessages, cli));
    cli->menuItems.emplace("logout", std::bind(&Cli::logOut, cli));
    cli->menuItems.emplace("exit", std::bind(&Cli::quit, cli)); //maybe not for logged in?
    cli->menuItems.emplace("quit", std::bind(&Cli::quit, cli));
  };
}
