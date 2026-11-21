#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "LibraryManager.hpp"
#include "LibrarySerializer.hpp"
#include "Exceptions.hpp"

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <filesystem>

using namespace std;

static string getDataFilePath() {
    if (filesystem::exists("data/library.dat") || filesystem::exists("data")) {
        return "data/library.dat";
    }
    if (filesystem::exists("../data/library.dat") || filesystem::exists("../data")) {
        return "../data/library.dat";
    }
    return "data/library.dat";
}

static void syncLoad(LibraryManager& library) {
    string path = getDataFilePath();
    if (filesystem::exists(path)) {
        LibrarySerializer::loadFromFile(path, library);
    } else {
        filesystem::create_directories(filesystem::path(path).parent_path());
        library.seedData();
        LibrarySerializer::saveToFile(path, library);
    }
}

static void syncSave(const LibraryManager& library) {
    string path = getDataFilePath();
    filesystem::create_directories(filesystem::path(path).parent_path());
    LibrarySerializer::saveToFile(path, library);
}

static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

static string removeBom(string s) {
    if (s.size() >= 3 &&
        static_cast<unsigned char>(s[0]) == 0xEF &&
        static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF) {
        s = s.substr(3);
    }
    return trim(s);
}

static int readInt(const string& prompt, int minValue = numeric_limits<int>::min(), int maxValue = numeric_limits<int>::max()) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            return minValue;
        }
        string s = removeBom(line);
        if (s.empty()) {
            cout << "[BLAD] Pole nie moze byc puste! Wprowadz liczbe calkowita.\n";
            continue;
        }

        try {
            size_t idx = 0;
            int val = stoi(s, &idx);
            if (idx != s.length()) {
                cout << "[BLAD] Niepoprawny format! Wprowadz sama liczbe calkowita.\n";
                continue;
            }
            if (val < minValue) {
                cout << "[BLAD] Wartosc nie moze byc mniejsza niz " << minValue << "! Sprobuj ponownie.\n";
                continue;
            }
            if (val > maxValue) {
                cout << "[BLAD] Wartosc nie moze byc wieksza niz " << maxValue << "! Sprobuj ponownie.\n";
                continue;
            }
            return val;
        } catch (const out_of_range&) {
            cout << "[BLAD] Podana liczba wykracza poza dozwolony zakres pamieci (overflow)!\n";
        } catch (const invalid_argument&) {
            cout << "[BLAD] Niepoprawna wartosc! Wprowadz liczbe calkowita.\n";
        }
    }
}

static string readLine(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            return "";
        }
        string s = removeBom(line);
        if (!s.empty()) {
            return s;
        }
        cout << "[BLAD] Pole nie moze byc puste ani skladac sie z samych spacji! Sprobuj ponownie.\n";
    }
}

static bool askYesNo(const string& question) {
    while (true) {
        cout << question << " (t/n): ";
        string line;
        if (!getline(cin, line)) {
            return false;
        }
        string s = removeBom(line);
        if (s == "t" || s == "T" || s == "tak" || s == "TAK" || s == "y" || s == "Y") return true;
        if (s == "n" || s == "nie" || s == "NIE" || s == "no" || s == "NO" || s.empty()) return false;
        cout << "[BLAD] Wprowadz 't' (tak) lub 'n' (nie).\n";
    }
}

static void printGuestMenu() {
    cout << "\n========================================\n";
    cout << "     PANEL UZYTKOWNIKA - WYPOZYCZALNIA  \n";
    cout << "========================================\n";
    cout << " 1. Zaloguj sie\n";
    cout << " 2. Zarejestruj nowe konto (z haszowaniem hasla)\n";
    cout << " 3. Przegladaj katalog mediow\n";
    cout << " 4. Wyjscie z programu\n";
    cout << "========================================\n";
}

static void printUserMenu(const User& user) {
    cout << "\n========================================\n";
    cout << "  PANEL UZYTKOWNIKA | Zalogowany: " << user.getFullName() << "\n";
    cout << "  Login: " << user.getUsername() << " | Wiek: " << user.getAge() << " lat | ID: " << user.getUserId() << "\n";
    cout << "========================================\n";
    cout << " 1. Przegladaj caly katalog mediow\n";
    cout << " 2. Wypozycz film online (VOD na 48h)\n";
    cout << " 3. Przedluz za darmo wypozyczenie (o +7 dni)\n";
    cout << " 4. Moje aktywne subskrypcje cyfrowe (VOD)\n";
    cout << " 5. Moje wypozyczenia fizyczne\n";
    cout << " 6. Wyloguj sie\n";
    cout << " 7. Wyjscie z programu\n";
    cout << "========================================\n";
}

static void displayUserPhysicalLoans(const LibraryManager& library, int userId) {
    cout << "\n--- TWOJE WYPOZYCZENIA FIZYCZNE ---\n";
    int count = 0;
    for (const auto& item : library.getCatalog()) {
        if (!item->getIsAvailable() && item->getBorrowedByUserId() == userId) {
            cout << " * ID: " << item->getId()
                 << " | [" << item->getType() << "] \"" << item->getTitle() << "\""
                 << " | Termin zwrotu: " << item->getFormattedDueDate() << "\n";
            count++;
        }
    }
    if (count == 0) {
        cout << " (Brak aktywnych wypozyczen fizycznych)\n";
    }
}

static void displayVodMovies(const LibraryManager& library) {
    cout << "\n--- DOSTEPNE FILMY W DYSTRYBUCJI ONLINE (VOD) ---\n";
    int count = 0;
    for (const auto& item : library.getCatalog()) {
        const Movie* m = dynamic_cast<const Movie*>(item.get());
        if (m) {
            cout << " * ID: " << m->getId()
                 << " | \"" << m->getTitle() << "\" (" << m->getReleaseYear() << ")"
                 << " | Rezyser: " << m->getDirector()
                 << " | Czas: " << m->getDurationMinutes() << " min"
                 << " | Wiek: " << m->getAgeRating() << "+"
                 << " | Cena VOD 48h: " << fixed << setprecision(2) << m->getDigitalPrice() << " PLN\n";
            count++;
        }
    }
    if (count == 0) {
        cout << " (Brak filmow w katalogu)\n";
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    LibraryManager library;
    syncLoad(library);

    cout << "====================================================\n";
    cout << " WITAJ W PANELU UZYTKOWNIKA WYPOZYCZALNI MEDIOW\n";
    cout << "====================================================\n";

    int loggedUserId = -1;
    bool running = true;

    while (running) {
        syncLoad(library);

        if (loggedUserId == -1) {
            printGuestMenu();
            int choice = readInt("Wybierz opcje (1-4): ", 1, 4);

            switch (choice) {
                case 1: { // Logowanie
                    cout << "\n--- LOGOWANIE DO SYSTEMU ---\n";
                    string uname = readLine("Podaj login: ");
                    string pass = readLine("Podaj haslo: ");

                    syncLoad(library);
                    User* user = library.findUserByLogin(uname);
                    if (!user) {
                        cout << "[BLAD] Nie znaleziono uzytkownika o loginie: \"" << uname << "\"!\n";
                        break;
                    }
                    if (!user->checkPassword(pass)) {
                        cout << "[BLAD] Nieprawidlowe haslo! Odmowa dostepu.\n";
                        break;
                    }

                    loggedUserId = user->getUserId();
                    cout << "\n[SUKCES] Zalogowano pomyslnie! Witaj, " << user->getFullName() << "!\n";
                    break;
                }
                case 2: { // Rejestracja nowego konta
                    cout << "\n--- REJESTRACJA NOWEGO KONTA ---\n";
                    string name = readLine("Podaj imie i nazwisko: ");
                    int age = readInt("Podaj wiek: ", 1, 150);
                    string uname = readLine("Podaj wymarzony login: ");
                    string pass = readLine("Podaj haslo: ");

                    syncLoad(library);
                    if (library.loginExists(uname)) {
                        cout << "[BLAD] Uzytkownik o loginie \"" << uname << "\" juz istnieje! Wybierz inny login.\n";
                        break;
                    }

                    try {
                        int newId = library.addUserWithLogin(name, age, uname, pass);
                        syncSave(library);
                        cout << "\n[SUKCES] Konto zostalo utworzone! Przypisane ID: " << newId << ".\n";
                        cout << "[INFO] Haslo zostalo bezpiecznie zahaszowane.\n";
                        cout << "Mozesz sie teraz zalogowac.\n";
                    } catch (const exception& e) {
                        cout << "[BLAD REJESTRACJI] " << e.what() << "\n";
                    }
                    break;
                }
                case 3: { // Przegladaj katalog
                    syncLoad(library);
                    library.displayAllMedia();
                    break;
                }
                case 4: { // Wyjscie
                    cout << "Dziekujemy za skorzystanie z panelu uzytkownika. Do widzenia!\n";
                    running = false;
                    break;
                }
            }
        } else {
            // Uzytkownik zalogowany
            syncLoad(library);
            User* currentUser = library.findUserById(loggedUserId);
            if (!currentUser) {
                cout << "[BLAD] Sesja wygasla lub uzytkownik zostal usuniety.\n";
                loggedUserId = -1;
                continue;
            }

            printUserMenu(*currentUser);
            int choice = readInt("Wybierz opcje (1-7): ", 1, 7);

            try {
                switch (choice) {
                    case 1: { // Przegladaj caly katalog
                        library.displayAllMedia();
                        break;
                    }
                    case 2: { // Wypozycz film cyfrowo VOD
                        cout << "\n--- WYPOZYCZENIE FILMU ONLINE (VOD 48h) ---\n";
                        if (askYesNo("Czy chcesz wyswietlic katalog dostepnych filmow VOD?")) {
                            displayVodMovies(library);
                        }
                        int movieId = readInt("Podaj ID filmu do wypozyczenia online: ", 1);

                        double fee = library.rentMovieOnline(movieId, loggedUserId);
                        syncSave(library);
                        Media* m = library.findMediaById(movieId);
                        cout << "\n[SUKCES] Wypozyczono film \"" << (m ? m->getTitle() : "") << "\" w usludze VOD!\n";
                        cout << "Pobrano oplate cyfrowa: " << fixed << setprecision(2) << fee << " PLN.\n";
                        cout << "Dostep do filmu wygasa automatycznie po 48 godzinach.\n";
                        break;
                    }
                    case 3: { // Przedluz wypozyczenie o tydzien
                        cout << "\n--- BEZPLATNE PRZEDLUZENIE TERMINU WYPOZYCZENIA (+7 DNI) ---\n";
                        if (askYesNo("Czy chcesz wyswietlic liste Twoich wypozyczen fizycznych?")) {
                            displayUserPhysicalLoans(library, loggedUserId);
                        }
                        int mediaId = readInt("Podaj ID wypozyczonej pozycji: ", 1);

                        library.extendLoan(mediaId, loggedUserId);
                        syncSave(library);
                        Media* m = library.findMediaById(mediaId);
                        cout << "\n[SUKCES] Pomyslnie przedluzono wypozyczenie pozycji \"" 
                             << (m ? m->getTitle() : "") << "\" o 7 dni!\n";
                        if (m) {
                            cout << "Nowy termin zwrotu: " << m->getFormattedDueDate() << "\n";
                        }
                        break;
                    }
                    case 4: { // Moje aktywne subskrypcje
                        library.cleanupAllExpiredDigitalRentals();
                        syncSave(library);
                        library.displayUserDigitalRentals(loggedUserId);
                        break;
                    }
                    case 5: { // Moje wypozyczenia fizyczne
                        displayUserPhysicalLoans(library, loggedUserId);
                        break;
                    }
                    case 6: { // Wyloguj sie
                        cout << "\n[INFO] Wylogowano pomyslnie. Do zobaczenia, " << currentUser->getFullName() << "!\n";
                        loggedUserId = -1;
                        break;
                    }
                    case 7: { // Wyjscie
                        cout << "Dziekujemy za skorzystanie z panelu uzytkownika. Do widzenia!\n";
                        running = false;
                        break;
                    }
                }
            } catch (const LibraryException& e) {
                cout << "\n[BLAD OPERACJI] " << e.what() << "\n";
            } catch (const exception& e) {
                cout << "\n[NIEOCZEKIWANY BLAD] " << e.what() << "\n";
            }
        }
    }

    return 0;
}
