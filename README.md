# System Zarządzania Wypożyczalnią Mediów (Biblioteka C++)

Projekt akademicki realizujący pełną funkcjonalność systemu zarządzania biblioteką multimediów (książki, filmy) wraz z obsługą czytelników, operacjami CRUD, polimorfizmem dynamicznym, kontrolą ograniczeń wiekowych, przeciążaniem operatorów oraz własną hierarchią wyjątków.

Projekt został zaprojektowany i zaimplementowany zgodnie z najwyższymi standardami programowania obiektowego w języku C++ (standard C++17), spełniając wszystkie wymagania na **ocenę 5.0**.

---

## 1. Struktura Projektu

Kod został modularnie podzielony na interfejsy nagłówkowe (`include/`) oraz implementacje (`src/`):

```text
media_library_system/
├── include/
│   ├── Exceptions.hpp        # Hierarchia własnych wyjątków (dziedziczenie po std::runtime_error)
│   ├── Media.hpp             # Klasa bazowa reprezentująca ogólny nośnik informacji
│   ├── Book.hpp              # Klasa pochodna reprezentująca książkę
│   ├── Movie.hpp             # Klasa pochodna reprezentująca film
│   ├── User.hpp              # Klasa reprezentująca czytelnika/użytkownika
│   └── LibraryManager.hpp    # Główny kontroler zarządzający katalogiem mediów i czytelnikami
├── src/
│   ├── Media.cpp             # Implementacja metod klasy Media i operatorów
│   ├── Book.cpp              # Implementacja metod klasy Book
│   ├── Movie.cpp             # Implementacja metod klasy Movie
│   ├── User.cpp              # Implementacja metod klasy User i operatora<<
│   ├── LibraryManager.cpp    # Logika biznesowa, kompozycja, dynamic_cast, obsługa błędów
│   └── main.cpp              # Interfejs konsolowy (menu tekstowe, obsługa wejścia, try-catch)
├── docs/
│   ├── UML.puml              # Diagram klas w formacie PlantUML
│   └── UML.md                # Diagram klas Mermaid wraz z opisem relacji
├── bin/                      # Katalog na skompilowany plik wykonywalny (.exe)
├── Makefile                  # Plik ułatwiający automatyczną kompilację
├── CMakeLists.txt            # Konfiguracja systemu CMake
├── .gitignore                # Wykluczenia plików tymczasowych i binarnych
└── README.md                 # Dokumentacja projektu i instrukcja obsługi
```

---

## 2. Realizacja Wymogów Projektowych (Kryteria na ocenę 5.0)

| Wymóg / Kryterium | Sposób realizacji w projekcie | Gdzie sprawdzić w kodzie |
| :--- | :--- | :--- |
| **Podział na pliki i moduły** | Czyste rozdzielenie definicji klas (`.hpp`) od definicji metod (`.cpp`). | `include/`, `src/` |
| **Hermetyzacja (enkapsulacja)** | Wszystkie pola są `private` lub `protected`; dostęp przez selektory i modyfikatory (`const` gettery/settery). | Wszystkie klasy |
| **Hierarchia klas i dziedziczenie** | Klasa abstrakcyjna `Media` oraz klasy pochodne `Book` i `Movie` dziedziczące publicznie. | `Media.hpp`, `Book.hpp`, `Movie.hpp` |
| **Wirtualny destruktor** | `virtual ~Media() = default;` w klasie bazowej zapewnia bezpieczne usuwanie obiektów polimorficznych. | `Media.hpp` |
| **Polimorfizm dynamiczny** | Metody `printDetails()`, `calculateLateFee()`, `getType()` zadeklarowane jako wirtualne i nadpisane z `override`. | `Book.cpp`, `Movie.cpp` |
| **Zarządzanie pamięcią i sprytne wskaźniki** | Użycie `std::vector<std::unique_ptr<Media>>` oraz `std::make_unique` zapobiega wszelkim wyciekom pamięci bez konieczności ręcznego `delete`. | `LibraryManager.hpp`, `LibraryManager.cpp` |
| **Rzutowanie dynamiczne (`dynamic_cast`)** | W metodzie `borrowMedia()` następuje bezpieczne rzutowanie `dynamic_cast<const Movie*>(media)` w celu sprawdzenia ograniczenia wiekowego (`ageRating` vs `user.getAge()`). | `src/LibraryManager.cpp` (metoda `borrowMedia`) |
| **Przeciążanie operatorów** | `operator==` porównujący media po `id`; polimorficzny `operator<<` współpracujący z `std::ostream`. | `Media.hpp`, `Media.cpp`, `User.hpp`, `User.cpp` |
| **Własna hierarchia wyjątków** | Klasy `LibraryException`, `MediaNotFoundException`, `MediaUnavailableException`, `AgeRestrictionException`, `UserNotFoundException` dziedziczące po `std::runtime_error`. | `include/Exceptions.hpp` |
| **Odporny interfejs użytkownika** | Pętla menu w `main.cpp` z walidacją wejścia (`cin.fail()`, czyszczenie bufora) oraz blokiem `try ... catch`. | `src/main.cpp` |
| **Historia prac w Git** | 8 logicznych, atomowych commitów dokumentujących kolejne fazy powstawania aplikacji. | `git log --oneline` |

---

## 3. Kompilacja i Uruchomienie

### Opcja 1: Bezpośrednio za pomocą kompilatora g++ (zalecane)
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Media.cpp src/User.cpp src/Book.cpp src/Movie.cpp src/LibraryManager.cpp src/main.cpp -o bin/library_app.exe
```
Uruchomienie:
```powershell
.\bin\library_app.exe
```

### Opcja 2: Za pomocą Makefile (jeśli zainstalowany jest program `make`)
```powershell
make
.\bin\library_app.exe
```

### Opcja 3: Za pomocą CMake
```powershell
mkdir build
cd build
cmake ..
cmake --build .
```

---

## 4. Przebieg Prezentacji / Scenariusz Obrony Projektu

Program przy starcie uruchamia metodę `seedData()`, która wprowadza przykładowe dane:
- **Książki**:
  - ID 1: *Wiedźmin: Ostatnie życzenie* (Andrzej Sapkowski)
  - ID 2: *Solaris* (Stanisław Lem)
  - ID 3: *Czysty Kod* (Robert C. Martin)
- **Filmy**:
  - ID 4: *Incepcja* (od lat 12)
  - ID 5: *Ojciec Chrzestny* (od lat 16)
  - ID 6: *Pulp Fiction* (od lat 18)
- **Czytelnicy**:
  - ID 1: *Jan Kowalski* (25 lat)
  - ID 2: *Anna Nowak* (15 lat)
  - ID 3: *Piotr Wiśniewski* (10 lat)

### Rekomendowane kroki demonstracyjne dla prowadzącego:
1. **Opcja 1 (Wypisanie katalogu)**: Demonstracja polimorficznego działania `operator<<` i metody `printDetails()` (książki wyświetlają autora i ISBN, filmy reżysera i ograniczenie wiekowe).
2. **Opcja 7 (Próba wypożyczenia filmu 18+ przez 15-latka)**:
   - Wybierz pozycję ID 6 (*Pulp Fiction*, od 18 lat) i użytkownika ID 2 (*Anna Nowak*, 15 lat).
   - **Efekt**: Rzucenie i obsłużenie wyjątku `AgeRestrictionException` dzięki `dynamic_cast`.
3. **Opcja 7 (Prawidłowe wypożyczenie)**:
   - Wypożycz pozycję ID 6 użytkownikowi ID 1 (*Jan Kowalski*, 25 lat).
   - **Efekt**: Sukces. Status pozycji zmienia się na wypożyczony.
4. **Opcja 7 (Próba ponownego wypożyczenia)**:
   - Ponowna próba wypożyczenia pozycji ID 6.
   - **Efekt**: Złapanie wyjątku `MediaUnavailableException`.
5. **Opcja 8 (Zwrot pozycji z karą)**:
   - Zwróć pozycję ID 6 ze spóźnieniem np. 5 dni.
   - **Efekt**: Wyliczenie kary przez polimorficzną metodę `calculateLateFee()` (stawka 2.00 zł/dzień = 10.00 PLN).
6. **Opcja 9 (Porównanie operator==)**:
   - Wprowadź ID 1 i ID 2 (wynik: FALSE).
   - Wprowadź ID 1 i ID 1 (wynik: TRUE).
7. **Opcja 5 (Usunięcie pozycji i próba operacji)**:
   - Usuń pozycję ID 2, a następnie spróbuj ją wypożyczyć.
   - **Efekt**: Wyjątek `MediaNotFoundException`.

---

## 5. Historia Commitów Git

Projekt zawiera historię 8 kroków:
1. `inicjalizacja struktury projektu (include, src, docs)`
2. `implementacja klasy bazowej Media oraz klasy User`
3. `dodanie klas pochodnych Book i Movie z metodami wirtualnymi`
4. `implementacja LibraryManager z operacjami CRUD`
5. `przeciazenie operatorow << oraz == dla klasy Media`
6. `dodanie wlasnych wyjatkow i walidacji wieku przy wypozyczaniu`
7. `budowa interfejsu konsolowego w main.cpp i danych testowych`
8. `docs: dodanie schematu UML oraz instrukcji kompilacji w README.md`
