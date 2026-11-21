#include "LibraryManager.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

LibraryManager::LibraryManager() : nextMediaId(1), nextUserId(1) {}

int LibraryManager::addBook(const string& title, int year, const string& author, int pageCount, const string& isbn) {
    int id = nextMediaId++;
    catalog.push_back(make_unique<Book>(id, title, year, author, pageCount, isbn));
    return id;
}

int LibraryManager::addMovie(const string& title, int year, const string& director, int durationMinutes, int ageRating) {
    int id = nextMediaId++;
    catalog.push_back(make_unique<Movie>(id, title, year, director, durationMinutes, ageRating));
    return id;
}

int LibraryManager::addUser(const string& fullName, int age) {
    int id = nextUserId++;
    users.push_back(User(id, fullName, age));
    return id;
}

bool LibraryManager::removeMedia(int id) {
    auto it = find_if(catalog.begin(), catalog.end(), [id](const unique_ptr<Media>& item) {
        return item->getId() == id;
    });

    if (it != catalog.end()) {
        catalog.erase(it);
        return true;
    }
    return false;
}

Media* LibraryManager::findMediaById(int id) {
    for (auto& item : catalog) {
        if (item->getId() == id) {
            return item.get();
        }
    }
    return nullptr;
}

const Media* LibraryManager::findMediaById(int id) const {
    for (const auto& item : catalog) {
        if (item->getId() == id) {
            return item.get();
        }
    }
    return nullptr;
}

User* LibraryManager::findUserById(int userId) {
    for (auto& u : users) {
        if (u.getUserId() == userId) {
            return &u;
        }
    }
    return nullptr;
}

const User* LibraryManager::findUserById(int userId) const {
    for (const auto& u : users) {
        if (u.getUserId() == userId) {
            return &u;
        }
    }
    return nullptr;
}

bool LibraryManager::updateMediaTitle(int id, const string& newTitle) {
    Media* media = findMediaById(id);
    if (media) {
        media->setTitle(newTitle);
        return true;
    }
    return false;
}

void LibraryManager::displayAllMedia() const {
    if (catalog.empty()) {
        cout << "Katalog mediow jest pusty.\n";
        return;
    }
    cout << "\n--- KATALOG MEDIOW (" << catalog.size() << ") ---\n";
    for (const auto& item : catalog) {
        cout << *item << "\n";
    }
}

void LibraryManager::displayAllUsers() const {
    if (users.empty()) {
        cout << "Brak zarejestrowanych uzytkownikow.\n";
        return;
    }
    cout << "\n--- LISTA UZYTKOWNIKOW (" << users.size() << ") ---\n";
    for (const auto& u : users) {
        cout << u << "\n";
    }
}

bool LibraryManager::borrowMedia(int mediaId, int userId) {
    Media* media = findMediaById(mediaId);
    if (!media) return false;

    User* user = findUserById(userId);
    if (!user) return false;

    if (!media->getIsAvailable()) return false;

    media->setAvailable(false);
    media->setBorrowedByUserId(userId);
    return true;
}

double LibraryManager::returnMedia(int mediaId, int daysOverdue) {
    Media* media = findMediaById(mediaId);
    if (!media || media->getIsAvailable()) return -1.0;

    double fee = media->calculateLateFee(daysOverdue);
    media->setAvailable(true);
    media->setBorrowedByUserId(-1);
    return fee;
}

void LibraryManager::seedData() {
    addBook("Wiedzmin: Ostatnie zyczenie", 1993, "Andrzej Sapkowski", 288, "978-8375780635");
    addBook("Solaris", 1961, "Stanislaw Lem", 216, "978-8308049006");
    addBook("Czysty Kod", 2008, "Robert C. Martin", 464, "978-8328302341");

    addMovie("Incepcja", 2010, "Christopher Nolan", 148, 12);
    addMovie("Ojciec Chrzestny", 1972, "Francis Ford Coppola", 175, 16);
    addMovie("Pulp Fiction", 1994, "Quentin Tarantino", 154, 18);

    addUser("Jan Kowalski", 25);
    addUser("Anna Nowak", 15);
    addUser("Piotr Wisniewski", 10);
}

const vector<unique_ptr<Media>>& LibraryManager::getCatalog() const {
    return catalog;
}

const vector<User>& LibraryManager::getUsers() const {
    return users;
}
