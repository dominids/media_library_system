# Diagram Klas UML i Relacje w Projekcie

Poniższy diagram przedstawia architekturę obiektową Systemu Zarządzania Wypożyczalnią Mediów wzbogaconą o moduł **dystrybucji cyfrowej filmów online (VOD 48h)**.

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
        #time_point borrowTime
        #time_point dueTime
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
        +void setBorrowInfo(int userId, time_point currentTime, int loanDays)
        +void resetBorrowInfo()
        +time_point getBorrowTime() const
        +time_point getDueTime() const
        +int calculateOverdueDays(time_point currentTime) const
        +string getFormattedDueDate() const
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
        -double digitalPrice
        +Movie(int id, string title, int year, string director, int durationMinutes, int ageRating, double digitalPrice)
        +string getDirector() const
        +int getDurationMinutes() const
        +int getAgeRating() const
        +double getDigitalPrice() const
        +void setDirector(string newDirector)
        +void setDurationMinutes(int duration)
        +void setAgeRating(int rating)
        +void setDigitalPrice(double price)
        +void printDetails(ostream& os) const override
        +double calculateLateFee(int daysOverdue) const override
        +string getType() const override
    }

    class DigitalRental {
        -int movieId
        -string movieTitle
        -time_point rentalTime
        -time_point expirationTime
        -double feePaid
        +DigitalRental()
        +DigitalRental(int movieId, string title, double fee, int durationHours, time_point startTime)
        +int getMovieId() const
        +string getMovieTitle() const
        +time_point getRentalTime() const
        +time_point getExpirationTime() const
        +double getFeePaid() const
        +bool isExpired(time_point currentTime) const
        +int getRemainingHours(time_point currentTime) const
        +string getFormattedExpiration() const
    }

    class User {
        -int userId
        -string fullName
        -int age
        -vector~DigitalRental~ digitalRentals
        +User()
        +User(int userId, string fullName, int age)
        +int getUserId() const
        +string getFullName() const
        +int getAge() const
        +void setFullName(string name)
        +void setAge(int newAge)
        +void addDigitalRental(DigitalRental rental)
        +const vector~DigitalRental~& getDigitalRentals() const
        +bool hasActiveDigitalRental(int movieId, time_point currentTime) const
        +int cleanupExpiredRentals(time_point currentTime)
        +void displayDigitalRentals(ostream& os, time_point currentTime) const
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
    class InvalidDataException {
        +InvalidDataException(string message)
    }

    class LibraryManager {
        -vector~unique_ptr~Media~~ catalog
        -vector~User~ users
        -int nextMediaId
        -int nextUserId
        -int simulatedHoursOffset
        +LibraryManager()
        +int addBook(string title, int year, string author, int pageCount, string isbn)
        +int addMovie(string title, int year, string director, int durationMinutes, int ageRating, double digitalPrice)
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
        +ReturnInfo returnMedia(int mediaId)
        +double rentMovieOnline(int movieId, int userId)
        +int cleanupAllExpiredDigitalRentals()
        +void displayUserDigitalRentals(int userId) const
        +void advanceSimulatedTime(int hours)
        +int getSimulatedHoursOffset() const
        +time_point getCurrentTime() const
        +seedData() void
        +const vector~unique_ptr~Media~~& getCatalog() const
        +const vector~User~& getUsers() const
    }

    Media <|-- Book : dziedziczenie
    Media <|-- Movie : dziedziczenie
    LibraryException <|-- MediaNotFoundException : dziedziczenie
    LibraryException <|-- MediaUnavailableException : dziedziczenie
    LibraryException <|-- AgeRestrictionException : dziedziczenie
    LibraryException <|-- UserNotFoundException : dziedziczenie
    LibraryException <|-- InvalidDataException : dziedziczenie

    LibraryManager *-- Media : kompozycja (std::unique_ptr)
    LibraryManager o-- User : agregacja / kolekcja uzytkownikow
    User *-- DigitalRental : kompozycja subskrypcji cyfrowych
    LibraryManager ..> LibraryException : rzuca wyjatki
    LibraryManager ..> Movie : dynamic_cast (walidacja wieku i VOD)
    LibraryManager ..> DigitalRental : tworzy subskrypcje
```

---

## Opis relacji w diagramie UML (dla prowadzącego)

1. **Dziedziczenie (Generalizacja)**:
   - Klasy `Book` oraz `Movie` dziedziczą publicznie po klasie abstrakcyjnej `Media`.
   - Klasy wyjątków dziedziczą po `LibraryException` (`std::runtime_error`).
2. **Kompozycja**:
   - `LibraryManager` zarządza cyklem życia zasobów w `catalog` typu `std::vector<std::unique_ptr<Media>>`.
   - `User` posiada kolekcję `digitalRentals` typu `std::vector<DigitalRental>`. Subskrypcja online (48h) jest trwale powiązana z użytkownikiem i automatycznie usuwana po wygaśnięciu.
3. **Agregacja**:
   - `LibraryManager` gromadzi obiekty `User`.
4. **Dystrybucja cyfrowa filmów (VOD 48h)**:
   - Każdy obiekt `Movie` posiada atrybut `digitalPrice` (stawka za 48-godzinny dostęp cyfrowy).
   - Metoda `rentMovieOnline()` weryfikuje wiek użytkownika (`dynamic_cast<Movie*>`), pobiera opłatę i rejestruje subskrypcję.
   - Po upływie 48 godzin subskrypcja wygasa i jest automatycznie usuwana z konta czytelnika (`cleanupExpiredRentals()`).
