/*
 * Test_MailBox.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: egirkar
 */

#include "Test_MailBox.h"

#include "../MailBox.h"
#include "../User.h"

Test_MailBox::Test_MailBox() {
  // TODO Auto-generated constructor stub
}

Test_MailBox::~Test_MailBox() {
  // TODO Auto-generated destructor stub
}

using namespace ssms;

TEST(Test_MailBox, Init)
{
  User a {"alice"};
  MailBox mb {a};
  ASSERT_EQ(mb.getReceivedMsgs().size(), 0);
}

TEST(Test_MailBox, ThrowMsg)
{
  User a {"alice"};
  User b {"bob"};

  MailBox mb {b};
  mb.throwMsg( Msg {a.getId(), "asdf"} );
  ASSERT_EQ(mb.getReceivedMsgs().size(), 1);
}

TEST(Test_MailBox, GetReceivedMsgs)
{
  User a {"alice"};
  User b {"bob"};
  MailBox mb {b};

  const Msg msg {a.getId(), "Hello!"};
  mb.throwMsg(msg);
  const auto receivedMsgs = mb.getReceivedMsgs();
  ASSERT_NE(receivedMsgs.begin(), receivedMsgs.end());
  ASSERT_EQ(*receivedMsgs.begin(), msg);
  Msg it = *receivedMsgs.begin();
  std::cout << "Original text: " << msg.text << std::endl;
  std::cout << "Decrypted text: " << it.text << std::endl;
}

TEST(Test_MailBox, Clear)
{
  User a {"alice"};
  User b {"bob"};
  MailBox mb {b};

  mb.throwMsg(Msg {a.getId(), "Hello!"});
  mb.clear();
  ASSERT_EQ(mb.getReceivedMsgs().size(), 0);
}
