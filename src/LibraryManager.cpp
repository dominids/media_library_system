#include "LibraryManager.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

LibraryManager::LibraryManager()
    : nextMediaId(1), nextUserId(1), simulatedHoursOffset(0) {}

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

int LibraryManager::addMovie(const string& title, int year, const string& director, int durationMinutes, int ageRating, double digitalPrice) {
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
    if (digitalPrice <= 0.0) {
        throw InvalidDataException("Oplata za dystrybucje cyfrowa musi byc wieksza od zera.");
    }

    int id = nextMediaId++;
    catalog.push_back(make_unique<Movie>(id, title, year, director, durationMinutes, ageRating, digitalPrice));
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
        throw MediaUnavailableException("Pozycja o ID " + to_string(mediaId) + " (\"" + media->getTitle() + "\") jest juz wypozyczona fizycznie!");
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

    media->setBorrowInfo(userId, getCurrentTime(), 14); // 14 dni termin zwrotu
}

ReturnInfo LibraryManager::returnMedia(int mediaId) {
    Media* media = findMediaById(mediaId);
    if (!media) {
        throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(mediaId));
    }

    if (media->getIsAvailable()) {
        throw MediaUnavailableException("Pozycja o ID " + to_string(mediaId) + " (\"" + media->getTitle() + "\") nie jest wypozyczona!");
    }

    int daysOverdue = media->calculateOverdueDays(getCurrentTime());
    double fee = media->calculateLateFee(daysOverdue);
    string dueDate = media->getFormattedDueDate();

    media->resetBorrowInfo();
    return {fee, daysOverdue, dueDate};
}

double LibraryManager::rentMovieOnline(int movieId, int userId) {
    // Przed nowym wypozyczeniem czyscimy wygasle subskrypcje
    cleanupAllExpiredDigitalRentals();

    Media* media = findMediaById(movieId);
    if (!media) {
        throw MediaNotFoundException("Nie znaleziono filmu o ID: " + to_string(movieId));
    }

    Movie* movie = dynamic_cast<Movie*>(media);
    if (!movie) {
        throw InvalidDataException("Pozycja o ID " + to_string(movieId) + " (\"" + media->getTitle() + "\") nie jest filmem! Tylko filmy sa dostepne w dystrybucji online (VOD).");
    }

    User* user = findUserById(userId);
    if (!user) {
        throw UserNotFoundException("Nie znaleziono uzytkownika o ID: " + to_string(userId));
    }

    if (user->getAge() < movie->getAgeRating()) {
        throw AgeRestrictionException("Ograniczenie wiekowe VOD: Uzytkownik " + user->getFullName() +
                                      " (wiek: " + to_string(user->getAge()) +
                                      ") nie moze wypozyczyc filmu \"" + movie->getTitle() +
                                      "\" (wymagane: " + to_string(movie->getAgeRating()) + "+ lat)!");
    }

    if (user->hasActiveDigitalRental(movieId, getCurrentTime())) {
        throw MediaUnavailableException("Uzytkownik " + user->getFullName() +
                                       " posiada juz aktywna subskrypcje na film \"" + movie->getTitle() + "\"!");
    }

    DigitalRental rental(movieId, movie->getTitle(), movie->getDigitalPrice(), 48, getCurrentTime());
    user->addDigitalRental(rental);
    return movie->getDigitalPrice();
}

int LibraryManager::cleanupAllExpiredDigitalRentals() {
    int totalCleaned = 0;
    auto nowTime = getCurrentTime();
    for (auto& u : users) {
        totalCleaned += u.cleanupExpiredRentals(nowTime);
    }
    return totalCleaned;
}

void LibraryManager::displayUserDigitalRentals(int userId) const {
    const User* user = findUserById(userId);
    if (!user) {
        throw UserNotFoundException("Nie znaleziono uzytkownika o ID: " + to_string(userId));
    }
    cout << "\n--- SUBSKRYPCJE CYFROWE UZYTKOWNIKA: " << user->getFullName() << " (ID: " << userId << ") ---\n";
    user->displayDigitalRentals(cout, getCurrentTime());
}

void LibraryManager::advanceSimulatedTime(int hours) {
    if (hours <= 0) {
        throw InvalidDataException("Liczba godzin symulacji musi byc wieksza od zera (podano: " + to_string(hours) + ").");
    }
    simulatedHoursOffset += hours;
    int expired = cleanupAllExpiredDigitalRentals();
    cout << "[SYMULACJA CZASU] Przesunieto czas o +" << hours << "h (lacznie: +" << simulatedHoursOffset << "h).\n";
    if (expired > 0) {
        cout << "[AUTOMATYCZNY ZWROT] Automatycznie usunieto " << expired << " wygaslych subskrypcji online po uplywie 48h!\n";
    } else {
        cout << "[AUTOMATYCZNY ZWROT] Brak subskrypcji, ktore wygasly w tym przedziale czasu.\n";
    }
}

int LibraryManager::getSimulatedHoursOffset() const {
    return simulatedHoursOffset;
}

chrono::system_clock::time_point LibraryManager::getCurrentTime() const {
    return chrono::system_clock::now() + chrono::hours(simulatedHoursOffset);
}

void LibraryManager::seedData() {
    addBook("Wiedzmin: Ostatnie zyczenie", 1993, "Andrzej Sapkowski", 288, "978-8375780635");
    addBook("Solaris", 1961, "Stanislaw Lem", 216, "978-8308049006");
    addBook("Czysty Kod", 2008, "Robert C. Martin", 464, "978-8328302341");

    addMovie("Incepcja", 2010, "Christopher Nolan", 148, 12, 14.99);
    addMovie("Ojciec Chrzestny", 1972, "Francis Ford Coppola", 175, 16, 12.99);
    addMovie("Pulp Fiction", 1994, "Quentin Tarantino", 154, 18, 17.99);

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
