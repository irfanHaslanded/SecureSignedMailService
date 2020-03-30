/*
 * Test_Base64.h
 *
 *  Created on: Mar 30, 2020
 *      Author: ezdezta
 */

#ifndef SRC_UNITTEST_BASE64_H_
#define SRC_UNITTEST_BASE64_H_

#include <gtest/gtest.h>

class Test_Base64 : testing::Test {
public:
  Test_Base64();
  virtual ~Test_Base64();
  void SetUp();
  void TearDown();
};

#endif /* SRC_UNITTEST_BASE64_H_ */
