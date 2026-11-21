# System Zarządzania Wypożyczalnią Mediów (Biblioteka C++)

Projekt akademicki realizujący pełną funkcjonalność systemu zarządzania biblioteką multimediów (książki, filmy tradycyjne oraz **dystrybucja cyfrowa filmów online VOD na 48h**) wraz z obsługą czytelników, operacjami CRUD, polimorfizmem dynamicznym, kontrolą ograniczeń wiekowych, przeciążaniem operatorów, własną hierarchią wyjątków oraz **dwoma niezależnymi interfejsami (Panel Administratora i Panel Użytkownika) działającymi równocześnie w osobnych terminalach na współdzielonym stanie**.

Projekt został zaprojektowany i zaimplementowany zgodnie z najwyższymi standardami programowania obiektowego w języku C++ (standard C++17), spełniając wszystkie wymagania na **ocenę 5.0**.

---

## 1. Struktura Projektu

Kod został modularnie podzielony na interfejsy nagłówkowe (`include/`), implementacje (`src/`) oraz skrypty uruchomieniowe:

```text
media_library_system/
├── include/
│   ├── Exceptions.hpp        # Hierarchia własnych wyjątków (dziedziczenie po std::runtime_error)
│   ├── Media.hpp             # Klasa bazowa reprezentująca ogólny nośnik informacji
│   ├── Book.hpp              # Klasa pochodna reprezentująca książkę
│   ├── Movie.hpp             # Klasa pochodna reprezentująca film (wraz z opłatą VOD)
│   ├── DigitalRental.hpp     # Klasa reprezentująca 48-godzinną subskrypcję cyfrową filmu
│   ├── User.hpp              # Klasa użytkownika z kontenerem subskrypcji, loginem i hashem hasła
│   ├── LibrarySerializer.hpp # Moduł serializacji i deserializacji stanu bazy danych (plik tekstowy)
│   └── LibraryManager.hpp    # Główny kontroler zarządzający katalogiem, czytelnikami i VOD
├── src/
│   ├── Media.cpp             # Implementacja metod klasy Media, terminów zwrotu i operatorów
│   ├── Book.cpp              # Implementacja metod klasy Book
│   ├── Movie.cpp             # Implementacja metod klasy Movie i opłaty cyfrowej
│   ├── DigitalRental.cpp     # Logika czasu wygaśnięcia, obliczania pozostałych godzin
│   ├── User.cpp              # Zarządzanie subskrypcjami użytkownika, hashowanie hasła (djb2)
│   ├── LibrarySerializer.cpp # Zapis i odczyt stanu katalogu, użytkowników i wypożyczeń z pliku
│   ├── LibraryManager.cpp    # Logika biznesowa, kompozycja, dynamic_cast, przedłużanie, symulacja czasu
│   ├── main.cpp              # Panel Administratora (zarządzanie zasobami, użytkownikami, zwrotami)
│   └── user_main.cpp         # Panel Użytkownika (logowanie, rejestracja, VOD, przedłużanie wypożyczeń)
├── docs/
│   ├── UML.puml              # Diagram klas w formacie PlantUML
│   └── UML.md                # Diagram klas Mermaid wraz z opisem relacji
├── bin/                      # Katalog na skompilowane pliki wykonywalne (.exe)
├── Makefile                  # Plik ułatwiający automatyczną kompilację obu aplikacji
├── CMakeLists.txt            # Konfiguracja systemu CMake dla dwóch celów (library_app, user_app)
├── start_both.bat            # Skrypt uruchamiający oba panele jednocześnie w osobnych terminalach (CMD)
├── start_both.ps1            # Skrypt PowerShell uruchamiający oba panele w osobnych oknach
├── .gitignore                # Wykluczenia plików tymczasowych, binarnych i bazy runtime
└── README.md                 # Dokumentacja projektu i instrukcja obsługi
```

---

## 2. Realizacja Wymogów Projektowych (Kryteria na ocenę 5.0)

| Wymóg / Kryterium | Sposób realizacji w projekcie | Gdzie sprawdzić w kodzie |
| :--- | :--- | :--- |
| **Dwa niezależne interfejsy (Admin + Użytkownik)** | Dwa osobne pliki wykonywalne: `library_app.exe` (zarządzanie systemem) oraz `user_app.exe` (panel klienta). Uruchamiane jednocześnie w dwóch terminalach. | `src/main.cpp`, `src/user_main.cpp`, `start_both.bat` |
| **Współdzielony stan (Persistence)** | Moduł `LibrarySerializer` synchronizuje stan katalogu, użytkowników i subskrypcji w pliku `data/library.dat` w czasie rzeczywistym. | `include/LibrarySerializer.hpp`, `src/LibrarySerializer.cpp` |
| **Uwierzytelnianie i haszowanie haseł** | Rejestracja i logowanie użytkowników z hashowaniem haseł algorytmem djb2. Hasło jawne nigdy nie jest przechowywane w stanie systemu. | `User.hpp`, `User.cpp`, `src/user_main.cpp` |
| **Bezpłatne przedłużanie wypożyczeń (+7 dni)** | Użytkownik z poziomu panelu klienta może przedłużyć termin zwrotu wypożyczonej książki/filmu o 1 tydzień. | `LibraryManager::extendLoan`, `Media::extendDueDate` |
| **Dystrybucja cyfrowa filmów (VOD 48h)** | Filmy dostępne online na 48 godzin ze stawką `digitalPrice`. Po upływie 48h subskrypcja wygasa i jest automatycznie usuwana. | `DigitalRental.hpp`, `User.hpp`, `LibraryManager.cpp` |
| **Automatyczna kalkulacja kar i model czasu** | Kara za spóźnienie przy zwrocie zasobu fizycznego jest wyliczana automatycznie z zegara systemowego / symulowanego (`calculateOverdueDays`). | `Media.hpp`, `LibraryManager.cpp` |
| **Hierarchia klas i dziedziczenie** | Klasa abstrakcyjna `Media` oraz klasy pochodne `Book` i `Movie` dziedziczące publicznie. | `Media.hpp`, `Book.hpp`, `Movie.hpp` |
| **Wirtualny destruktor** | `virtual ~Media() = default;` w klasie bazowej zapewnia bezpieczne usuwanie obiektów polimorficznych. | `Media.hpp` |
| **Polimorfizm dynamiczny** | Metody `printDetails()`, `calculateLateFee()`, `getType()` zadeklarowane jako wirtualne i nadpisane z `override`. | `Book.cpp`, `Movie.cpp` |
| **Zarządzanie pamięcią i sprytne wskaźniki** | Użycie `vector<unique_ptr<Media>>` oraz `make_unique` zapobiega wyciekom pamięci. | `LibraryManager.hpp`, `LibraryManager.cpp` |
| **Rzutowanie dynamiczne (`dynamic_cast`)** | W metodach `borrowMedia()`, `rentMovieOnline()` oraz w serializerze następuje bezpieczne sprawdzanie typów. | `src/LibraryManager.cpp`, `src/LibrarySerializer.cpp` |
| **Przeciążanie operatorów** | `operator==` porównujący media po `id`; polimorficzny `operator<<` współpracujący z `ostream`. | `Media.hpp`, `Media.cpp`, `User.hpp`, `User.cpp` |
| **Własna hierarchia wyjątków** | Klasy `LibraryException`, `MediaNotFoundException`, `MediaUnavailableException`, `AgeRestrictionException`, `UserNotFoundException`, `InvalidDataException` dziedziczące po `runtime_error`. | `include/Exceptions.hpp` |
| **Odporny interfejs użytkownika i walidacja** | Rygorystyczna walidacja danych wejściowych (`readInt`, `readDouble`, `readLine` z `removeBom()` i `trim()`). | `src/main.cpp`, `src/user_main.cpp` |

---

## 3. Instrukcja Kompilacji i Uruchomienia

### Wymagania wstępne
- Kompilator wspierający standard C++17 (np. GCC 15/14/13 z MSYS2 UCRT64 lub MinGW-w64, Clang lub MSVC).
- Opcjonalnie program `make` lub `cmake`.

### Kompilacja za pomocą Makefile
W głównym katalogu projektu:
```bash
make
```
Spowoduje to skompilowanie obu programów:
- `bin/library_app.exe` — Panel Administratora
- `bin/user_app.exe` — Panel Użytkownika

### Kompilacja ręczna g++
```bash
# Panel Administratora
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Media.cpp src/User.cpp src/Book.cpp src/Movie.cpp src/LibraryManager.cpp src/DigitalRental.cpp src/LibrarySerializer.cpp src/main.cpp -o bin/library_app.exe

# Panel Użytkownika
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Media.cpp src/User.cpp src/Book.cpp src/Movie.cpp src/LibraryManager.cpp src/DigitalRental.cpp src/LibrarySerializer.cpp src/user_main.cpp -o bin/user_app.exe
```

### Jednoczesne uruchomienie obu paneli w osobnych terminalach
Można skorzystać z przygotowanego skryptu:
- **Wiersz poleceń (CMD)**:
  ```cmd
  start_both.bat
  ```
- **PowerShell**:
  ```powershell
  .\start_both.ps1
  ```
Skrypt automatycznie otwiera dwa niezależne okna terminala: w jednym działa Panel Administratora, a w drugim Panel Użytkownika. Wszystkie zmiany dokonane w jednym oknie (np. wypożyczenie filmu, przedłużenie terminu, rejestracja klienta) są natychmiast widoczne w drugim oknie dzięki współdzielonemu plikowi stanu `data/library.dat`.

---

## 4. Domyślne Dane Demonstracyjne (Konta startowe)

Przy pierwszym uruchomieniu biblioteka jest automatycznie inicjalizowana danymi demonstracyjnymi:

### Użytkownicy:
1. **Jan Kowalski** (wiek: 25 lat, ID: 1)
   - Login: `jan`
   - Hasło: `haslo123`
2. **Anna Nowak** (wiek: 15 lat, ID: 2)
   - Login: `anna`
   - Hasło: `haslo123`
3. **Piotr Wiśniewski** (wiek: 10 lat, ID: 3)
   - Login: `piotr`
   - Hasło: `haslo123`

### Katalog:
- Książki: *Wiedźmin: Ostatnie życzenie*, *Solaris*, *Czysty Kod*
- Filmy: *Incepcja* (12+, 14.99 PLN VOD), *Ojciec Chrzestny* (16+, 12.99 PLN VOD), *Pulp Fiction* (18+, 17.99 PLN VOD)
