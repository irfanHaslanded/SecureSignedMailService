/* User.h Oskar Paulander 2020-03-
* Class for handling users, including
* creating users, deleting, sending msgs etc.
*/
#ifndef SSMS_USER_H_
#define SSMS_USER_H_

#include "MailBox.h"

#include <map>
#include <exception>

namespace ssms {

class UserAlreadyExists : public std::exception {
  std::string id_;
  const char* what() const throw() override;
public:
  UserAlreadyExists() = delete;
  UserAlreadyExists(const std::string& id);
  std::string getId();
};

class User {

public:
  User(const std::string& id);
  ~User();

  const std::string& getId() const;
  void setName(const std::string& name);
  const std::string& getName() const;
  std::string toString() const;

  void setKeyPair();
  const std::string& getPrivateKey() const;
  const std::string& getPublicKey() const;

  void setPassword(const std::string& password);
  bool checkPassword(const std::string &password) const;

  bool sendMessage(const std::string& recipient_id, const std::string& msg);
  void sendMessage(User& recipient, const std::string& msg);
  size_t showInbox();
  void emptyInbox();

  // void addContact(std::string username);
  // void addContact(std::string username, std::string nickname);
  // void addContactById(int userId);
  // void addContactById(int userId, std::string nickname);

  // void displayMessage(int msgId);
  // void createMessage(std::string msg, User recipient);
  // void deleteMessage(int id);
  // void sendAll();

private:
  std::string id_;
  std::string name_;
  std::string hash_;
  std::string private_key_;
  std::string public_key_;
  MailBox inbox_;
  //MailBox outbox;

  static std::map<std::string, User*> userMap_;
};

}
#endif //Include guard
