/*
 * Crypto.cpp
 *
 *  Created on: March 6, 2020
 *      Author: Gopi
 */

#include "Crypto.h"
#include <crypt.h>

#include <stdlib.h>

namespace ssms {

CryptoEvpPars::CryptoEvpPars() : encrypted_key_length(sizeof(encrypted_key)) {}

const int saltSize = 16;
const int saltIdSize = 3;  // eaxmple "$6$"

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
  std::string salt(hash.substr(0, saltSize + saltIdSize + 1));
  std::cout << "salt is  : " + salt << std::endl; // for UT debugging purposes
  return salt;
}

int Crypto::generateRsaKeypair(EVP_PKEY **keypair)
{
  EVP_PKEY_CTX *context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if(EVP_PKEY_keygen_init(context) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_keygen(context, keypair) <= 0) {
    return FAILURE;
  }

  EVP_PKEY_CTX_free(context);
  return SUCCESS;
}

void Crypto::generateRsaKeypair(std::string& private_key, std::string& public_key)
{
  EVP_PKEY *keypair = NULL;
  if (generateRsaKeypair(&keypair) != SUCCESS)
  {
    return;
  }
  private_key = extractKey(keypair, true);
  public_key  = extractKey(keypair, false);
  free(keypair);
}

std::string Crypto::extractKey(EVP_PKEY* keypair, bool isPrivate)
{
  // create a place to dump the IO, in this case in memory
  BIO *keyBIO = BIO_new(BIO_s_mem());
  // dump key to IO
  if (isPrivate) {
    PEM_write_bio_PrivateKey(keyBIO, keypair, NULL, NULL, 0, 0, NULL);
  } else {
    PEM_write_bio_PUBKEY(keyBIO, keypair);
  }
  // get buffer length
  int key_len = BIO_pending(keyBIO);
  // create char reference of public key length
  char *key_char = (char *) malloc(key_len+1);
  // read the key from the buffer and put it in the char reference
  BIO_read(keyBIO, key_char, key_len);
  key_char[key_len] = '\0';
  std::string extracted_key {key_char};
  free(key_char);
  free(keyBIO);
  return extracted_key;
}

EVP_PKEY* Crypto::compileKey(const std::string& key_str, bool isPrivate)
{
  // write char array to BIO
  BIO *rsaKeyBIO { BIO_new_mem_buf(key_str.c_str(), -1) };

  // create a RSA object from key char array
  RSA *(*read_key_from_bio)(BIO *, RSA **, pem_password_cb *, void *) =
    isPrivate ? PEM_read_bio_RSAPrivateKey : PEM_read_bio_RSA_PUBKEY;
  RSA *rsaKey { nullptr };
  read_key_from_bio(rsaKeyBIO, &rsaKey, nullptr, nullptr);

  // create key
  EVP_PKEY *pkey { EVP_PKEY_new() };
  EVP_PKEY_assign_RSA(pkey, rsaKey);
  free(rsaKeyBIO);
  //free(rsaKey);

  return pkey;
}

bool Crypto::encrypt(const std::string& plain_message,
                     const std::string& public_key,
                     std::string& encrypted_message,
                     CryptoEvpPars& evp_pars)
{
  /*TODO:Extracting public key from keypair generated in init and convert the key into EVP_PKEY** format and then replace localKeypair in EVP_SealInit() with that.---*/
  EVP_PKEY* evp_public_key { compileKey(public_key, false) };

  // Initalize context
  EVP_CIPHER_CTX* rsa_encrypt_context { EVP_CIPHER_CTX_new() };
  if(rsa_encrypt_context == nullptr) {
    return false;
  }

  unsigned char *encrypted_key = evp_pars.encrypted_key;
  /* Intializes a cipher context "rsaEncryptContext" for encryption with symmetric cipher type "EVP_aes_256_cbc()" using random secret key and IV. This secret key is encrypted using a public key.
     We can also encrypt with multiple pub keys */
  /* EVP_aes_256_cbc() is the AES-256 cipher type. AES - Advanced encryption standard */
  if(!EVP_SealInit(rsa_encrypt_context,
                   EVP_aes_256_cbc(),
                   &encrypted_key,
                   &evp_pars.encrypted_key_length,
                   evp_pars.iv,
                   &evp_public_key,
                   1))
  {
    return FAILURE;
  }

  int encrypted_message_length = 0;
  unsigned char* encrypted_message_buf = (unsigned char*)malloc(plain_message.size() + EVP_MAX_IV_LENGTH + 1);
  if (encrypted_message_buf == NULL) {
    return false;
  }

  /* encrypts the given number of bytes from plain_message and writes the encrypted version to *encrypted_message_buf */
  /* In our case we are encrypting only a single message, we can always call this multiple times and do line by line seperate encryption if needed. */
  if(!EVP_SealUpdate(rsa_encrypt_context,
                     encrypted_message_buf,
                     &encrypted_message_length,
                     (const unsigned char*)plain_message.c_str(),
                     (int)plain_message.size()))
  {
    return false;
  }

  /* Finalizing encryption */
  /* This will pad extra space in buffer to fill up last block. */
  int padding_length = 0;
  if(!EVP_SealFinal(rsa_encrypt_context,
                    encrypted_message_buf + encrypted_message_length,
                    &padding_length))
  {
    return false;
  }

  EVP_CIPHER_CTX_free(rsa_encrypt_context);
  free(evp_public_key);
  encrypted_message_buf[encrypted_message_length + padding_length] = '\0';
  encrypted_message = std::string { (char *)encrypted_message_buf }; // TODO we need a real bin->ascii conversion
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
  if(rsa_decrypt_context == NULL) {
    return false;
  }

  EVP_PKEY* evp_private_key { compileKey(private_key, true) };

  // Decrypt it!
  /*Initializes a cipher context "rsaDecryptContext" for decryption with cipher type "EVP_aes_256_cbc()". It decrypts the encrypted symmetric key of length "encryptedKeyLength" bytes passed in 
  as "encryptedKey" using the private key*/
  if(!EVP_OpenInit(rsa_decrypt_context,
                   EVP_aes_256_cbc(),
                   evp_pars.encrypted_key,
                   evp_pars.encrypted_key_length,
                   evp_pars.iv,
                   evp_private_key))
  {
    return false;
  }

  int decrypted_message_length = 0;
  unsigned char *decrypted_message_buf = (unsigned char *)malloc(encrypted_message.size() + EVP_MAX_IV_LENGTH + 1);
  if (decrypted_message_buf == nullptr) {
    return false;
  }

  /* You know what it does :p */
  if(!EVP_OpenUpdate(rsa_decrypt_context,
                     decrypted_message_buf,
                     &decrypted_message_length,
                     (const unsigned char *)encrypted_message.c_str(), // TODO we need an ascii->bin conversion
                     (int)encrypted_message.size())) {
    return false;
  }

  int padding_length = 0;
  if(!EVP_OpenFinal(rsa_decrypt_context,
                    decrypted_message_buf + decrypted_message_length,
                    &padding_length))
  {
    return false;
  }

  EVP_CIPHER_CTX_free(rsa_decrypt_context);
  free(evp_private_key);
  decrypted_message_buf[decrypted_message_length + padding_length] = '\0';
  decrypted_message = std::string { (const char*)decrypted_message_buf };
  free(decrypted_message_buf);
  return true;
}
} 
