#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "LibraryManager.hpp"
#include "Exceptions.hpp"

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

string removeBom(string s) {
    if (s.size() >= 3 &&
        static_cast<unsigned char>(s[0]) == 0xEF &&
        static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF) {
        s = s.substr(3);
    }
    return trim(s);
}

int readInt(const string& prompt, int minValue = numeric_limits<int>::min(), int maxValue = numeric_limits<int>::max()) {
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

double readDouble(const string& prompt, double minValue = 0.0, double maxValue = numeric_limits<double>::max()) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            return minValue;
        }
        string s = removeBom(line);
        if (s.empty()) {
            cout << "[BLAD] Pole nie moze byc puste! Wprowadz liczbe.\n";
            continue;
        }

        try {
            size_t idx = 0;
            double val = stod(s, &idx);
            if (idx != s.length()) {
                cout << "[BLAD] Niepoprawny format! Wprowadz prawidlowa kwote.\n";
                continue;
            }
            if (val < minValue) {
                cout << "[BLAD] Wartosc nie moze byc mniejsza niz " << fixed << setprecision(2) << minValue << "! Sprobuj ponownie.\n";
                continue;
            }
            if (val > maxValue) {
                cout << "[BLAD] Wartosc nie moze byc wieksza niz " << fixed << setprecision(2) << maxValue << "! Sprobuj ponownie.\n";
                continue;
            }
            return val;
        } catch (const out_of_range&) {
            cout << "[BLAD] Podana liczba wykracza poza dozwolony zakres pamieci!\n";
        } catch (const invalid_argument&) {
            cout << "[BLAD] Niepoprawna wartosc! Wprowadz liczbe (np. 14.99).\n";
        }
    }
}

string readLine(const string& prompt) {
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

void printMenu() {
    cout << "\n========================================\n";
    cout << "  SYSTEM ZARZADZANIA WYPOZYCZALNIA MEDIOW\n";
    cout << "========================================\n";
    cout << " 1. Wyswietl caly katalog mediow\n";
    cout << " 2. Dodaj nowa ksiazke\n";
    cout << " 3. Dodaj nowy film (wraz z oplata cyfrowa VOD)\n";
    cout << " 4. Edytuj tytul zasobu\n";
    cout << " 5. Usun zasob z katalogu\n";
    cout << " 6. Zarzadzaj uzytkownikami (Rejestracja / Lista)\n";
    cout << " 7. Wypozycz fizycznie pozycje uzytkownikowi\n";
    cout << " 8. Zwroc fizyczna pozycje (kalkulacja kary)\n";
    cout << " 9. Wypozycz film ONLINE (dystrybucja cyfrowa na 48h)\n";
    cout << " 10. Wyswietl aktywne subskrypcje online uzytkownika\n";
    cout << " 11. Symuluj uplyw czasu (+godziny) i automatyczny zwrot VOD\n";
    cout << " 12. Porownaj dwa zasoby (demonstracja operator==)\n";
    cout << " 13. Wyjscie z programu\n";
    cout << "========================================\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    LibraryManager library;
    cout << "[INFO] Inicjalizacja biblioteki danymi demonstracyjnymi (seedData)... ";
    library.seedData();
    cout << "Gotowe!\n";

    bool running = true;

    while (running) {
        printMenu();
        int choice = readInt("Wybierz opcje (1-13): ", 1, 13);

        try {
            switch (choice) {
                case 1: {
                    library.displayAllMedia();
                    break;
                }
                case 2: {
                    cout << "\n--- DODAWANIE NOWEJ KSIAZKI ---\n";
                    string title = readLine("Tytul: ");
                    int year = readInt("Rok wydania (od 1): ", 1, 2100);
                    string author = readLine("Autor: ");
                    int pages = readInt("Liczba stron (wieksza od 0): ", 1);
                    string isbn = readLine("Numer ISBN: ");

                    int id = library.addBook(title, year, author, pages, isbn);
                    cout << "[SUKCES] Pomyslnie dodano ksiazke z ID: " << id << "\n";
                    break;
                }
                case 3: {
                    cout << "\n--- DODAWANIE NOWEGO FILMU ---\n";
                    string title = readLine("Tytul: ");
                    int year = readInt("Rok wydania (od 1880): ", 1880, 2100);
                    string director = readLine("Rezyser: ");
                    int duration = readInt("Czas trwania w minutach (wiekszy od 0): ", 1);
                    int ageRating = readInt("Ograniczenie wiekowe (od 0 do 120, np. 0, 12, 16, 18): ", 0, 120);
                    double digitalPrice = readDouble("Oplata za wypozyczenie cyfrowe VOD w PLN (np. 14.99): ", 0.01);

                    int id = library.addMovie(title, year, director, duration, ageRating, digitalPrice);
                    cout << "[SUKCES] Pomyslnie dodano film z ID: " << id << "\n";
                    break;
                }
                case 4: {
                    cout << "\n--- EDYCJA TYTULU ZASOBU ---\n";
                    int id = readInt("Podaj ID pozycji do modyfikacji: ", 1);
                    string newTitle = readLine("Podaj nowy tytul: ");
                    library.updateMediaTitle(id, newTitle);
                    cout << "[SUKCES] Zaktualizowano tytul dla pozycji ID " << id << ".\n";
                    break;
                }
                case 5: {
                    cout << "\n--- USUWANIE ZASOBU Z KATALOGU ---\n";
                    int id = readInt("Podaj ID pozycji do usuniecia: ", 1);
                    library.removeMedia(id);
                    cout << "[SUKCES] Pozycja o ID " << id << " zostala usunieta.\n";
                    break;
                }
                case 6: {
                    cout << "\n--- ZARZADZANIE UZYTKOWNIKAMI ---\n";
                    cout << "1. Wyswietl liste uzytkownikow\n";
                    cout << "2. Zarejestruj nowego uzytkownika\n";
                    int subChoice = readInt("Wybierz opcje (1-2): ", 1, 2);
                    if (subChoice == 1) {
                        library.displayAllUsers();
                    } else if (subChoice == 2) {
                        string name = readLine("Imie i nazwisko: ");
                        int age = readInt("Wiek (1-120): ", 1, 120);
                        int uid = library.addUser(name, age);
                        cout << "[SUKCES] Zarejestrowano uzytkownika \"" << name << "\" z ID: " << uid << "\n";
                    }
                    break;
                }
                case 7: {
                    cout << "\n--- WYPOZYCZENIE FIZYCZNE ---\n";
                    int mediaId = readInt("Podaj ID wypozyczanej pozycji: ", 1);
                    int userId = readInt("Podaj ID uzytkownika: ", 1);
                    library.borrowMedia(mediaId, userId);
                    cout << "[SUKCES] Pozycja ID " << mediaId << " zostala pomyslnie wypozyczona uzytkownikowi ID " << userId << ".\n";
                    break;
                }
                case 8: {
                    cout << "\n--- ZWROT POZYCJI FIZYCZNEJ ---\n";
                    int mediaId = readInt("Podaj ID zwracanej pozycji: ", 1);
                    int daysOverdue = readInt("Liczba dni spoznienia (0 jesli zwrot w terminie): ", 0);
                    double fee = library.returnMedia(mediaId, daysOverdue);
                    cout << fixed << setprecision(2);
                    cout << "[SUKCES] Pozycja ID " << mediaId << " zostala zwrocona.\n";
                    if (fee > 0.0) {
                        cout << "[OPLATA] Naliczono kare za przetrzymanie (" << daysOverdue << " dni): " << fee << " PLN.\n";
                    } else {
                        cout << "[OPLATA] Zwrot w terminie - brak oplat karnych (0.00 PLN).\n";
                    }
                    break;
                }
                case 9: {
                    cout << "\n--- WYPOZYCZENIE FILMU ONLINE (CYFROWE VOD NA 48H) ---\n";
                    int movieId = readInt("Podaj ID filmu do wypozyczenia online: ", 1);
                    int userId = readInt("Podaj ID uzytkownika: ", 1);

                    double price = library.rentMovieOnline(movieId, userId);
                    cout << fixed << setprecision(2);
                    cout << "[SUKCES] Film ID " << movieId << " zostal wypozyczony online dla uzytkownika ID " << userId << "!\n";
                    cout << "[VOD] Pobrano oplate dystrybucyjna: " << price << " PLN.\n";
                    cout << "[VOD] Subskrypcja jest aktywna przez 48 godzin. Po tym czasie wygasnie i zostanie zwrocona automatycznie.\n";
                    break;
                }
                case 10: {
                    cout << "\n--- PRZEGLADANIE SUBSKRYPCJI ONLINE UZYTKOWNIKA ---\n";
                    int userId = readInt("Podaj ID uzytkownika: ", 1);
                    library.displayUserDigitalRentals(userId);
                    break;
                }
                case 11: {
                    cout << "\n--- SYMULACJA UPLYWU CZASU (TEST AUTOMATYCZNEGO ZWROTU) ---\n";
                    cout << "Aktualne przesuniecie czasu: +" << library.getSimulatedHoursOffset() << "h\n";
                    int hours = readInt("O ile godzin chcesz przesunac zegar symulacji? (np. 48 lub 49): ", 1);
                    library.advanceSimulatedTime(hours);
                    break;
                }
                case 12: {
                    cout << "\n--- POROWNANIE DWOCH ZASOBOW (operator==) ---\n";
                    int id1 = readInt("Podaj ID pierwszej pozycji: ", 1);
                    int id2 = readInt("Podaj ID drugiej pozycji: ", 1);

                    Media* m1 = library.findMediaById(id1);
                    Media* m2 = library.findMediaById(id2);

                    if (!m1) {
                        throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(id1));
                    }
                    if (!m2) {
                        throw MediaNotFoundException("Nie znaleziono pozycji o ID: " + to_string(id2));
                    }

                    cout << "\nObiekt 1: " << *m1 << "\n";
                    cout << "Obiekt 2: " << *m2 << "\n";

                    if (*m1 == *m2) {
                        cout << "[WYNIK] Obiekty sa rowne (*m1 == *m2 jest TRUE) - posiadaja ten sam identyfikator ID.\n";
                    } else {
                        cout << "[WYNIK] Obiekty sa rozne (*m1 == *m2 jest FALSE) - posiadaja rozne identyfikatory ID.\n";
                    }
                    break;
                }
                case 13: {
                    cout << "\nZamykanie programu. Dziekujemy za skorzystanie z systemu!\n";
                    running = false;
                    break;
                }
                default: {
                    cout << "[OSTRZEZENIE] Niepoprawny wybor! Wprowadz cyfre od 1 do 13.\n";
                    break;
                }
            }
        } catch (const LibraryException& ex) {
            cerr << "\n[WYJATEK BIBLIOTEKI] " << ex.what() << "\n";
        } catch (const exception& ex) {
            cerr << "\n[WYJATEK STANDARDOWY] " << ex.what() << "\n";
        }
    }

    return 0;
}
