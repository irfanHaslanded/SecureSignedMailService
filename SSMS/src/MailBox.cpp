#include "MailBox.h"
#include "Crypto.h"
#include "User.h"

namespace ssms {

// Message
Msg::Msg(const std::string& sender_id , const std::string& text)
  : sender_id(sender_id) , text(text) {}
bool Msg::operator==(const Msg& rhs) const
{
  return this->sender_id == rhs.sender_id &&
         this->text      == rhs.text;
}

// MailBox member methods

MailBox::MailBox(const User& owner) : owner_{owner} {};

void MailBox::throwMsg(const Msg& msg)
{
  mailbox_.emplace_back(
    Msg(msg.sender_id, Crypto::encrypt(msg.text, owner_.getPublicKey())));
}

std::list<Msg> MailBox::getReceivedMsgs() const
{
  std::list<Msg> decrypted_received_msgs;
  for (const auto &msg : mailbox_)
  {
    decrypted_received_msgs.emplace_back(
      Msg(msg.sender_id, Crypto::decrypt(msg.text, owner_.getPrivateKey())));
  }
  return decrypted_received_msgs;
}

void MailBox::clear()
{
  mailbox_.clear();
}

}
