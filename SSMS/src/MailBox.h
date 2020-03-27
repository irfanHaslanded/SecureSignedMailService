#ifndef SSMS_MAILBOX_H_
#define SSMS_MAILBOX_H_

#include <string>
#include <list>

namespace ssms {

class User;

struct Msg {
  const std::string sender_id; /** user id of the sender */
  const std::string text;      /** text of the message */
#if 0
  unsigned char ek[1024]; // TODO use proper size
  int ek_len;
  unsigned char iv[1024]; // TODO use proper size
  int iv_len;
#endif
  Msg(const std::string& sender_id, const std::string& text);
  bool operator==(const Msg& rhs) const;
};

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
   * @param msg Message containing sender id and text
   */
  void throwMsg(const Msg& msg);

  /**
   * Get all the received messages
   * @returns a list of decrypted messages
   */
  std::list<Msg> getReceivedMsgs() const;

  /**
   * Empty inbox
   */
  void clear();

private:
  const User& owner_;
  std::list<Msg> mailbox_;
};

}

#endif // SSMS_MAILBOX_H_