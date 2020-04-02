#ifndef SSMS_BASE64_H
#define SSMS_BASE64_H

#include <string>
#include <vector>

namespace ssms {

class Base64 {

public:
  static std::string encode(const std::vector<unsigned char>& binary);
  static std::string encode(const unsigned char* binary, size_t size);
  static std::vector<unsigned char> decode(const std::string& base64);

private:
  static const int BIO_flags;
};

}

#endif