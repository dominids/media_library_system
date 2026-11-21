# System Zarządzania Wypożyczalnią Mediów (Biblioteka C++)

Projekt akademicki realizujący pełną funkcjonalność systemu zarządzania biblioteką multimediów (książki, filmy tradycyjne oraz **dystrybucja cyfrowa filmów online VOD na 48h**) wraz z obsługą czytelników, operacjami CRUD, polimorfizmem dynamicznym, kontrolą ograniczeń wiekowych, przeciążaniem operatorów oraz własną hierarchią wyjątków.

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
│   ├── Movie.hpp             # Klasa pochodna reprezentująca film (wraz z opłatą VOD)
│   ├── DigitalRental.hpp     # Klasa reprezentująca 48-godzinną subskrypcję cyfrową filmu
│   ├── User.hpp              # Klasa czytelnika z kontenerem subskrypcji online
│   └── LibraryManager.hpp    # Główny kontroler zarządzający katalogiem, czytelnikami i VOD
├── src/
│   ├── Media.cpp             # Implementacja metod klasy Media i operatorów
│   ├── Book.cpp              # Implementacja metod klasy Book
│   ├── Movie.cpp             # Implementacja metod klasy Movie i opłaty cyfrowej
│   ├── DigitalRental.cpp     # Logika czasu wygaśnięcia, obliczania pozostałych godzin
│   ├── User.cpp              # Zarządzanie subskrypcjami użytkownika, auto-usuwanie
│   ├── LibraryManager.cpp    # Logika biznesowa, kompozycja, dynamic_cast, symulacja czasu
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
| **Rzutowanie dynamiczne (`dynamic_cast`)** | W metodach `borrowMedia()` oraz `rentMovieOnline()` następuje bezpieczne rzutowanie `dynamic_cast<const Movie*>` w celu sprawdzenia ograniczenia wiekowego oraz typu nośnika. | `src/LibraryManager.cpp` |
| **Dystrybucja cyfrowa filmów (VOD 48h)** | Każdy film posiada stawkę za dystrybucję cyfrową (`digitalPrice`). Użytkownik może wypożyczyć film online na 48 godzin. Po tym czasie subskrypcja wygasa i jest automatycznie usuwana z konta użytkownika. | `DigitalRental.hpp`, `User.hpp`, `LibraryManager.cpp` |
| **Przeciążanie operatorów** | `operator==` porównujący media po `id`; polimorficzny `operator<<` współpracujący z `std::ostream`. | `Media.hpp`, `Media.cpp`, `User.hpp`, `User.cpp` |
| **Własna hierarchia wyjątków** | Klasy `LibraryException`, `MediaNotFoundException`, `MediaUnavailableException`, `AgeRestrictionException`, `UserNotFoundException`, `InvalidDataException` dziedziczące po `std::runtime_error`. | `include/Exceptions.hpp` |
| **Odporny interfejs użytkownika i walidacja** | Pętla menu w `main.cpp` z rygorystyczną walidacją wejścia (`readInt`, `readDouble`, `readLine` z `trim()` i `removeBom()`). | `src/main.cpp` |
| **Historia prac w Git** | Logiczne commity podpisane autorem dokumentujące fazy powstawania aplikacji. | `git log --oneline` |

---

## 3. Kompilacja i Uruchomienie

### Opcja 1: Bezpośrednio za pomocą kompilatora g++ (zalecane)
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/Media.cpp src/User.cpp src/Book.cpp src/Movie.cpp src/LibraryManager.cpp src/DigitalRental.cpp src/main.cpp -o bin/library_app.exe
```
Uruchomienie:
```powershell
.\bin\library_app.exe
```

### Opcja 2: Za pomocą Makefile
```powershell
make
.\bin\library_app.exe
```

---

## 4. Scenariusz Demonstracyjny dla Prowadzącego (VOD 48h)

1. **Wypożyczenie filmu online (Opcja 9)**:
   - Wybierz film ID 6 (*Pulp Fiction*, wiek 18+, cena 17.99 PLN).
   - Próba wypożyczenia przez użytkownika ID 2 (*Anna Nowak*, 15 lat) -> Wyjątek `AgeRestrictionException`.
   - Wypożyczenie przez użytkownika ID 1 (*Jan Kowalski*, 25 lat) -> Pomyślne pobranie opłaty 17.99 PLN i utworzenie subskrypcji na 48h.
2. **Podgląd aktywnych subskrypcji (Opcja 10)**:
   - Wyświetl subskrypcje użytkownika ID 1: widoczny film, opłata oraz dokładny czas wygaśnięcia i licznik pozostałych godzin (48h).
3. **Symulacja automatycznego zwrotu (Opcja 11)**:
   - Przesuń czas symulacji o 49 godzin.
   - Program automatycznie wykrywa wygaśnięcie subskrypcji, dokonuje automatycznego zwrotu i usuwa film z konta użytkownika.
   - Ponowne sprawdzenie opcją 10 potwierdza, że subskrypcja została pomyślnie i automatycznie usunięta!
