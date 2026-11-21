#include "User.hpp"
#include "Exceptions.hpp"
#include <algorithm>
#include <iomanip>

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

void User::addDigitalRental(const DigitalRental& rental) {
    digitalRentals.push_back(rental);
}

const vector<DigitalRental>& User::getDigitalRentals() const {
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
    os << "[Użytkownik] ID: " << user.userId
       << " | Imię i nazwisko: " << user.fullName
       << " | Wiek: " << user.age << " lat"
       << " | Aktywne subskrypcje VOD: " << user.digitalRentals.size();
    return os;
}
