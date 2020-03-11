/*
 * Crypto.cpp
 *
 *  Created on: March 6, 2020
 *      Author: Gopi
 */

#include "Crypto.h"
#include <crypt.h>



namespace ssms {

static const std::string hashValidChars {
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

  //std::string salt = get the salt of the user
  std::string hash = genHash(salt, password);


  return true;
}

std::string Crypto::genPassword(const std::string &password) {

  std::string salt = passTheSalt();

  return genHash(salt, password);

}

 std::string Crypto::passTheSalt() {
  int j = 0;
  const int saltSize = 16;
  unsigned char saltData[saltSize] = "";
  RAND_bytes(saltData, saltSize);

  std::string saltstring = "$6$";

  for (j = 0; j < saltSize; j++) {
    saltstring += hashValidChars[saltData[j] % hashValidChars.size()];
  }
  return saltstring + "$";
}

std::string Crypto::genHash(const std::string &salt,
                            const std::string &rawPwd) {
  struct crypt_data buf;
  buf.initialized = 0;
  
  char *encPwd =
      crypt_r(rawPwd.c_str(), salt.c_str(), &buf);
  std::string s(encPwd);
  std::cout<< s << std::endl; //for UT debugging purposes
  return s;
}
} 
