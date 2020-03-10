#include <iostream>

#include "Cli.h"

namespace ssms {
void Cli::start(void) {
}

Cli::Cli() {
}

Cli::~Cli() {
}

void Cli::processIo() {
}

void Cli::printHelpText() {
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
