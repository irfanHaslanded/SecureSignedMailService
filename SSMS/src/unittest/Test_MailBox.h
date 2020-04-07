/*
 * Test_MailBox.h
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#ifndef SRC_UNITTEST_MAILBOX_H_
#define SRC_UNITTEST_MAILBOX_H_
#include <gtest/gtest.h>

class Test_MailBox : public ::testing::Test {
protected:
  Test_MailBox();
  virtual ~Test_MailBox();
  virtual void SetUp() override;
  virtual void TearDown() override;
};

#endif /* SRC_UNITTEST_MAILBOX_H_ */
