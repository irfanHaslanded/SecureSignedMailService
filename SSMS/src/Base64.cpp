#include "Base64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <memory>

namespace ssms {

struct BIOFreeAll {
  void operator()(BIO *b64) { BIO_free_all(b64); }
};

const int Base64::BIO_flags = BIO_FLAGS_BASE64_NO_NL;

std::string Base64::encode(const std::vector<unsigned char>& binary)
{
  return encode(binary.data(), binary.size());
}

std::string Base64::encode(const unsigned char* binary, size_t size)
{
  std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
  BIO_set_flags(b64.get(), BIO_flags);
  BIO *mem = BIO_new(BIO_s_mem());
  BIO_push(b64.get(), mem);
  BIO_write(b64.get(), binary, size);
  BIO_flush(b64.get());
  const char *encoded;
  const size_t length = BIO_get_mem_data(mem, &encoded);
  return std::string(encoded, length);
}

std::vector<unsigned char> Base64::decode(const std::string& encoded)
{
  std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
  BIO_set_flags(b64.get(), BIO_flags);
  BIO *source = BIO_new_mem_buf(encoded.c_str(), -1); // read-only source
  BIO_push(b64.get(), source);
  const size_t maxlen = encoded.size() / 4 * 3 + 1;
  std::vector<unsigned char> decoded(maxlen);
  const size_t length = BIO_read(b64.get(), decoded.data(), maxlen);
  decoded.resize(length);
  return decoded;
}

}
