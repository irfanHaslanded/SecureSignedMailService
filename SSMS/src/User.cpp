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
  user->outbox_ = std::shared_ptr<MailBox>(new MailBox(user));

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
  std::cerr << "\n===== PASSWORD HASH BEGIN ====="
            << "\n " << id_ << ": " << hash_
            << "\n===== PASSWORD HASH END ====="
            << std::endl;
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
  recipient.inbox_->throwMsg(Msg{this->getId(), msg, recipient.id_});
  outbox_->throwMsg(Msg{this->getId(), msg, recipient.id_});
}

size_t User::showInbox()
{
  return printMsgs(inbox_);
}

size_t User::showOutbox()
{
  return printMsgs(outbox_);
}

size_t User::printMsgs(std::shared_ptr<MailBox> mailBox)
{
  const auto Msgs = mailBox->getMsgs();
  for (const auto &msg : Msgs) {
    const auto user_search =
        userMap_.find(msg.sender_id); // sender might already be deleted or just
                                      // having no name set
    std::cout << "From: " <<  msg.sender_id
              << "\nTo:   " <<  msg.receiver_id
              << "\nMessage:"
              << msg.text
              << "\n---------------"
              << std::endl;
  }
  return Msgs.size();
}
void User::emptyInbox()
{
  inbox_->clear();
}

void User::emptyOutbox()
{
  outbox_->clear();
}

}
