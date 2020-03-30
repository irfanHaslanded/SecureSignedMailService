/*
 * Test_Base64.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: ezdezta
 */

#include "Test_Base64.h"
#include "../Base64.h"

using namespace ssms;

Test_Base64::Test_Base64() {
  // TODO Auto-generated constructor stub
}

Test_Base64::~Test_Base64() {
  // TODO Auto-generated destructor stub
}

void Test_Base64::SetUp() {}

void Test_Base64::TearDown() {}


TEST(Test_Base64, backAndForth)
{
  const std::vector<unsigned char> original {0x11, 0x22, 0x33, 0x44, 0x00, 0x99};
  std::string encoded { Base64::encode(original) };
  std::cout << encoded << std::endl;
  std::vector<unsigned char> decoded { Base64::decode(encoded) };
  ASSERT_EQ(original, decoded);
}