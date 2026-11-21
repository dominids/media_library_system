#include "Media.hpp"

using namespace std;

Media::Media()
    : id(0), title(""), releaseYear(0), isAvailable(true), borrowedByUserId(-1) {}

Media::Media(int id, const string& title, int releaseYear)
    : id(id), title(title), releaseYear(releaseYear), isAvailable(true), borrowedByUserId(-1) {}

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
    title = newTitle;
}

void Media::setReleaseYear(int year) {
    releaseYear = year;
}

void Media::setAvailable(bool status) {
    isAvailable = status;
}

void Media::setBorrowedByUserId(int userId) {
    borrowedByUserId = userId;
}

void Media::printDetails(ostream& os) const {
    os << "[" << getType() << "] ID: " << id
       << " | Tytuł: \"" << title << "\""
       << " | Rok: " << releaseYear
       << " | Dostępność: " << (isAvailable ? "Dostępny" : "Wypożyczony (ID użytkownika: " + to_string(borrowedByUserId) + ")");
}

bool Media::operator==(const Media& other) const {
    return this->id == other.id;
}

ostream& operator<<(ostream& os, const Media& media) {
    media.printDetails(os);
    return os;
}
