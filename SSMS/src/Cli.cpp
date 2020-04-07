#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <termios.h>

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

}

void Cli::printHelpText() {
  // print help text
	int input=0;
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

bool Cli::createUser()
{
	std::string userId, password;
	// Just to take input from user
	std::cout<<"Enter your User Id:";
	std::cin>>userId;
	password = Cli::inputPassword("Please enter the password: ");

	User* user = new User(userId);
	user->setPassword(password);
	std::cout<<"User Created"<<std::endl;

	return true;
}

bool Cli::deleteUser() {

  return true;
}

bool Cli::logIn()
{
	std::string input;
	// Just to take input from user
	std::cout<<"Enter your username:"<< std::endl;
	std::cin>>input;
	try
	{
			User currentUser(input);
	}
	catch(UserAlreadyExists& e)
	{
		std::cout<<"User already exists"<<std::endl;
	}
//	if()
//	{
//		cli->loggedInUser = &tmpUser;
//    	  std::cout<< Cli::getPrompt();
//
//	}

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

}
