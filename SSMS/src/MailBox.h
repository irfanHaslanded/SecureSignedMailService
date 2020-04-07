#ifndef SSMS_MAILBOX_H_
#define SSMS_MAILBOX_H_

#include "Crypto.h"

#include <openssl/evp.h>
#include <string>
#include <list>

namespace ssms {

class User;

struct Msg {
  std::string sender_id; /** user id of the sender */
  std::string text;      /** text of the message */
  CryptoEvpPars evp_pars;
  Msg();
  Msg(const std::string& sender_id , const std::string& text);
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
  MailBox(std::shared_ptr<User> owner);

  /**
   * Throw message encrypted to the mailbox
   * @param plain_msg Message containing sender id and text
   */
  void throwMsg(const Msg& plain_msg);

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
  std::shared_ptr<User> owner_;
  std::list<Msg> mailbox_;
};

}

#endif // SSMS_MAILBOX_H_
