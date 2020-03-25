/*
 * Crypto.cpp
 *
 *  Created on: March 6, 2020
 *      Author: Gopi
 */

#include "Crypto.h"
#include <crypt.h>



namespace ssms {
const int saltSize = 16;
const int saltIdSize = 3;  // eaxmple "$6$"

EVP_PKEY* Crypto::localKeypair;
EVP_CIPHER_CTX* Crypto::rsaEncryptContext;
EVP_CIPHER_CTX* Crypto::rsaDecryptContext;
int Crypto::messageLength;
unsigned char* Crypto::encryptedKey;
size_t Crypto::encryptedKeyLength;
static const std::string hashValidChars {
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"};

std::string Crypto::encrypt(const std::string &message,
                            const std::string &publickey) {

  init();//TODO: Better move this into a constructor

  // Encrypt the message with RSA
  // +1 on the string length argument because we want to encrypt the NUL terminator too
  unsigned char *encryptedMessage = NULL;
  unsigned char *iv;
  size_t ivLength;

  //TODO: We have an empty public key. Need to use the generated one from init().
  //unsigned char *publickey_generated;
  //getLocalPublicKey((unsigned char**)publickey_generated);

  int encryptedMessageLength = rsaEncrypt((const unsigned char*)message.c_str(), message.size()+1,
    &encryptedMessage, &encryptedKey, &encryptedKeyLength, &iv, &ivLength);

  messageLength = encryptedMessageLength;
  std::string s((char*)encryptedMessage);
  return s;
}

std::string Crypto::decrypt(const std::string &message,
                            const std::string &privatekey) {

  unsigned char *decryptedMessage = NULL;
  unsigned char *iv;
  size_t privatekeyLength = sizeof(privatekey);
  size_t ivLength;

  //TODO: We have an empty private key. Need to use the generated one in init().
  //unsigned char *privatekey_generated;
  //getLocalPrivateKey((unsigned char**)privatekeys);

  int decryptedMessageLength = rsaDecrypt((const unsigned char*)message.c_str(), messageLength,
  (const unsigned char*)&encryptedKey, encryptedKeyLength, iv, ivLength, (unsigned char**)&decryptedMessage);

  std::string s((char*)decryptedMessage);
  return s;
}

bool Crypto::validatePassword(const std::string &hash,
                              const std::string &password) {
  return hash == genHash(Crypto::getSalt(hash), password);

}

std::string Crypto::genPassword(const std::string &password) {

  std::string salt = passTheSalt();
  std::cout << salt << std::endl; // for UT debugging purposes
  return genHash(salt, password);

}

 std::string Crypto::passTheSalt() {
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

std::string Crypto::getSalt(const std::string &hash) {
  std::string salt(hash.substr(0, saltSize + saltIdSize + 1));
  std::cout << "salt is  : " + salt << std::endl; // for UT debugging purposes
  return salt;
}

int Crypto::init() {
  //Initialize keys
  localKeypair = NULL;

  //IV -Initilization vector can be left to null for rsa cipher

  // Generate RSA keys
  generateRsaKeypair(&localKeypair);

  return SUCCESS;
}

void Crypto::free(){

  EVP_CIPHER_CTX_free(rsaEncryptContext);

  EVP_CIPHER_CTX_free(rsaDecryptContext);
}

int Crypto::generateRsaKeypair(EVP_PKEY **keypair) {
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

int Crypto::rsaEncrypt(const unsigned char *message, size_t messageLength, unsigned char **encryptedMessage, unsigned char **encryptedKey,
  size_t *encryptedKeyLength, unsigned char **iv, size_t *ivLength) {

  // Allocate memory for everything
  size_t encryptedMessageLength = 0;
  size_t blockLength = 0;

  /*TODO:Extracting public key from keypair generated in init and convert the key into EVP_PKEY** format and then replace localKeypair in EVP_SealInit() with that.---*/

  // Initalize contexts
  rsaEncryptContext = EVP_CIPHER_CTX_new();

  // Check if any of the contexts initializations failed
  if(rsaEncryptContext == NULL) {
    return FAILURE;
  }

  *encryptedKey = (unsigned char*)malloc(EVP_PKEY_size(localKeypair));
  *iv = (unsigned char*)malloc(EVP_MAX_IV_LENGTH);/*Make sure to add more info about IV*/
  *ivLength = EVP_MAX_IV_LENGTH;

  if(*encryptedKey == NULL || *iv == NULL) {
    return FAILURE;
  }

  *encryptedMessage = (unsigned char*)malloc(messageLength + EVP_MAX_IV_LENGTH);
  if(encryptedMessage == NULL) {
    return FAILURE;
  }

  // Encrypt it!
  /*Intializes a cipher context "rsaEncryptContext" for encryption with symmetric cipher type "EVP_aes_256_cbc()" using random secret key and IV. This secret key is encrypted using a public key.
  We can also encrypt with multiple pub keys*/
  /*EVP_aes_256_cbc() is the AES-256 cipher type. AES - Advanced encryption standard*/
  if(!EVP_SealInit(rsaEncryptContext, EVP_aes_256_cbc(), encryptedKey, (int*)encryptedKeyLength, *iv, &localKeypair, 1)) {
    return FAILURE;
  }

  /*encrypts "messageLength" bytes from the "message" in and writes the encrypted version to "*encryptedMessage + encryptedMessageLength" of size "(int*)&blockLength"*/
  /*In our case we are encrypting only a single message, we can always call this multiple times and do line by line seperate encryption if needed.*/
  /*The update function returns the number of bytes that are encrypted.*/
  if(!EVP_SealUpdate(rsaEncryptContext, *encryptedMessage + encryptedMessageLength, (int*)&blockLength, (const unsigned char*)message, (int)messageLength)) {
    return FAILURE;
  }
  encryptedMessageLength += blockLength;

  /*Finalizing encryption*/
  /*This will pad extra space in buffer to fill up last block.*/
  if(!EVP_SealFinal(rsaEncryptContext, *encryptedMessage + encryptedMessageLength, (int*)&blockLength)) {
    return FAILURE;
  }
  encryptedMessageLength += blockLength;

  /*Returning the number of bytes encrypted. It is a binary string.*/
  return (int)encryptedMessageLength;
}

int Crypto::rsaDecrypt(const unsigned char *encryptedMessage, size_t encryptedMessageLength, const unsigned char *encryptedKey,
  size_t encryptedKeyLength, unsigned char *iv, size_t ivLength, unsigned char **decryptedMessage) {

  //Allocate memory for everything
  size_t decryptedMessageLength = 0;
  size_t blockLength = 0;

  *decryptedMessage = (unsigned char*)malloc(encryptedMessageLength + ivLength);
  if(*decryptedMessage == NULL) {
    return FAILURE;
  }

  // Initalize contexts
  rsaDecryptContext = EVP_CIPHER_CTX_new();

  // Check if any of the contexts initializations failed
  if(rsaDecryptContext == NULL) {
    return FAILURE;
  }

  /*TODO:Extracting private key from keypair generated in init and convert the key into EVP_PKEY** format. Then replace localKeypair in EVP_OpenInit() with that.---*/

  // Decrypt it!
  /*Initializes a cipher context "rsaDecryptContext" for decryption with cipher type "EVP_aes_256_cbc()". It decrypts the encrypted symmetric key of length "encryptedKeyLength" bytes passed in 
  as "encryptedKey" using the private key*/
  if(!EVP_OpenInit(rsaDecryptContext, EVP_aes_256_cbc(), encryptedKey, encryptedKeyLength, iv, localKeypair)) {
    return FAILURE;
  }

  /*You know what it does :p*/
  if(!EVP_OpenUpdate(rsaDecryptContext, (unsigned char*)*decryptedMessage + decryptedMessageLength, (int*)&blockLength, encryptedMessage, (int)encryptedMessageLength)) {
    return FAILURE;
  }
  decryptedMessageLength += blockLength;

  if(!EVP_OpenFinal(rsaDecryptContext, (unsigned char*)*decryptedMessage + decryptedMessageLength, (int*)&blockLength)) {
    return FAILURE;
  }

  decryptedMessageLength += blockLength;
  return (int)decryptedMessageLength;
}

int Crypto::getLocalPublicKey(unsigned char **publicKey) {
  BIO *bio = BIO_new(BIO_s_mem());
  PEM_write_bio_PUBKEY(bio, localKeypair);
  return bioToString(bio, publicKey);
}

int Crypto::getLocalPrivateKey(unsigned char **privateKey) {
  BIO *bio = BIO_new(BIO_s_mem());
  PEM_write_bio_PrivateKey(bio, localKeypair, NULL, NULL, 0, 0, NULL);
  return bioToString(bio, privateKey);
}

int Crypto::bioToString(BIO *bio, unsigned char **string) {
  size_t bioLength = BIO_pending(bio);
  *string = (unsigned char*)malloc(bioLength + 1);

  if(string == NULL) {
    return FAILURE;
  }

  BIO_read(bio, *string, bioLength);

  // Insert the NUL terminator
  (*string)[bioLength] = '\0';

  BIO_free_all(bio);

  return (int)bioLength;
}
} 
