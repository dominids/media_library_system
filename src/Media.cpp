#include "Media.hpp"
#include "Exceptions.hpp"
#include <iomanip>

using namespace std;

Media::Media()
    : id(0), title(""), releaseYear(0), isAvailable(true), borrowedByUserId(-1),
      borrowTime(chrono::system_clock::now()), dueTime(chrono::system_clock::now()) {}

Media::Media(int id, const string& title, int releaseYear)
    : id(id), title(title), releaseYear(releaseYear), isAvailable(true), borrowedByUserId(-1),
      borrowTime(chrono::system_clock::now()), dueTime(chrono::system_clock::now()) {
    if (releaseYear <= 0) {
        throw InvalidDataException("Rok wydania musi byc wiekszy od zera (podano: " + to_string(releaseYear) + ").");
    }
}

int Media::getId() const {
    return id;
}

string Media::getTitle() const {
    return title;
}

int Media::getReleaseYear() const {
    return releaseYear;
}

bool Media::getIsAvailable() const {
    return isAvailable;
}

int Media::getBorrowedByUserId() const {
    return borrowedByUserId;
}

void Media::setTitle(const string& newTitle) {
    if (newTitle.empty()) {
        throw InvalidDataException("Tytul nie moze byc pusty.");
    }
    title = newTitle;
}

void Media::setReleaseYear(int year) {
    if (year <= 0) {
        throw InvalidDataException("Rok wydania musi byc wiekszy od zera (podano: " + to_string(year) + ").");
    }
    releaseYear = year;
}

void Media::setAvailable(bool status) {
    isAvailable = status;
}

void Media::setBorrowedByUserId(int userId) {
    borrowedByUserId = userId;
}

void Media::setBorrowInfo(int userId, chrono::system_clock::time_point currentTime, int loanDays) {
    isAvailable = false;
    borrowedByUserId = userId;
    borrowTime = currentTime;
    dueTime = currentTime + chrono::hours(loanDays * 24);
}

void Media::resetBorrowInfo() {
    isAvailable = true;
    borrowedByUserId = -1;
}

chrono::system_clock::time_point Media::getBorrowTime() const {
    return borrowTime;
}

chrono::system_clock::time_point Media::getDueTime() const {
    return dueTime;
}

int Media::calculateOverdueDays(chrono::system_clock::time_point currentTime) const {
    if (currentTime <= dueTime) {
        return 0;
    }
    auto diffHours = chrono::duration_cast<chrono::hours>(currentTime - dueTime).count();
    // Kazde rozpoczete 24h powyzej terminu liczone sa jako kolejny dzien spoznienia
    int days = static_cast<int>((diffHours + 23) / 24);
    return days > 0 ? days : 1;
}

string Media::getFormattedDueDate() const {
    time_t expTime = chrono::system_clock::to_time_t(dueTime);
    tm* timeinfo = localtime(&expTime);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

void Media::printDetails(ostream& os) const {
    os << "[" << getType() << "] ID: " << id
       << " | Tytuł: \"" << title << "\""
       << " | Rok: " << releaseYear
       << " | Dostępność: ";
    if (isAvailable) {
        os << "Dostępny";
    } else {
        os << "Wypożyczony (ID użytkownika: " << borrowedByUserId << ", termin zwrotu: " << getFormattedDueDate() << ")";
    }
}

bool Media::operator==(const Media& other) const {
    return this->id == other.id;
}

ostream& operator<<(ostream& os, const Media& media) {
    media.printDetails(os);
    return os;
}
