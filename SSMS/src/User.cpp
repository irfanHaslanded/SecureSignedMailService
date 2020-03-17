#include "User.h"
#include "MailBox.h"
#include "Crypto.h"

namespace ssms {

int User::nextId_(0);
std::map<std::string, User*> User::userMap_{};

User::User(const std::string& name) : name_(name)
                                    , id_(getNextId())
                                    , inbox_(new MailBox(*this))
{
  userMap_.emplace(name, this);
}

User::~User()
{
  userMap_.erase(name_);
  delete inbox_;
}

int User::getNextId()
{
  return nextId_++;
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

void User::showInbox()
{
  for (const auto& it : inbox_->getReceivedMsgs())
  {
    std::cout << "From: " << it.first << " Msg: " << Crypto::decrypt(it.second, private_key_) << std::endl;
  }
}

}