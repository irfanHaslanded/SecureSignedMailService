#ifndef SSMS_MAILBOX_H_
#define SSMS_MAILBOX_H_

#include <string>
#include <list>

#include "User.h"

namespace ssms {

class MailBox {

using Msg = std::pair<std::string, std::string>;

public:
  MailBox(const User& owner);
  void sendMsg(MailBox& to, const std::string& plain_msg);
  std::list<Msg> getReceivedMsgs() const;

private:
  const User& owner_;
  std::list<Msg> mailbox_;
};

}

#endif // SSMS_MAILBOX_H_