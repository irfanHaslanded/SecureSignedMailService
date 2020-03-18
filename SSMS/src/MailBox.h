#ifndef SSMS_MAILBOX_H_
#define SSMS_MAILBOX_H_

#include <string>
#include <list>

namespace ssms {

  /**
   * A message consists of two parts
   * @param first The name of the sender user
   * @param second The body of the message
   */
using Msg = std::pair<std::string, std::string>;

class User;

/**
 * MailBox class. Each user has a MailBox, via which
 * it is able to send encrypted messages into other
 * Users' mailboxes, and into which it is able to get
 * encrypted messages from other users.
 */
class MailBox {

public:
  /**
   * Constructor
   * @param owner User to whom the mailbox belongs
   */
  MailBox(const User& owner);

  /**
   * Throw message encrypted to the mailbox
   * @param from      Sender of the message
   * @param plain_msg Text of the message
   */
  void throwMsg(const User& from, const std::string& plain_msg);

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