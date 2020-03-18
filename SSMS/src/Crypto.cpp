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
  return message;
}

std::string Crypto::decrypt(const std::string &message,
                            const std::string &privatekey) {
  return message;
}

bool Crypto::validatePassword(const std::string &hash, const std::string &password)
{
  return hash == genHash(Crypto::getSalt(hash), password);

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

std::string Crypto::getSalt(const std::string &hash)
{
  return "TODO";
}

} 
