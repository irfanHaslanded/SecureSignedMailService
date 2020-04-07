#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <termios.h>

#include "Cli.h"

namespace ssms {

static Cli *cli = NULL;

void Cli::start(void) {

  cli = new Cli();
  cli->setMenuNotLoggedInUser();
  cli->printWelcomeScreen();

  while(true) {
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
  std::cout << "------------------" <<std::endl;
  std::cout <<"Help" << std::endl;
  std::cout <<"Create User" << std::endl;
  std::cout <<"Delete User" << std::endl;
  std::cout <<"Log in" << std::endl;
  std::cout <<"Exit" << std::endl;

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

bool Cli::deleteUser() {

  return true;
}

bool Cli::logIn()
{
  //First
  //check if ANY user exist -> if not, tell the user to create one.
  /*
      if(no users)
      cout<< No existing users, create one first!
  */

	std::string input;

	std::cout<<"Enter your username:"<< std::endl;
	std::cin>>input;

  // Here we should:
  // check if user name exist in the list of users ; userMap_.emplace(input, cli).second
  // or with userMap.at()

  // if so require password,
  // then set current user to that user.

  // As for now make a dummy User and have it as active user.

  std::string dummyUserName = "banQuo";
  static std::shared_ptr<User> dummyUser = nullptr;
  if (!dummyUser) {
    dummyUser = User::create(dummyUserName);
  }
  //TODO: should point at the user in the user map.
  cli->loggedInUser = dummyUser;

  return true;
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
  std::cout<<"You entered:"<< password; //Remove in final version. Only for demo
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

  if ( cli->menuItems.find(input) == cli->menuItems.end() ) {
      std::cout<< input << std::endl;
      std::cout << "Invalid input. type help or ? for a list of commands." << std::endl;
      return;
  } else {
    cli->menuItems.at(input)();
  }

 };

// TODO: description, nicer way of handling similar inputs (h, help etc).
// other set of inputs for logged in and not logged in users, maybe separate class
// fix exit function
  void Cli::setMenuNotLoggedInUser()
  {
    menu_map initMap;
    cli->menuItems = initMap;
    cli->menuItems.emplace("Help", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("help", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("?", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("Create User", std::bind(&Cli::createUser, cli));
    cli->menuItems.emplace("Delete User", std::bind(&Cli::deleteUser, cli));
    cli->menuItems.emplace("Log in", std::bind(&Cli::logIn, cli));
    cli->menuItems.emplace("Exit", std::bind(&Cli::logIn, cli));
  };
}
