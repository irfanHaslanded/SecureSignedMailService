/*
 * Crypto.h
 *
 *  Created on: March 06, 2020
 *      Author: Gopi
 */

#ifndef SSMS_CRYPTO_H
#define SSMS_CRYPTO_H

#include <string>
#include <iostream>

namespace ssms
{

class Crypto
{
public:
  /*Constructor*/
  Crypto();
  /*Destructor*/
  virtual ~Crypto();

private:
  void encrypt(std::string message);
  void decrypt(std::string message);
  void validatePassword(std::string password);

};
};

#endif
