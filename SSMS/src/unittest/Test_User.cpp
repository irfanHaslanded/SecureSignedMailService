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

TEST(Test_User, Create)
{
  const std::string the_id("alice");
  User alice(the_id);
  ASSERT_EQ(alice.getId(), the_id);
}

TEST(Test_User, AlreadyExists)
{
  User user1("alice");
  ASSERT_THROW(User user2("alice"), UserAlreadyExists);
}

TEST(Test_User, NoInitialName)
{
  User user("alice");
  ASSERT_EQ(user.getName(), std::string());
}

TEST(Test_User, SetName)
{
  User alice("alice");
  const std::string the_name("Alice Whois");
  alice.setName(the_name);
  ASSERT_EQ(alice.getName(), the_name);
}

TEST(Test_User, SendMessage)
{
  User alice("alice");
  User bob("bob");

  ASSERT_EQ(bob.showInbox(), 0);
  ASSERT_EQ(alice.showInbox(), 0);
  ASSERT_TRUE(alice.sendMessage("bob", "Hello Bob! It's Alice"));
  ASSERT_EQ(bob.showInbox(), 1);
}

TEST(Test_User, EmptyInbox)
{
  User alice("alice");
  User bob("bob");

  alice.sendMessage("bob", "msg1");
  alice.sendMessage("bob", "msg2");
  bob.emptyInbox();
  ASSERT_EQ(bob.showInbox(), 0);
}