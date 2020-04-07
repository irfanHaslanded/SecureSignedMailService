#include "MailBox.h"
#include "Crypto.h"
#include "User.h"

namespace ssms {

// Message
Msg::Msg() {}
Msg::Msg(const std::string& sender_id , const std::string& text)
  : sender_id(sender_id) , text(text) {}
bool Msg::operator==(const Msg& rhs) const
{
  return this->sender_id == rhs.sender_id &&
         this->text      == rhs.text;
}

// MailBox member methods

MailBox::MailBox(std::shared_ptr<User> owner) : owner_{owner} {};

void MailBox::throwMsg(const Msg& plain_msg)
{
  Msg encrypted_msg {};
  if (Crypto::encrypt(plain_msg.text,
                      owner_->getPublicKey(),
                      encrypted_msg.text,
                      encrypted_msg.evp_pars))
  {
    encrypted_msg.sender_id = plain_msg.sender_id;
    mailbox_.push_back(encrypted_msg);
  }
}

std::list<Msg> MailBox::getReceivedMsgs() const
{
  std::list<Msg> decrypted_received_msgs;
  Msg decrypted_msg {};
  for (const auto& encrypted_msg : mailbox_)
  {
    if (Crypto::decrypt(encrypted_msg.text,
                        owner_->getPrivateKey(),
                        decrypted_msg.text,
                        encrypted_msg.evp_pars))
    {
      decrypted_msg.sender_id = encrypted_msg.sender_id;
      decrypted_received_msgs.push_back(decrypted_msg);
    }
  }
  return decrypted_received_msgs;
}

void MailBox::clear()
{
  mailbox_.clear();
}

}
