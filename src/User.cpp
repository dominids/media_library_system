#include "User.hpp"
#include "Exceptions.hpp"
#include <algorithm>
#include <iomanip>

using namespace std;

// --- Hash djb2 (prosta implementacja akademicka) ---
static string hashPassword(const string& pwd) {
    unsigned long hash = 5381;
    for (unsigned char c : pwd) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return to_string(hash);
}

// Konstruktory
User::User() : userId(0), fullName(""), age(0), username(""), passwordHash("") {}

User::User(int userId, const string& fullName, int age)
    : userId(userId), fullName(fullName), age(age), username(""), passwordHash("") {
    if (age <= 0) {
        throw InvalidDataException("Wiek uzytkownika musi byc wiekszy od zera (podano: " + to_string(age) + ").");
    }
}

User::User(int userId, const string& fullName, int age,
           const string& username, const string& rawPassword)
    : userId(userId), fullName(fullName), age(age),
      username(username), passwordHash(hashPassword(rawPassword)) {
    if (age <= 0) {
        throw InvalidDataException("Wiek uzytkownika musi byc wiekszy od zera (podano: " + to_string(age) + ").");
    }
    if (username.empty()) {
        throw InvalidDataException("Login uzytkownika nie moze byc pusty.");
    }
    if (rawPassword.empty()) {
        throw InvalidDataException("Haslo uzytkownika nie moze byc puste.");
    }
}

// Gettery
int User::getUserId() const { return userId; }
string User::getFullName() const { return fullName; }
int User::getAge() const { return age; }
string User::getUsername() const { return username; }
string User::getPasswordHash() const { return passwordHash; }

// Settery
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

void User::setUsername(const string& uname) {
    if (uname.empty()) {
        throw InvalidDataException("Login uzytkownika nie moze byc pusty.");
    }
    username = uname;
}

void User::setPasswordHash(const string& hash) {
    passwordHash = hash;
}

void User::setPassword(const string& rawPassword) {
    if (rawPassword.empty()) {
        throw InvalidDataException("Haslo uzytkownika nie moze byc puste.");
    }
    passwordHash = hashPassword(rawPassword);
}

bool User::checkPassword(const string& rawPassword) const {
    return passwordHash == hashPassword(rawPassword);
}

bool User::hasLogin() const {
    return !username.empty() && !passwordHash.empty();
}

// Wynajem cyfrowy
void User::addDigitalRental(const DigitalRental& rental) {
    digitalRentals.push_back(rental);
}

const vector<DigitalRental>& User::getDigitalRentals() const {
    return digitalRentals;
}

vector<DigitalRental>& User::getDigitalRentals() {
    return digitalRentals;
}

bool User::hasActiveDigitalRental(int movieId, chrono::system_clock::time_point currentTime) const {
    for (const auto& rental : digitalRentals) {
        if (rental.getMovieId() == movieId && !rental.isExpired(currentTime)) {
            return true;
        }
    }
    return false;
}

int User::cleanupExpiredRentals(chrono::system_clock::time_point currentTime) {
    size_t before = digitalRentals.size();
    digitalRentals.erase(
        remove_if(digitalRentals.begin(), digitalRentals.end(),
                  [currentTime](const DigitalRental& rental) {
                      return rental.isExpired(currentTime);
                  }),
        digitalRentals.end());
    return static_cast<int>(before - digitalRentals.size());
}

void User::displayDigitalRentals(ostream& os, chrono::system_clock::time_point currentTime) const {
    if (digitalRentals.empty()) {
        os << "   (Brak aktywnych wypozyczen online)\n";
        return;
    }
    for (const auto& rental : digitalRentals) {
        os << "   * Film: \"" << rental.getMovieTitle() << "\" (ID: " << rental.getMovieId() << ")"
           << " | Oplata: " << fixed << setprecision(2) << rental.getFeePaid() << " PLN"
           << " | Wygasa: " << rental.getFormattedExpiration()
           << " (pozostalo: " << rental.getRemainingHours(currentTime) << "h)\n";
    }
}

ostream& operator<<(ostream& os, const User& user) {
    os << "[Uzytkownik] ID: " << user.userId
       << " | Imie i nazwisko: " << user.fullName
       << " | Wiek: " << user.age << " lat";
    if (!user.username.empty()) {
        os << " | Login: " << user.username;
    }
    os << " | Aktywne subskrypcje VOD: " << user.digitalRentals.size();
    return os;
}
