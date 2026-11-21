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

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "[BLAD] Niepoprawna wartosc! Wprowadz liczbe calkowita.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    while (input.empty()) {
        getline(cin, input);
    }
    return input;
}

void printMenu() {
    cout << "\n========================================\n";
    cout << "  SYSTEM ZARZADZANIA WYPOZYCZALNIA MEDIOW\n";
    cout << "========================================\n";
    cout << " 1. Wyswietl caly katalog mediow\n";
    cout << " 2. Dodaj nowa ksiazke\n";
    cout << " 3. Dodaj nowy film\n";
    cout << " 4. Edytuj tytul zasobu\n";
    cout << " 5. Usun zasob z katalogu\n";
    cout << " 6. Zarzadzaj uzytkownikami (Rejestracja / Lista)\n";
    cout << " 7. Wypozycz pozycje uzytkownikowi\n";
    cout << " 8. Zwroc pozycje (kalkulacja kary)\n";
    cout << " 9. Porownaj dwa zasoby (demonstracja operator==)\n";
    cout << " 10. Wyjscie z programu\n";
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
        int choice = readInt("Wybierz opcje (1-10): ");

        try {
            switch (choice) {
                case 1: {
                    library.displayAllMedia();
                    break;
                }
                case 2: {
                    cout << "\n--- DODAWANIE NOWEJ KSIAZKI ---\n";
                    string title = readLine("Tytul: ");
                    int year = readInt("Rok wydania: ");
                    string author = readLine("Autor: ");
                    int pages = readInt("Liczba stron: ");
                    string isbn = readLine("Numer ISBN: ");

                    int id = library.addBook(title, year, author, pages, isbn);
                    cout << "[SUKCES] Pomyslnie dodano ksiazke z ID: " << id << "\n";
                    break;
                }
                case 3: {
                    cout << "\n--- DODAWANIE NOWEGO FILMU ---\n";
                    string title = readLine("Tytul: ");
                    int year = readInt("Rok wydania: ");
                    string director = readLine("Rezyser: ");
                    int duration = readInt("Czas trwania (w minutach): ");
                    int ageRating = readInt("Ograniczenie wiekowe (np. 12, 16, 18): ");

                    int id = library.addMovie(title, year, director, duration, ageRating);
                    cout << "[SUKCES] Pomyslnie dodano film z ID: " << id << "\n";
                    break;
                }
                case 4: {
                    cout << "\n--- EDYCJA TYTULU ZASOBU ---\n";
                    int id = readInt("Podaj ID pozycji do modyfikacji: ");
                    string newTitle = readLine("Podaj nowy tytul: ");
                    library.updateMediaTitle(id, newTitle);
                    cout << "[SUKCES] Zaktualizowano tytul dla pozycji ID " << id << ".\n";
                    break;
                }
                case 5: {
                    cout << "\n--- USUWANIE ZASOBU Z KATALOGU ---\n";
                    int id = readInt("Podaj ID pozycji do usuniecia: ");
                    library.removeMedia(id);
                    cout << "[SUKCES] Pozycja o ID " << id << " zostala usunieta.\n";
                    break;
                }
                case 6: {
                    cout << "\n--- ZARZADZANIE UZYTKOWNIKAMI ---\n";
                    cout << "1. Wyswietl liste uzytkownikow\n";
                    cout << "2. Zarejestruj nowego uzytkownika\n";
                    int subChoice = readInt("Wybierz opcje (1-2): ");
                    if (subChoice == 1) {
                        library.displayAllUsers();
                    } else if (subChoice == 2) {
                        string name = readLine("Imie i nazwisko: ");
                        int age = readInt("Wiek: ");
                        int uid = library.addUser(name, age);
                        cout << "[SUKCES] Zarejestrowano uzytkownika \"" << name << "\" z ID: " << uid << "\n";
                    } else {
                        cout << "[OSTRZEZENIE] Nieznana opcja.\n";
                    }
                    break;
                }
                case 7: {
                    cout << "\n--- WYPOZYCZENIE POZYCJI ---\n";
                    int mediaId = readInt("Podaj ID wypozyczanej pozycji: ");
                    int userId = readInt("Podaj ID uzytkownika: ");
                    library.borrowMedia(mediaId, userId);
                    cout << "[SUKCES] Pozycja ID " << mediaId << " zostala pomyslnie wypozyczona uzytkownikowi ID " << userId << ".\n";
                    break;
                }
                case 8: {
                    cout << "\n--- ZWROT POZYCJI ---\n";
                    int mediaId = readInt("Podaj ID zwracanej pozycji: ");
                    int daysOverdue = readInt("Liczba dni spoznienia (0 jesli zwrot w terminie): ");
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
                    cout << "\n--- POROWNANIE DWOCH ZASOBOW (operator==) ---\n";
                    int id1 = readInt("Podaj ID pierwszej pozycji: ");
                    int id2 = readInt("Podaj ID drugiej pozycji: ");

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
                case 10: {
                    cout << "\nZamykanie programu. Dziekujemy za skorzystanie z systemu!\n";
                    running = false;
                    break;
                }
                default: {
                    cout << "[OSTRZEZENIE] Niepoprawny wybor! Wprowadz cyfre od 1 do 10.\n";
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
