#include "User.h"
#include "MailBox.h"
#include "Crypto.h"

namespace ssms {

// Exception
const char* UserAlreadyExists::what() const throw()
{
  return "User already exists";
}
UserAlreadyExists::UserAlreadyExists(const std::string& id) : id_(id) {}
std::string UserAlreadyExists::getId() { return id_; }

// Static members
std::map<std::string, std::shared_ptr<User>> User::userMap_{};

// Member methods
User::User(const std::string& id) : id_(id), name_(id) {}

User::~User() {}

std::shared_ptr<User> User::create(const std::string& id)
{
  std::shared_ptr<User> user(new User(id));
  user->inbox_ = std::shared_ptr<MailBox>(new MailBox(user));

  if (!Crypto::generateRsaKeypair(user->private_key_, user->public_key_))
  {
    throw std::runtime_error("Key-pair generation failed");
  }

  if (!userMap_.emplace(id, user).second)
  {
    throw UserAlreadyExists(id);
  }

  return user;
}

std::shared_ptr<User> User::get(const std::string& id)
{
  auto user_search = userMap_.find(id);
  return user_search == userMap_.end() ? nullptr : user_search->second;
}

std::list<std::string> User::getIdList()
{
  std::list<std::string> user_list;
  for (const auto& it : userMap_)
  {
    user_list.push_back(it.first);
  }
  return user_list;
}

void User::remove(const std::string& id)
{
  (void)userMap_.erase(id);
}

void User::removeAll()
{
  userMap_.clear();
}

void User::setName(const std::string& name)
{
  name_ = name;
}

const std::string& User::getId() const
{
  return id_;
}

const std::string& User::getName() const
{
  return name_;
}

std::string User::toString() const
{
  return name_;
}

const std::string& User::getPrivateKey() const
{
  return private_key_;
}

const std::string& User::getPublicKey() const
{
  return public_key_;
}

void User::setPassword(const std::string &password)
{
  hash_ = Crypto::genHash(Crypto::passTheSalt(), password);
}

bool User::checkPassword(const std::string &password) const
{
  return Crypto::validatePassword(hash_, password);
}

bool User::sendMessage(const std::string& recipient_id, const std::string& msg)
{
  auto user_search = userMap_.find(recipient_id);
  if (user_search == userMap_.end())
  {
    return false;
  }
  sendMessage(*user_search->second, msg);
  return true;
}

void User::sendMessage(User& recipient, const std::string& msg)
{
  recipient.inbox_->throwMsg(Msg{this->getId(), msg});
}

size_t User::showInbox()
{
  const auto receivedMsgs = inbox_->getReceivedMsgs();
  for (const auto &msg : receivedMsgs) {
    const auto user_search =
        userMap_.find(msg.sender_id); // sender might already be deleted or just
                                      // having no name set
    std::cout << (user_search != userMap_.end() && !user_search->second->getName().empty()
                      ? user_search->second->getName() + " <" + msg.sender_id + ">"
                      : msg.sender_id)
              << ": " << msg.text << std::endl;
  }
  return receivedMsgs.size();
}

void User::emptyInbox()
{
  inbox_->clear();
}

}
