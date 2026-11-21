#ifndef USER_HPP
#define USER_HPP

#include "DigitalRental.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class User {
private:
    int userId;
    string fullName;
    int age;
    string username;      // login do panelu uzytkownika
    string passwordHash;  // hash hasla (djb2)
    vector<DigitalRental> digitalRentals;

public:
    User();
    User(int userId, const string& fullName, int age);
    User(int userId, const string& fullName, int age,
         const string& username, const string& rawPassword);

    int getUserId() const;
    string getFullName() const;
    int getAge() const;
    string getUsername() const;
    string getPasswordHash() const;

    void setFullName(const string& name);
    void setAge(int newAge);
    void setUsername(const string& uname);
    void setPasswordHash(const string& hash); // uzywane przy deserializacji

    void setPassword(const string& rawPassword);
    bool checkPassword(const string& rawPassword) const;
    bool hasLogin() const;

    void addDigitalRental(const DigitalRental& rental);
    const vector<DigitalRental>& getDigitalRentals() const;
    vector<DigitalRental>& getDigitalRentals();

    bool hasActiveDigitalRental(int movieId, chrono::system_clock::time_point currentTime = chrono::system_clock::now()) const;
    int cleanupExpiredRentals(chrono::system_clock::time_point currentTime = chrono::system_clock::now());
    void displayDigitalRentals(ostream& os = cout, chrono::system_clock::time_point currentTime = chrono::system_clock::now()) const;

    friend ostream& operator<<(ostream& os, const User& user);
};

#endif // USER_HPP
