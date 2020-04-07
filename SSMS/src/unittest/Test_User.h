/*
 * Test_User.h
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#ifndef SRC_UNITTEST_USER_H_
#define SRC_UNITTEST_USER_H_
#include <gtest/gtest.h>


class Test_User : public ::testing::Test {
protected:
  Test_User();
  virtual ~Test_User();
  virtual void SetUp() override;
  virtual void TearDown() override;
};

#endif /* SRC_UNITTEST_USER_H_ */
