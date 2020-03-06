#include "MailBox.h"
#include "Crypto.h"

namespace ssms {

MailBox::SendMsg(const int &to, const std::string msg)
{
  //mailbox_.push_back(Crypto::Encrypt(msg, priv_key));
}

MailBox::ReadReceivedMsgs(const int &to)
{
  ;
}

MailBox::ReadSentMsgs(const int &to)
{
  ;
}

}
