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

void Cli::processIo() {

  std::cout << Cli::getPrompt();
  char input[100];

  std::cin.getline(input, sizeof(input));

  interpretInput(input);
}

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
      std::cout << "help"<< std::endl;
      std::cout << "?"<< std::endl;
      std::cout << "List Users"<< std::endl;
      std::cout << "Send Message"<< std::endl;
      std::cout << "Show Inbox"<< std::endl;
      std::cout << "Log Out"<< std::endl;
      std::cout << "Exit"<< std::endl;
    }

  }

  void Cli::printWelcomeScreen() {

  std::cout << " =============\nWelcome to SSMS\n ============= \n" << std::endl;
  std::cout <<"Created by Team Coral 2020" << std::endl;
  std::cout <<"Irfan, Gopi, Karthik, Naief, Oskar, Tamas \n\n" << std::endl;
  std::cout <<"Type command, Help or ? for help \n" << std::endl;
  }


std::string Cli::getPrompt() {
  std::string tmpPrompt;

  if(cli->loggedInUser){
    tmpPrompt = "(" + cli->loggedInUser->toString() + ")";
  }

  tmpPrompt += '>';

  return tmpPrompt;
}

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
// Might modify it. Create a function that can be use in both delete and login.
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
    std::cout<<"Please enter your password:"<< std::endl;
    std::cin>>password;
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

void Cli::logOut()
{
  cli->setMenuNotLoggedInUser();
  cli->loggedInUser = nullptr;
}

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


  void Cli::listUsers()
  {
    auto userList = cli->loggedInUser->getIdList();

    std::cout<<"Registered Users:" << std::endl;
    std::copy(userList.begin(), userList.end(),
              std::ostream_iterator<std::string>(std::cout,"\n"));
  };
  void Cli::sendMessage(){
    //Todo functions to read input.
    //cli->loggedInUser->
  };
  void Cli::showInbox(){
    auto sizeInbox = cli->loggedInUser->showInbox(); //todo understand this function.
    //todo: nice print function.
  };

  void Cli::quit()
  {
    run = false;
    std::cout << "Exit." << std::endl;
  }

// TODO: description, nicer way of handling similar inputs (h, help etc).
// other set of inputs for logged in and not logged in users, maybe separate class
// fix exit function
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


  void Cli::setMenuLoggedInUser()
  {
    menu_map initMap;
    cli->menuItems = initMap;
    cli->menuItems.emplace("help", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("?", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("listusers", std::bind(&Cli::listUsers, cli));
    cli->menuItems.emplace("sendmessage", std::bind(&Cli::sendMessage, cli));
    cli->menuItems.emplace("showinbox", std::bind(&Cli::showInbox, cli));
    cli->menuItems.emplace("logout", std::bind(&Cli::logOut, cli));
    cli->menuItems.emplace("exit", std::bind(&Cli::quit, cli)); //maybe not for logged in?
    cli->menuItems.emplace("quit", std::bind(&Cli::quit, cli));
  };
}
