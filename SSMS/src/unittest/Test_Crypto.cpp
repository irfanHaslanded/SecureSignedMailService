/*
 * Test_Crypto.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#include "Test_Crypto.h"
#include "../Crypto.h"
using namespace ssms;
Test_Crypto::Test_Crypto() {
  // TODO Auto-generated constructor stub
}

Test_Crypto::~Test_Crypto() {
  // TODO Auto-generated destructor stub
}

void Test_Crypto::SetUp() {}

void Test_Crypto::TearDown() {}


TEST(Test_Crypto, test4DifferentSalts) {
  ASSERT_NE(Crypto::passTheSalt(), Crypto::passTheSalt());
}

TEST(Test_Crypto, testDifferentHash) {
  std::string salt(Crypto::passTheSalt());
  std::string salt2(Crypto::passTheSalt());
  std::string hash1(Crypto::genHash(salt, "karthik123"));
  std::string hash2(Crypto::genHash(salt2, "karthik123"));
  ASSERT_NE(hash1,hash2);
}

TEST(Test_Crypto, testSameHash) {
  std::string salt(Crypto::passTheSalt());
  std::string hash1(Crypto::genHash(salt, "karthik123"));
  std::string hash2(Crypto::genHash(salt, "karthik123"));
  ASSERT_EQ(hash1, hash2);
}

TEST(Test_Crypto, test4DifferentHashes) {
  std::string pwd("karthik123");
      ASSERT_NE(Crypto::genPassword(pwd), Crypto::genPassword(pwd));
}