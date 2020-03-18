#include "User.h"
#include "MailBox.h"
#include "Crypto.h"

namespace ssms {

std::map<std::string, User*> User::userMap_{};

User::User(const std::string& id) : id_(id)
                                  , inbox_(new MailBox(*this))
{
  this->setKeyPair();
  if (!userMap_.emplace(id_, this).second)
  {
    throw "User with id '" + id + "' already exists";
  }
}

User::~User()
{
  userMap_.erase(id_);
  delete inbox_;
}

void User::setName(const std::string& name)
{
  name_ = name;
}

void User::setKeyPair()
{
  private_key_ = "TODO: generate private key";
  public_key_ = "TODO: generate public key";
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

void User::sendMessage(const User& recipient, const std::string& msg)
{
  recipient.inbox_->throwMsg(*this, msg);
}

size_t User::showInbox()
{
  const auto receivedMsgs = inbox_->getReceivedMsgs();
  for (const auto& it : receivedMsgs)
  {
    const auto search = userMap_.find(it.first); // sender might already be deleted or just having no name set
    std::cout << (search != userMap_.end() && !search->second->getName().empty()
                     ? search->second->getName()
                     : it.first)
              << ": "
              << Crypto::decrypt(it.second, private_key_)
              << std::endl;
  }
  return receivedMsgs.size();
}

}