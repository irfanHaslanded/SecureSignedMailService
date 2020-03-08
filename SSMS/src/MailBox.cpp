#include "MailBox.h"
#include "Crypto.h"

namespace ssms {

MailBox::MailBox(const User& owner) : owner_{owner} {};

void MailBox::SendMsg(MailBox& to, const std::string& plain_msg)
{
  to.mailbox_.emplace_back(
      std::make_pair(owner_.GetName(),
                     Crypto::Encrypt(plain_msg, to.owner_.GetPublicKey())));
}

std::list<Msg> MailBox::GetReceivedMsgs()
{
  std::list<Msg> decrypted_received_msgs;
  for (const auto &msg : mailbox_)
  {
    decrypted_received_msgs.emplace_back(
        std::make_pair(msg.first,
                       Crypto::Decrypt(msg.second, owner_.GetPrivateKey())));
  }
  return decrypted_received_msgs;
}

}