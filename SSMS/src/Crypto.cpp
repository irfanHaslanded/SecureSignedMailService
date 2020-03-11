/*
 * Crypto.cpp
 *
 *  Created on: March 6, 2020
 *      Author: Gopi
 */

#include "Crypto.h"
using namespace ssms;

static const std::string hashValidChars[65] = {
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"};

std::string Crypto::encrypt(const std::string &message,
                            const std::string &publickey) {
  return std::string();
}

std::string Crypto::decrypt(const std::string &message,
                            const std::string &privatekey) {
  return std::string();
}

bool Crypto::validatePassword(const User &user, const std::string &password) {
  return true;
}

void Crypto::passTheSalt(std::string &saltstring) {
  char j = 0;
  unsigned char saltData[16] = "";
  char SALT_STRING[] = {"$6$................$"};

  size_t saltStringLen = strnlen(SALT_STRING, 21);
  size_t saltSize = saltStringLen - 4;

  RAND_bytes(saltData, saltSize);

  saltstring = "$6$";

  for (j = 0; j < saltSize; j++) {
    saltstring += hashValidChars[saltData[j] & 0x3fu];
  }

  saltstring += "$";
}
