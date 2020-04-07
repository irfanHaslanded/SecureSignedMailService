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

void Test_User::SetUp() {
}

void Test_User::TearDown() {
  User::removeAll();
}

TEST_F(Test_User, Create)
{
  ASSERT_NE(User::create("asdf"), nullptr);
}

TEST_F(Test_User, CreateId)
{
  const std::string id { "asdf" };
  ASSERT_EQ(User::create(id)->getId(), id);
}

TEST_F(Test_User, GetUser)
{
  const std::string id { "alice" };
  User::create(id);
  auto u = User::get(id);
  ASSERT_EQ(id, u->getId());
}

TEST_F(Test_User, NoSuchUser)
{
  ASSERT_EQ(User::get("alice"), nullptr);
}

TEST_F(Test_User, RemoveUser)
{
  User::create("a");
  User::remove("a");
  ASSERT_EQ(User::get("a"), nullptr);
}

TEST_F(Test_User, RemoveAllUsers)
{
  User::create("a");
  User::create("b");
  User::removeAll();
  ASSERT_EQ(User::get("a"), nullptr);
  ASSERT_EQ(User::get("b"), nullptr);
  ASSERT_EQ(User::getIdList().size(), 0);
}

TEST_F(Test_User, AlreadyExists)
{
  const std::string id { "alice" };
  User::create(id);
  ASSERT_THROW(User::create(id), UserAlreadyExists);
}

TEST_F(Test_User, IdListSize)
{
  User::create("a");
  User::create("b");
  ASSERT_EQ(User::getIdList().size(), 2);
}

TEST_F(Test_User, IdListContent)
{
  User::create("a");
  ASSERT_EQ(User::getIdList().front(), "a");
}

TEST_F(Test_User, InitialNameIsId)
{
  auto user = User::create("alice");
  ASSERT_EQ(user->getName(), user->getId());
}

TEST_F(Test_User, SetName)
{
  auto alice = User::create("alice");
  const std::string the_name("Alice Whois");
  alice->setName(the_name);
  ASSERT_EQ(alice->getName(), the_name);
}

TEST_F(Test_User, SendMessage)
{
  auto alice = User::create("alice");
  auto bob   = User::create("bob");

  ASSERT_EQ(bob->showInbox(), 0);
  ASSERT_EQ(alice->showInbox(), 0);
  ASSERT_TRUE(alice->sendMessage("bob", "Hello Bob! It's Alice"));
  ASSERT_TRUE(alice->sendMessage("bob", "Hello Bob! It's Alice again"));
  ASSERT_EQ(bob->showInbox(), 2);
}

TEST_F(Test_User, EmptyInbox)
{
  auto alice = User::create("alice");
  auto bob   = User::create("bob");

  alice->sendMessage("bob", "msg1");
  alice->sendMessage("bob", "msg2");
  bob->emptyInbox();
  ASSERT_EQ(bob->showInbox(), 0);
}