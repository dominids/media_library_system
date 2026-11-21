#include "LibraryManager.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

LibraryManager::LibraryManager() : nextMediaId(1), nextUserId(1) {}

int LibraryManager::addBook(const string& title, int year, const string& author, int pageCount, const string& isbn) {
    if (pageCount <= 0) {
        throw InvalidDataException("Liczba stron ksiazki musi byc wieksza od zera (podano: " + to_string(pageCount) + ").");
    }
    if (year <= 0) {
        throw InvalidDataException("Rok wydania musi byc liczba dodatnia (podano: " + to_string(year) + ").");
    }
    if (title.empty() || author.empty() || isbn.empty()) {
        throw InvalidDataException("Pola tytul, autor oraz ISBN nie moga byc puste.");
    }

    int id = nextMediaId++;
    catalog.push_back(make_unique<Book>(id, title, year, author, pageCount, isbn));
    return id;
}

int LibraryManager::addMovie(const string& title, int year, const string& director, int durationMinutes, int ageRating) {
    if (durationMinutes <= 0) {
        throw InvalidDataException("Czas trwania filmu musi byc wiekszy od zera (podano: " + to_string(durationMinutes) + " min).");
    }
    if (ageRating < 0) {
        throw InvalidDataException("Ograniczenie wiekowe filmu nie moze byc ujemne (podano: " + to_string(ageRating) + ").");
    }
    if (year <= 0) {
        throw InvalidDataException("Rok wydania musi byc liczba dodatnia (podano: " + to_string(year) + ").");
    }
    if (title.empty() || director.empty()) {
        throw InvalidDataException("Pola tytul oraz rezyser nie moga byc puste.");
    }

    int id = nextMediaId++;
    catalog.push_back(make_unique<Movie>(id, title, year, director, durationMinutes, ageRating));
    return id;
}

int LibraryManager::addUser(const string& fullName, int age) {
    if (age <= 0) {
        throw InvalidDataException("Wiek uzytkownika musi byc wiekszy od zera (podano: " + to_string(age) + ").");
    }
    if (fullName.empty()) {
        throw InvalidDataException("Imie i nazwisko uzytkownika nie moga byc puste.");
    }

    int id = nextUserId++;
    users.push_back(User(id, fullName, age));
    return id;
}

void LibraryManager::removeMedia(int id) {
    auto it = find_if(catalog.begin(), catalog.end(), [id](const unique_ptr<Media>& item) {
        return item->getId() == id;
    });

    if (it != catalog.end()) {
        catalog.erase(it);
        return;
    }
    throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(id));
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

void LibraryManager::updateMediaTitle(int id, const string& newTitle) {
    if (newTitle.empty()) {
        throw InvalidDataException("Nowy tytul nie moze byc pusty.");
    }
    Media* media = findMediaById(id);
    if (!media) {
        throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(id));
    }
    media->setTitle(newTitle);
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

void LibraryManager::borrowMedia(int mediaId, int userId) {
    Media* media = findMediaById(mediaId);
    if (!media) {
        throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(mediaId));
    }

    User* user = findUserById(userId);
    if (!user) {
        throw UserNotFoundException("Nie znaleziono uzytkownika o ID: " + to_string(userId));
    }

    if (!media->getIsAvailable()) {
        throw MediaUnavailableException("Pozycja o ID " + to_string(mediaId) + " (\"" + media->getTitle() + "\") jest juz wypozyczona!");
    }

    // Swiadome uzycie dynamic_cast do sprawdzenia typu pochodnego Movie i weryfikacji ograniczenia wiekowego
    if (const Movie* movie = dynamic_cast<const Movie*>(media)) {
        if (user->getAge() < movie->getAgeRating()) {
            throw AgeRestrictionException("Ograniczenie wiekowe: Uzytkownik " + user->getFullName() +
                                          " (wiek: " + to_string(user->getAge()) +
                                          ") nie spelnia wymogu wiekowego dla filmu \"" +
                                          movie->getTitle() + "\" (wymagane: " +
                                          to_string(movie->getAgeRating()) + "+ lat)!");
        }
    }

    media->setAvailable(false);
    media->setBorrowedByUserId(userId);
}

double LibraryManager::returnMedia(int mediaId, int daysOverdue) {
    if (daysOverdue < 0) {
        throw InvalidDataException("Liczba dni spoznienia nie moze byc ujemna (podano: " + to_string(daysOverdue) + ").");
    }

    Media* media = findMediaById(mediaId);
    if (!media) {
        throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(mediaId));
    }

    if (media->getIsAvailable()) {
        throw MediaUnavailableException("Pozycja o ID " + to_string(mediaId) + " (\"" + media->getTitle() + "\") nie jest wypozyczona!");
    }

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
