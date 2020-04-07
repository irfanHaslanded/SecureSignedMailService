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
  std::cout<< Cli::getPrompt();

  std::string input;
  // Just to take input from user
  std::cout<<"Enter your username:"<< std::endl;
  std::cin>>input;
  User tmpUser(input);
  cli->loggedInUser = &tmpUser;

}

void Cli::printHelpText() {
  // print help text
}

std::string Cli::getPrompt() {
  std::string tmpPrompt;

  if(cli->loggedInUser){
    tmpPrompt = "(" + cli->loggedInUser->toString() + ")";
  }

  tmpPrompt += '>';

  return tmpPrompt;
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
