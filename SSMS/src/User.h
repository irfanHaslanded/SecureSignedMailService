/* User.h Oskar Paulander 2020-03-
* Class for handling users, including
* creating users, deleting, sending msgs etc.
*/

#include "SSMS.h"

namespace identifier {

  class User {


  public:
    User(std::string name){};
    ~User();

    std::string toString();

    void setPassword(std::string &password){};
    void login();
    void logout();

    MailBox inbox;
    MailBox outbox;

    //enough to store IDs or usernames?
    std::list <User> contacts;
    void addContact(std::string username);
    void addContact(std::string username, std::string nickname);
    void addContact(int userId);
    void addContact(int userId, std::string nickname);

    void listInbox();
    void listMessagesFrom(User sender);

    void displayMessage(int msgId);
    void sendMessage(std::string msg, User recipient);
    void createMessage(std::string msg, User recipient);
    void sendAll();

  private:
    //need to decide size of integers.
    int salt;
    int Id;
    std::string name;
    int password;     //Todo: fix. Name, type?
  }




}