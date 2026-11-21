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
    vector<DigitalRental> digitalRentals;

public:
    User();
    User(int userId, const string& fullName, int age);

    int getUserId() const;
    string getFullName() const;
    int getAge() const;

    void setFullName(const string& name);
    void setAge(int newAge);

    void addDigitalRental(const DigitalRental& rental);
    const vector<DigitalRental>& getDigitalRentals() const;

    bool hasActiveDigitalRental(int movieId, chrono::system_clock::time_point currentTime = chrono::system_clock::now()) const;
    int cleanupExpiredRentals(chrono::system_clock::time_point currentTime = chrono::system_clock::now());
    void displayDigitalRentals(ostream& os = cout, chrono::system_clock::time_point currentTime = chrono::system_clock::now()) const;

    friend ostream& operator<<(ostream& os, const User& user);
};

#endif // USER_HPP
