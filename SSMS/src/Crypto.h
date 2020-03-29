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
#include <memory>

#define RSA_KEYLEN 2048

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2

namespace ssms {

struct CryptoEvpPars
{
  unsigned char encrypted_key[1024]; // EVP_PKEY_size(evp_public_key)

  int encrypted_key_length;
  unsigned char iv[EVP_MAX_IV_LENGTH];
  CryptoEvpPars();
};

class Crypto
{
public:
  static bool encrypt(const std::string &plain_message,
                      const std::string &public_key,
                      std::string& encrypted_message,
                      CryptoEvpPars& evp_pars);
  static bool decrypt(const std::string &encrypted_message,
                      const std::string &private_key,
                      std::string& decrypted_message,
                      const CryptoEvpPars& evp_pars);
  static bool validatePassword(const std::string &hash, const std::string &password);
  static std::string genHash(const std::string &salt, const std::string &rawPwd);
  static std::string passTheSalt();
  static std::string getSalt(const std::string &hash);
  static std::string genPassword(const std::string &password);

  static int getLocalPublicKey(unsigned char **publicKey);
  static int getLocalPrivateKey(unsigned char **privateKey);

  static void generateRsaKeypair(std::string& private_key, std::string& public_key);


private:
  static EVP_PKEY *localKeypair;
  static int messageLength;
  static unsigned char *encryptedKey;
  static size_t encryptedKeyLength;

  static EVP_CIPHER_CTX *rsaEncryptContext;
  static EVP_CIPHER_CTX *rsaDecryptContext;

  static int generateRsaKeypair(EVP_PKEY **keypair);
  static std::string extractKey(EVP_PKEY* keypair, bool isPrivate);
  static EVP_PKEY* compileKey(const std::string& key_str, bool isPrivate);
  static int bioToString(BIO *bio, unsigned char **string);



};
}; // namespace ssms

#endif
