/*
 * Crypto.cpp
 *
 *  Created on: March 6, 2020
 *      Author: Gopi
 */

#include "Crypto.h"
#include "Base64.h"

#include <crypt.h>
#include <stdlib.h>

namespace ssms {

const int Crypto::RSA_KEYLEN = 2048;
const int Crypto::saltSize   = 16;
const int Crypto::saltIdSize = 3;  // eaxmple "$6$"

CryptoEvpPars::CryptoEvpPars() : encrypted_key_length(sizeof(encrypted_key)) {}

static const std::string hashValidChars {
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"};

bool Crypto::validatePassword(const std::string &hash,
                              const std::string &password)
{
  return hash == genHash(Crypto::getSalt(hash), password);
}

std::string Crypto::genPassword(const std::string &password)
{
  std::string salt = passTheSalt();
  std::cout << salt << std::endl; // for UT debugging purposes
  return genHash(salt, password);
}

std::string Crypto::passTheSalt()
{
  int j = 0;
  
  unsigned char saltData[saltSize] = "";
  RAND_bytes(saltData, saltSize);

  std::string saltstring = "$6$";

  for (j = 0; j < saltSize; j++) {
    saltstring += hashValidChars[saltData[j] % hashValidChars.size()];
  }
  return saltstring + "$";
}

std::string Crypto::genHash(const std::string &salt,
                            const std::string &rawPwd)
{
  struct crypt_data buf;
  buf.initialized = 0;
  
  char* encPwd = crypt_r(rawPwd.c_str(), salt.c_str(), &buf);
  std::string s(encPwd);
  std::cout << s << std::endl; //for UT debugging purposes
  return s;
}

std::string Crypto::getSalt(const std::string &hash)
{
  std::string salt(hash.substr(0, saltSize + saltIdSize + 1));
  std::cout << "salt is  : " + salt << std::endl; // for UT debugging purposes
  return salt;
}

bool Crypto::generateRsaKeypair(EVP_PKEY **keypair)
{
  EVP_PKEY_CTX* context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

  if(EVP_PKEY_keygen_init(context) <= 0) {
    return false;
  }

  if(EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0) {
    return false;
  }

  if(EVP_PKEY_keygen(context, keypair) <= 0) {
    return false;
  }

  EVP_PKEY_CTX_free(context);
  return true;
}

bool Crypto::generateRsaKeypair(std::string& private_key, std::string& public_key)
{
  EVP_PKEY* keypair { nullptr };
  if (!generateRsaKeypair(&keypair))
  {
    return false;
  }
  private_key = extractKey(keypair, KeyAccess::PRIVATE);
  public_key  = extractKey(keypair, KeyAccess::PUBLIC);
  EVP_PKEY_free(keypair);
  return true;
}

std::string Crypto::extractKey(EVP_PKEY* keypair, KeyAccess key_access)
{
  // Create a place to dump the IO, in this case in memory
  BIO* keyBIO = BIO_new(BIO_s_mem());

  // Dump key to IO
  if (key_access == KeyAccess::PRIVATE) {
    PEM_write_bio_PrivateKey(keyBIO, keypair, nullptr, nullptr, 0, nullptr, nullptr);
  } else {
    PEM_write_bio_PUBKEY(keyBIO, keypair);
  }

  // Create char reference of public key length
  int key_len = BIO_pending(keyBIO);
  auto key_char = static_cast<char*>(malloc(key_len+1));

  // Read the key from the buffer and put it in the char reference
  BIO_read(keyBIO, key_char, key_len);
  BIO_free_all(keyBIO);
  key_char[key_len] = '\0';
  std::string extracted_key {key_char};
  free(key_char);
  return extracted_key;
}

EvpPkeyUniquePtr Crypto::compileKey(const std::string& key_str, KeyAccess key_access)
{
  // Write char array to BIO
  BIO* rsaKeyBIO { BIO_new_mem_buf(key_str.c_str(), -1) };

  // Create a RSA object from key char array
  RSA* (*read_key_from_bio)(BIO*, RSA**, pem_password_cb*, void*) =
    key_access == KeyAccess::PRIVATE ? PEM_read_bio_RSAPrivateKey
                                     : PEM_read_bio_RSA_PUBKEY;
  RSA* rsaKey { nullptr };
  read_key_from_bio(rsaKeyBIO, &rsaKey, nullptr, nullptr);
  BIO_free_all(rsaKeyBIO);

  // Create key
  EVP_PKEY* pkey { EVP_PKEY_new() };
  EVP_PKEY_assign_RSA(pkey, rsaKey);
  return std::unique_ptr<EVP_PKEY, EvpPkeyFree> { pkey };
}

bool Crypto::encrypt(const std::string& plain_message,
                     const std::string& public_key,
                     std::string& encrypted_message,
                     CryptoEvpPars& evp_pars)
{
  auto evp_public_key { compileKey(public_key, KeyAccess::PUBLIC) };

  // Initalize context
  EVP_CIPHER_CTX* rsa_encrypt_context { EVP_CIPHER_CTX_new() };
  if (rsa_encrypt_context == nullptr) {
    return false;
  }

  // Initializes a cipher context `rsa_encrypt_context' for encryption with
  // symmetric cipher type `EVP_aes_256_cbc()' using random secret key and IV.
  // This secret key is encrypted using a public key. We can also encrypt with
  // multiple public keys. `EVP_aes_256_cbc()' is the AES-256 cipher type.
  // AES - Advanced encryption standard
  unsigned char* encrypted_key = evp_pars.encrypted_key;
  EVP_PKEY* evp_public_keys[] = { evp_public_key.get() };
  if (!EVP_SealInit(rsa_encrypt_context,
                    EVP_aes_256_cbc(),
                    &encrypted_key,
                    &evp_pars.encrypted_key_length,
                    evp_pars.iv,
                    evp_public_keys,
                    sizeof(evp_public_keys)/sizeof(evp_public_keys[0])))
  {
    return false;
  }

  int encrypted_message_length = 0;
  auto encrypted_message_buf = static_cast<unsigned char*>(malloc(plain_message.size() + EVP_MAX_IV_LENGTH + 1));
  if (encrypted_message_buf == nullptr) {
    return false;
  }

  // Encrypts the given number of bytes from plain_message and writes the
  // encrypted version to `encrypted_message_buf'. In our case we are
  // encrypting only a single message, we can always call this multiple times
  // and do line by line seperate encryption if needed.
  if (!EVP_SealUpdate(rsa_encrypt_context,
                      encrypted_message_buf,
                      &encrypted_message_length,
                      reinterpret_cast<const unsigned char*>(plain_message.c_str()),
                      static_cast<int>(plain_message.size())))
  {
    return false;
  }

  /* Finalizing encryption */
  /* This will pad extra space in buffer to fill up last block. */
  int padding_length = 0;
  if (!EVP_SealFinal(rsa_encrypt_context,
                     encrypted_message_buf + encrypted_message_length,
                     &padding_length))
  {
    return false;
  }

  EVP_CIPHER_CTX_free(rsa_encrypt_context);

  encrypted_message_length += padding_length;
  encrypted_message = Base64::encode(encrypted_message_buf,
                                     static_cast<size_t>(encrypted_message_length));
  free(encrypted_message_buf);
  return true;
}

bool Crypto::decrypt(const std::string& encrypted_message,
                     const std::string& private_key,
                     std::string& decrypted_message,
                     const CryptoEvpPars& evp_pars)
{
  // Initalize context
  EVP_CIPHER_CTX* rsa_decrypt_context { EVP_CIPHER_CTX_new() };
  if(rsa_decrypt_context == nullptr)
  {
    return false;
  }

  auto evp_private_key { compileKey(private_key, KeyAccess::PRIVATE) };

  // Initializes a cipher context `rsa_decrypt_context' for decryption with
  // cipher type `EVP_aes_256_cbc()'. It decrypts the encrypted symmetric key
  // of length `encrypted_key_length' bytes passed in as `encrypted_key' using
  // the compiled private key
  if (!EVP_OpenInit(rsa_decrypt_context,
                    EVP_aes_256_cbc(),
                    evp_pars.encrypted_key,
                    evp_pars.encrypted_key_length,
                    evp_pars.iv,
                    evp_private_key.get()))
  {
    return false;
  }

  int decrypted_message_length = 0;
  auto decrypted_message_buf = static_cast<unsigned char*>(malloc(encrypted_message.size() + EVP_MAX_IV_LENGTH + 1));
  if (decrypted_message_buf == nullptr)
  {
    return false;
  }

  std::vector<unsigned char> encrypted_message_original { Base64::decode(encrypted_message) };

  // Decrypt the message
  if (!EVP_OpenUpdate(rsa_decrypt_context,
                      decrypted_message_buf,
                      &decrypted_message_length,
                      encrypted_message_original.data(),
                      static_cast<int>(encrypted_message_original.size())))
  {
    return false;
  }

  // Add padding
  int padding_length = 0;
  if (!EVP_OpenFinal(rsa_decrypt_context,
                     decrypted_message_buf + decrypted_message_length,
                     &padding_length))
  {
    return false;
  }

  EVP_CIPHER_CTX_free(rsa_decrypt_context);
  decrypted_message_buf[decrypted_message_length + padding_length] = '\0';
  decrypted_message = std::string { reinterpret_cast<const char*>(decrypted_message_buf) };
  free(decrypted_message_buf);

  return true;
}
} 
