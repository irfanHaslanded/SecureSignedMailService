/* User.h Oskar Paulander 2020-03-
* Class for handling users, including
* creating users, deleting, sending msgs etc.
*/
#ifndef SSMS_USER_H_
#define SSMS_USER_H_

#include "SSMS.h"
#include "MailBox.h"
#include <map>

namespace ssms {

  class User {


  public:
    User(std::string name){};
    ~User();

    std::string toString();

    void setPassword(std::string &password);
    bool login(const std::string &password);
    void logout();

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
    //need to decide size of integers.
    std::string salt;
    int Id;
    std::string hash;     //Todo: fix. Name, type?

    std::map<std::string name, User user> userMap;

    MailBox inbox;
    MailBox outbox;
  };

}
#endif //Include guard