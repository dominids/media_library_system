#include "User.hpp"
#include "Exceptions.hpp"

using namespace std;

User::User() : userId(0), fullName(""), age(0) {}

User::User(int userId, const string& fullName, int age)
    : userId(userId), fullName(fullName), age(age) {
    if (age <= 0) {
        throw InvalidDataException("Wiek uzytkownika musi byc wiekszy od zera (podano: " + to_string(age) + ").");
    }
}

int User::getUserId() const {
    return userId;
}

string User::getFullName() const {
    return fullName;
}

int User::getAge() const {
    return age;
}

void User::setFullName(const string& name) {
    if (name.empty()) {
        throw InvalidDataException("Imie i nazwisko uzytkownika nie moga byc puste.");
    }
    fullName = name;
}

void User::setAge(int newAge) {
    if (newAge <= 0) {
        throw InvalidDataException("Wiek uzytkownika musi byc wiekszy od zera (podano: " + to_string(newAge) + ").");
    }
    age = newAge;
}

ostream& operator<<(ostream& os, const User& user) {
    os << "[Użytkownik] ID: " << user.userId
       << " | Imię i nazwisko: " << user.fullName
       << " | Wiek: " << user.age << " lat";
    return os;
}
