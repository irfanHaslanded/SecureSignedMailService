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
  ASSERT_EQ(mb.getMsgs().size(), 0);
}

TEST_F(Test_MailBox, ThrowMsg)
{
  auto a = User::create("alice");
  auto b = User::create("bob");
  MailBox mb { b };

  mb.throwMsg( Msg {a->getId(), "asdf", b->getId()});
  ASSERT_EQ(mb.getMsgs().size(), 1);
}

TEST_F(Test_MailBox, GetReceivedMsgs)
{
  auto a = User::create("alice");
  auto b = User::create("bob");
  MailBox mb { b };
  const Msg msg {a->getId(), " Hello!", b->getId()};

  mb.throwMsg(msg);
  const auto receivedMsgs = mb.getMsgs();
  ASSERT_NE(receivedMsgs.begin(), receivedMsgs.end());
  ASSERT_EQ(*receivedMsgs.begin(), msg);
  Msg it = *receivedMsgs.begin();
  std::cout << "Original text:  '" << msg.text << "'" << std::endl;
  std::cout << "Decrypted text: '" << it.text  << "'" << std::endl;
}

TEST_F(Test_MailBox, GetSentMsgs)
{
  auto a = User::create("alice");
  auto b = User::create("bob");
  MailBox mb { b };
  MailBox mb_sender {a};
  const Msg msg {a->getId(), " Hello!", b->getId()};

  mb.throwMsg(msg);
  mb_sender.throwMsg(msg);
  const auto sentMsgs = mb_sender.getMsgs();
  ASSERT_NE(sentMsgs.begin(), sentMsgs.end());
  ASSERT_EQ(*sentMsgs.begin(), msg);
  Msg it = *sentMsgs.begin();
  std::cout << "Original text:  '" << msg.text << "'" << std::endl;
  std::cout << "Decrypted text: '" << it.text  << "'" << std::endl;
}

TEST_F(Test_MailBox, Clear)
{
  auto a = User::create("alice");
  auto b = User::create("bob");
  MailBox mb { b };
  MailBox mb_sender {a};

  const Msg msg {a->getId(), " Hello!", b->getId()};
  mb.throwMsg(msg);
  mb_sender.throwMsg(msg);
  mb.clear();
  mb_sender.clear();
  ASSERT_EQ(mb.getMsgs().size(), 0);
  ASSERT_EQ(mb_sender.getMsgs().size(), 0);
}
