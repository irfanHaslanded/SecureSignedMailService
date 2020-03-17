#include "MailBox.h"
#include "Crypto.h"
#include "User.h"

namespace ssms {

MailBox::MailBox(const User& owner) : owner_{owner} {};

void MailBox::sendMsg(MailBox& to, const std::string& plain_msg)
{
  to.mailbox_.emplace_back(
      std::make_pair(owner_.getName(),
                     Crypto::encrypt(plain_msg, to.owner_.getPublicKey())));
}

std::list<Msg> MailBox::getReceivedMsgs() const
{
  std::list<Msg> decrypted_received_msgs;
  for (const auto &msg : mailbox_)
  {
    decrypted_received_msgs.emplace_back(
        std::make_pair(msg.first,
                       Crypto::decrypt(msg.second, owner_.getPrivateKey())));
  }
  return decrypted_received_msgs;
}

}