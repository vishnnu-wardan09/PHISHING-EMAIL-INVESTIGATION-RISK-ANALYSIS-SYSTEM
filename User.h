#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User
{
private:
    int userId;
    string username;
    string password;

public:
    User();
    User(int id, string username, string password);

    void setUserId(int id);
    void setUsername(string username);
    void setPassword(string password);

    int getUserId();
    string getUsername();
    string getPassword();
};

#endif