/*
 * Test_MailBox.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#include "Test_MailBox.h"

#include "../MailBox.h"
#include "../User.h"

using namespace ssms;

Test_MailBox::Test_MailBox() {
  // TODO Auto-generated constructor stub
}

Test_MailBox::~Test_MailBox() {
  // TODO Auto-generated destructor stub
}

void Test_MailBox::SetUp() {
}

void Test_MailBox::TearDown() {
  User::removeAll();
}

TEST_F(Test_MailBox, Init)
{
  MailBox mb { User::create("alice") };
  ASSERT_EQ(mb.getReceivedMsgs().size(), 0);
}

TEST_F(Test_MailBox, ThrowMsg)
{
  auto a = User::create("alice");
  MailBox mb { User::create("bob") };

  mb.throwMsg( Msg {a->getId(), "asdf"} );
  ASSERT_EQ(mb.getReceivedMsgs().size(), 1);
}

TEST_F(Test_MailBox, GetReceivedMsgs)
{
  auto a = User::create("alice");
  MailBox mb { User::create("bob") };

  const Msg msg {a->getId(), " Hello!"};
  mb.throwMsg(msg);
  const auto receivedMsgs = mb.getReceivedMsgs();
  ASSERT_NE(receivedMsgs.begin(), receivedMsgs.end());
  ASSERT_EQ(*receivedMsgs.begin(), msg);
  Msg it = *receivedMsgs.begin();
  std::cout << "Original text:  '" << msg.text << "'" << std::endl;
  std::cout << "Decrypted text: '" << it.text  << "'" << std::endl;
}

TEST_F(Test_MailBox, Clear)
{
  auto a = User::create("alice");
  MailBox mb { User::create("bob") };

  mb.throwMsg(Msg {a->getId(), "Hello!"});
  mb.clear();
  ASSERT_EQ(mb.getReceivedMsgs().size(), 0);
}
