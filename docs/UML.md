# Diagram Klas UML i Relacje w Projekcie

Poniższy diagram przedstawia architekturę obiektową Systemu Zarządzania Wypożyczalnią Mediów.

```mermaid
classDiagram
    direction TB

    class Media {
        <<abstract>>
        #int id
        #string title
        #int releaseYear
        #bool isAvailable
        #int borrowedByUserId
        +Media()
        +Media(int id, string title, int year)
        +virtual ~Media() = default
        +int getId() const
        +string getTitle() const
        +int getReleaseYear() const
        +bool getIsAvailable() const
        +int getBorrowedByUserId() const
        +void setTitle(string newTitle)
        +void setReleaseYear(int year)
        +void setAvailable(bool status)
        +void setBorrowedByUserId(int userId)
        +virtual void printDetails(ostream& os) const
        +virtual double calculateLateFee(int daysOverdue) const = 0
        +virtual string getType() const = 0
        +bool operator==(const Media& other) const
        +friend ostream& operator<<(ostream& os, const Media& media)
    }

    class Book {
        -string author
        -int pageCount
        -string isbn
        +Book(int id, string title, int year, string author, int pageCount, string isbn)
        +string getAuthor() const
        +int getPageCount() const
        +string getIsbn() const
        +void setAuthor(string newAuthor)
        +void setPageCount(int pages)
        +void setIsbn(string newIsbn)
        +void printDetails(ostream& os) const override
        +double calculateLateFee(int daysOverdue) const override
        +string getType() const override
    }

    class Movie {
        -string director
        -int durationMinutes
        -int ageRating
        +Movie(int id, string title, int year, string director, int durationMinutes, int ageRating)
        +string getDirector() const
        +int getDurationMinutes() const
        +int getAgeRating() const
        +void setDirector(string newDirector)
        +void setDurationMinutes(int duration)
        +void setAgeRating(int rating)
        +void printDetails(ostream& os) const override
        +double calculateLateFee(int daysOverdue) const override
        +string getType() const override
    }

    class User {
        -int userId
        -string fullName
        -int age
        +User()
        +User(int userId, string fullName, int age)
        +int getUserId() const
        +string getFullName() const
        +int getAge() const
        +void setFullName(string name)
        +void setAge(int newAge)
        +friend ostream& operator<<(ostream& os, const User& user)
    }

    class LibraryException {
        +LibraryException(string message)
    }
    class MediaNotFoundException {
        +MediaNotFoundException(string message)
    }
    class MediaUnavailableException {
        +MediaUnavailableException(string message)
    }
    class AgeRestrictionException {
        +AgeRestrictionException(string message)
    }
    class UserNotFoundException {
        +UserNotFoundException(string message)
    }

    class LibraryManager {
        -vector~unique_ptr~Media~~ catalog
        -vector~User~ users
        -int nextMediaId
        -int nextUserId
        +LibraryManager()
        +int addBook(string title, int year, string author, int pageCount, string isbn)
        +int addMovie(string title, int year, string director, int durationMinutes, int ageRating)
        +int addUser(string fullName, int age)
        +void removeMedia(int id)
        +Media* findMediaById(int id)
        +const Media* findMediaById(int id) const
        +User* findUserById(int userId)
        +const User* findUserById(int userId) const
        +void updateMediaTitle(int id, string newTitle)
        +void displayAllMedia() const
        +void displayAllUsers() const
        +void borrowMedia(int mediaId, int userId)
        +double returnMedia(int mediaId, int daysOverdue)
        +void seedData()
        +const vector~unique_ptr~Media~~& getCatalog() const
        +const vector~User~& getUsers() const
    }

    Media <|-- Book : dziedziczenie
    Media <|-- Movie : dziedziczenie
    LibraryException <|-- MediaNotFoundException : dziedziczenie
    LibraryException <|-- MediaUnavailableException : dziedziczenie
    LibraryException <|-- AgeRestrictionException : dziedziczenie
    LibraryException <|-- UserNotFoundException : dziedziczenie

    LibraryManager *-- Media : kompozycja (std::unique_ptr)
    LibraryManager o-- User : agregacja / kolekcja uzytkownikow
    LibraryManager ..> LibraryException : rzuca wyjatki
    LibraryManager ..> Movie : dynamic_cast (walidacja ograniczenia wiekowego)
```

---

## Opis relacji w diagramie UML (dla prowadzącego)

1. **Dziedziczenie (Generalizacja)**:
   - Klasy `Book` oraz `Movie` dziedziczą publicznie po klasie abstrakcyjnej `Media`.
   - Klasy wyjątków `MediaNotFoundException`, `MediaUnavailableException`, `AgeRestrictionException`, `UserNotFoundException` dziedziczą po `LibraryException` (która dziedziczy po `std::runtime_error`).
2. **Kompozycja**:
   - `LibraryManager` posiada kolekcję `catalog` typu `std::vector<std::unique_ptr<Media>>`. Obiekty mediów są tworzone przez menedżera za pomocą `std::make_unique` i ich cykl życia jest ściśle związany z instancją menedżera. Usunięcie menedżera lub elementu z wektora automatycznie i bezpiecznie zwalnia pamięć (brak wycieków).
3. **Agregacja**:
   - `LibraryManager` przechowuje listę zarejestrowanych czytelników w kontenerze `std::vector<User>`.
4. **Zależność (Dependency) i Rzutowanie Dynamiczne (`dynamic_cast`)**:
   - `LibraryManager` w metodzie `borrowMedia` weryfikuje czy wypożyczany obiekt jest typu `Movie`. Jeśli tak, rzutuje wskaźnik polimorficzny `Media*` na `const Movie*` i porównuje wiek użytkownika z `ageRating`. W razie naruszenia reguły rzuca `AgeRestrictionException`.
