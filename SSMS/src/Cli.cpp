#include <iostream>

#include "Cli.h"

namespace ssms {

static Cli *cli = NULL;

void Cli::start(void) {

  cli = new Cli();
  cli->setMenuNotLoggedInUser();

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
  // Just to take input from user
 std::cin.getline(input, sizeof(input));
  User tmpUser(input);
  cli->loggedInUser = &tmpUser;

  interpretInput(input);
}

void Cli::printHelpText() {
  // print help text
	int input;
	std::cout<<"Please select your option:"<<std::endl;
	std::cout<<"1- Create User"<<std::endl;
	std::cout<<"2- Delete User"<<std::endl;
	std::cout<<"3- Log In "<<std::endl;
  //std::cin>>input;
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
	// Just to take input from user
	std::cout<<"Enter your User Id:";
	std::cin>>userId;
	std::cout<<"Enter your password:";
	std::cin>>password;
	User* user = new User(userId);
	user->setPassword(password);
	std::cout<<"User Created";
	return true;
}

bool Cli::deleteUser() {

  return true;
}

bool Cli::logIn() {
	char input[100];
	// Just to take input from user
	std::cout<<"Enter your username:"<< std::endl;
	std::cin.getline(input, sizeof(input));
	User tmpUser(input);
	cli->loggedInUser = &tmpUser;

	return true;
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


  void Cli::setMenuNotLoggedInUser()
  {
    menu_map initMap;
    cli->menuItems = initMap;
    cli->menuItems.emplace("Help", std::bind(&Cli::printHelpText, cli));
    cli->menuItems.emplace("Create User", std::bind(&Cli::createUser, cli));
    cli->menuItems.emplace("Delete User", std::bind(&Cli::deleteUser, cli));
    cli->menuItems.emplace("Log in", std::bind(&Cli::logIn, cli));
  };
}
