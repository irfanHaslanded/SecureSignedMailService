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

    static int getNextId();
    std::string getName() const;
    std::string toString() const;
    std::string getPrivateKey() const;
    std::string getPublicKey() const;

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
    void sendMessage(std::string msg, User recipient);
    void createMessage(std::string msg, User recipient);
    void deleteMessage(int id);
    void sendAll();

    int getId();

  private:
    int id_;
    std::string name_;
    std::string hash_;
    std::string salt_;
    std::string private_key_;
    std::string public_key_;
    MailBox *inbox_;
    //MailBox outbox;

    static std::map<std::string, User*> userMap_;
    static int nextId_;
  };

}
#endif //Include guard