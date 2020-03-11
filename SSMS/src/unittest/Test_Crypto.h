/*
 * Test_Crypto.h
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#ifndef SRC_UNITTEST_CRYTO_H_
#define SRC_UNITTEST_CRYTO_H_
#include <gtest/gtest.h>
class Test_Crypto : testing::Test {
public:
  Test_Crypto();
  virtual ~Test_Crypto();
  void SetUp();
  void TearDown();
};

#endif /* SRC_UNITTEST_CRYTO_H_ */
