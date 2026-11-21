#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

using namespace std;

class User {
private:
    int userId;
    string fullName;
    int age;

public:
    User();
    User(int userId, const string& fullName, int age);

    int getUserId() const;
    string getFullName() const;
    int getAge() const;

    void setFullName(const string& name);
    void setAge(int newAge);

    friend ostream& operator<<(ostream& os, const User& user);
};

#endif // USER_HPP
