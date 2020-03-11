/*
 * Crypto.h
 *
 *  Created on: March 06, 2020
 *      Author: Gopi
 */

#ifndef SSMS_CRYPTO_H
#define SSMS_CRYPTO_H

#include <cstring>
#include <iostream>
#include <openssl/rand.h>
#include <string>

#include "User.h"

namespace ssms {
class Crypto
{
public:
  std::string encrypt(const std::string &message, const std::string &publickey);
  std::string decrypt(const std::string &message, const std::string &privatekey);
  static bool validatePassword(const User& user, const std::string &password);
  static std::string genHash(const std::string &salt, const std::string &rawPwd);
  static std::string passTheSalt();
  static std::string genPassword(const std::string &password);
};
}; // namespace ssms

#endif
