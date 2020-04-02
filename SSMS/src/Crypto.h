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

namespace ssms {

enum class KeyAccess { PRIVATE, PUBLIC };

struct EvpPkeyFree {
  void operator()(EVP_PKEY* evp_pkey) { EVP_PKEY_free(evp_pkey); }
};

using EvpPkeyUniquePtr = std::unique_ptr<EVP_PKEY, EvpPkeyFree>;

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
  static bool generateRsaKeypair(std::string& private_key, std::string& public_key);

private:
  static bool generateRsaKeypair(EVP_PKEY **keypair);
  static std::string extractKey(EVP_PKEY* keypair, KeyAccess key_access);
  static EvpPkeyUniquePtr compileKey(const std::string& key_str, KeyAccess key_access);
  static int bioToString(BIO *bio, unsigned char **string);

private:
  static const int RSA_KEYLEN;
  static const int saltSize;
  static const int saltIdSize;
};
} // namespace ssms

#endif
