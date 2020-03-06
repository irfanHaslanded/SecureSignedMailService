#ifndef SSMS_MAILBOX_H_
#define SSMS_MAILBOX_H_

#include <string>
#include <list>

#include "User.h"

namespace ssms {
class MailBox {

public:
  MailBox() = default;
  ~MailBox() = default;
  bool SendMsg(const User& to, const std::string msg);
  std::string ReadReceivedMsgs();
  std::string ReadSentMsgs();

private:
  std::list<std::string> mailbox_;
};

}

#endif