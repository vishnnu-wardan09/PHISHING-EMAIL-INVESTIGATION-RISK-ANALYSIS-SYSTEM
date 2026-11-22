#include "../include/User.h"

User::User()
{
    userId = 0;
    username = "";
    password = "";
}

User::User(int id, string username, string password)
{
    userId = id;
    this->username = username;
    this->password = password;
}

void User::setUserId(int id)
{
    userId = id;
}

void User::setUsername(string username)
{
    this->username = username;
}

void User::setPassword(string password)
{
    this->password = password;
}

int User::getUserId()
{
    return userId;
}

string User::getUsername()
{
    return username;
}

string User::getPassword()
{
    return password;
}