#include "MailBox.h"
#include "Crypto.h"
#include "User.h"
#include "Base64.h"

#include <iostream>

namespace ssms {

// Message
Msg::Msg() {}
Msg::Msg(const std::string& sender_id , const std::string& text, const std::string& receiver_id)
  : sender_id(sender_id) , text(text), receiver_id(receiver_id) {}
bool Msg::operator==(const Msg& rhs) const
{
  return this->sender_id == rhs.sender_id && this->receiver_id == rhs.receiver_id &&
         this->text      == rhs.text;
}
std::ostream& operator<<(std::ostream& os, const Msg& msg)
{
  os << "\nFrom: " << msg.sender_id
     //<< "\nTo:   " << msg.dest_id
     << "\nMessage: "
     << "\n" << msg.text
     << "\nEncrypted key:"
     << "\n" << Base64::encode(msg.evp_pars.encrypted_key,
                               msg.evp_pars.encrypted_key_length)
     << "\nIV (nonce): ";
  for (size_t i = 0; i < sizeof(msg.evp_pars.iv); ++i) {
    os << std::hex << static_cast<unsigned int>(msg.evp_pars.iv[i]) << " " ;
  }
  os <<  std::endl;
  return os;
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
    encrypted_msg.receiver_id = plain_msg.receiver_id;
    mailbox_.push_back(encrypted_msg);

    std::cerr << "\n===== MESSAGE BEGIN ====="
              << encrypted_msg
              << "===== MESSAGE END ===="
              << std::endl;
  }
}

std::list<Msg> MailBox::getMsgs() const
{
  std::list<Msg> decrypted_msgs;
  Msg decrypted_msg {};
  for (const auto& encrypted_msg : mailbox_)
  {
    if (Crypto::decrypt(encrypted_msg.text,
                        owner_->getPrivateKey(),
                        decrypted_msg.text,
                        encrypted_msg.evp_pars))
    {
      decrypted_msg.sender_id = encrypted_msg.sender_id;
      decrypted_msg.receiver_id = encrypted_msg.receiver_id;
      decrypted_msgs.push_back(decrypted_msg);
    }
  }
  return decrypted_msgs;
}

void MailBox::clear()
{
  mailbox_.clear();
}

}
