/*
 * Test_User.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#include "Test_User.h"
#include "../User.h"

using namespace ssms;

Test_User::Test_User() {
  // TODO Auto-generated constructor stub
}

Test_User::~Test_User() {
  // TODO Auto-generated destructor stub
}

TEST(Test_User, SendMessage)
{
  User alice("Alice");
  User bob("Bob");

  ASSERT_EQ(bob.showInbox(), 0);
  ASSERT_EQ(bob.showInbox(), 0);
  ASSERT_TRUE(alice.sendMessage("Bob", "Hello Bob! It's Alice"));
  ASSERT_EQ(bob.showInbox(), 1);
}