#include <iostream>

#include "Cli.h"
#include "User.h"

namespace ssms {

static Cli *cli = NULL;

void Cli::start(void) {
  cli = new Cli();
  while(true) {
    cli->processIo();

  }
}

Cli::Cli() {
}

Cli::~Cli() {
}

void Cli::processIo() {
  Cli::printHelpText();
  std::cout<< Cli::getPrompt();





}

void Cli::printHelpText() {
  // print help text
	int input;
	std::cout<<"Please select your option:"<<std::endl;
	std::cout<<"1- Create User"<<std::endl;
	std::cout<<"2- Delete User"<<std::endl;
	std::cout<<"3- Log In "<<std::endl;
    std::cin>>input;
    switch(input)
    {
      case 1:
        Cli::createUser();
        break;
      case 2:
        Cli::deleteUser();
        break;
      case 3:
        Cli::logIn();
        break;
      default:
        std::cout<<"Invalid option.";
        break;
    }

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
	std::string input;
	// Just to take input from user
	std::cout<<"Enter your username:"<< std::endl;
	std::cin>>input;
	User tmpUser(input);
	cli->loggedInUser = &tmpUser;
	if ()

	return true;
}

}
