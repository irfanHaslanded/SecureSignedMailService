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
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string>

#define RSA_KEYLEN 2048

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2

namespace ssms {
class Crypto
{
public:
  static std::string encrypt(const std::string &message, const std::string &publickey);
  static std::string decrypt(const std::string &message, const std::string &privatekey);
  static bool validatePassword(const std::string &hash, const std::string &password);
  static std::string genHash(const std::string &salt, const std::string &rawPwd);
  static std::string passTheSalt();
  static std::string getSalt(const std::string &hash);
  static std::string genPassword(const std::string &password);

  /*Functions for rsa encryption and decryption*/
  static int rsaEncrypt(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage, unsigned char **encryptedKey,
      size_t *encryptedKeyLength, unsigned char **iv, size_t *ivLength);

  static int rsaDecrypt(const unsigned char *encryptedMessage, size_t encryptedMessageLength,const unsigned char *encryptedKey, size_t encryptedKeyLength,
      unsigned char *iv, size_t ivLength, unsigned char **decryptedMessage);

  static int getLocalPublicKey(unsigned char **publicKey);
  static int getLocalPrivateKey(unsigned char **privateKey);

private:
  static EVP_PKEY *localKeypair;
  static int messageLength;
  static unsigned char *encryptedKey;
  static size_t encryptedKeyLength;

  static EVP_CIPHER_CTX *rsaEncryptContext;
  static EVP_CIPHER_CTX *rsaDecryptContext;

  static int init();
  void free();
  static int generateRsaKeypair(EVP_PKEY **keypair);
  static  int bioToString(BIO *bio, unsigned char **string);



};
}; // namespace ssms

#endif
