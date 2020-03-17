/* User.h Oskar Paulander 2020-03-
* Class for handling users, including
* creating users, deleting, sending msgs etc.
*/
#ifndef SSMS_USER_H_
#define SSMS_USER_H_

#include <map>

namespace ssms {

  class MailBox;

  class User {

  public:
    User(const std::string& name);
    ~User();

    void setName(const std::string& name);
    void setKeyPair();
    const std::string& getId() const;
    const std::string& getName() const;
    std::string toString() const;
    const std::string& getPrivateKey() const;
    const std::string& getPublicKey() const;

    void setPassword(const std::string& password);
    bool checkPassword(const std::string &password) const;

    //enough to store IDs or usernames?
    void addContact(std::string username);
    void addContact(std::string username, std::string nickname);
    void addContactById(int userId);
    void addContactById(int userId, std::string nickname);

    void showInbox();
    void listMessagesFrom(User sender);

    void displayMessage(int msgId);
    void sendMessage(const std::string& msg, const User& recipient);
    void createMessage(std::string msg, User recipient);
    void deleteMessage(int id);
    void sendAll();

    int getId();

  private:
    std::string id_;
    std::string name_;
    std::string hash_;
    std::string private_key_;
    std::string public_key_;
    MailBox *inbox_;
    //MailBox outbox;

    static std::map<std::string, User*> userMap_;
  };

}
#endif //Include guard