/* User.h Oskar Paulander 2020-03-
* Class for handling users, including
* creating users, deleting, sending msgs etc.
*/
#ifndef SSMS_USER_H_
#define SSMS_USER_H_

#include "MailBox.h"

#include <map>
#include <exception>
#include <memory>

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
  User() = delete;
  virtual ~User();

  static std::shared_ptr<User> create(const std::string& id);
  static std::shared_ptr<User> get(const std::string& id);
  static std::list<std::string> getIdList();
  static void remove(const std::string& id);
  static void removeAll();

  const std::string& getId() const;
  void setName(const std::string& name);
  const std::string& getName() const;
  std::string toString() const;

  const std::string& getPrivateKey() const;
  const std::string& getPublicKey() const;

  void setPassword(const std::string& password);
  bool checkPassword(const std::string &password) const;

  bool sendMessage(const std::string& recipient_id, const std::string& msg);
  void sendMessage(User& recipient, const std::string& msg);
  size_t showInbox();
  void emptyInbox();

private:
  User(const std::string& id);

private:
  std::string id_;
  std::string name_;
  std::string hash_;
  std::string private_key_;
  std::string public_key_;
  std::shared_ptr<MailBox> inbox_;
  //MailBox outbox;

  static std::map<std::string, std::shared_ptr<User>> userMap_;
};

}
#endif //Include guard
