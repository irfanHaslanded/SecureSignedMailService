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

std::string User::getName() const
{
  return name_;
}

std::string User::toString() const
{
  return getName();
}

std::string User::getPrivateKey() const
{
  return private_key_;
}

std::string User::getPublicKey() const
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

void User::sendMessage(const std::string& msg, const User& recipient)
{
  recipient.inbox_->throwMsg(msg);
}

void User::showInbox()
{
  for (const auto& it : inbox_->getReceivedMsgs())
  {
    auto search = userMap_.find(it.first); // sender might already be deleted or just having no name set
    std::cout << "From: "
              << (search != userMap_.end() && !search->second->getName().empty()
                     ? search->second->getName()
                     : it.first)
              << " Msg: "
              << Crypto::decrypt(it.second, private_key_)
              << std::endl;
  }
}

}