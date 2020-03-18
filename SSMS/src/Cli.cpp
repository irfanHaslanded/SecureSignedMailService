#include <iostream>

#include "Cli.h"

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
	std::string input;
	// Just to take input from user
	std::cout<<"Enter your username:"<< std::endl;
	std::cin>>input;

//	std::cout<<"Enter your password:";
//	std::cin>>users;

}

void Cli::printHelpText() {
	// print help text
}

std::string Cli::getPrompt() {
  return std::string();
}

bool Cli::createUser(User& user) {
  return true;
}

bool Cli::deleteUser(User& user) {
  return true;
}

bool Cli::logIn(std::string username, std::string password) {
  return true;
}
}
