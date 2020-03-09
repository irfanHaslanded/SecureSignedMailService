#ifndef SSMS_MAILBOX_H_
#define SSMS_MAILBOX_H_

#include <string>
#include <list>

#include "User.h"

namespace ssms {

/**
 * MailBox class. Each user has a MailBox, via which
 * it is able to send encrypted messages into other
 * Users' mailboxes, and into which it is able to get
 * encrypted messages from other users.
 */
class MailBox {

  /**
   * A message consists of two parts
   * @param first The name of the sender user
   * @param second The body of the message
   */
using Msg = std::pair<std::string, std::string>;

public:
  /**
   * Constructor
   * @param owner User to whom the mailbox belongs
   */
  MailBox(const User& owner);

  /**
   * Send encrypted message
   * @param to Recipient user's inbox
   * @param plain_msg Text of the message
   */
  void sendMsg(MailBox& to, const std::string& plain_msg);

  /**
   * Get all the received messages
   * @returns a list of decrypted messages
   */
  std::list<Msg> getReceivedMsgs() const;

private:
  const User& owner_;
  std::list<Msg> mailbox_;
};

}

#endif // SSMS_MAILBOX_H_