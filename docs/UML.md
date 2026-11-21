# Diagram Klas UML i Relacje w Projekcie

Poniższy diagram przedstawia architekturę obiektową Systemu Zarządzania Wypożyczalnią Mediów wzbogaconą o moduł **dystrybucji cyfrowej filmów online (VOD 48h)**, **uwierzytelnianie użytkowników z hashowaniem haseł**, **dwustronną synchronizację bazy w pliku (`LibrarySerializer`)** oraz **obsługę dwóch terminali**.

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
        +void setRawBorrowState(bool avail, int borrowBy, time_point bTime, time_point dTime)
        +void extendDueDate(int days)
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
        +Book(int id, string title, int year, string author, int pages, string isbn)
        +string getAuthor() const
        +int getPageCount() const
        +string getIsbn() const
        +void setAuthor(string newAuthor)
        +void setPageCount(int pages)
        +void setIsbn(string newIsbn)
        +void printDetails(ostream& os) const
        +double calculateLateFee(int daysOverdue) const
        +string getType() const
    }

    class Movie {
        -string director
        -int durationMinutes
        -int ageRating
        -double digitalPrice
        +Movie(int id, string title, int year, string director, int dur, int rating, double price)
        +string getDirector() const
        +int getDurationMinutes() const
        +int getAgeRating() const
        +double getDigitalPrice() const
        +void setDirector(string newDirector)
        +void setDurationMinutes(int duration)
        +void setAgeRating(int rating)
        +void setDigitalPrice(double price)
        +void printDetails(ostream& os) const
        +double calculateLateFee(int daysOverdue) const
        +string getType() const
    }

    class DigitalRental {
        -int movieId
        -string movieTitle
        -time_point rentalTime
        -time_point expirationTime
        -double feePaid
        +DigitalRental()
        +DigitalRental(int id, string title, double fee, int durationHours, time_point startTime)
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
        -string username
        -string passwordHash
        -vector~DigitalRental~ digitalRentals
        +User()
        +User(int id, string name, int age)
        +User(int id, string name, int age, string username, string rawPassword)
        +int getUserId() const
        +string getFullName() const
        +int getAge() const
        +string getUsername() const
        +string getPasswordHash() const
        +void setFullName(string name)
        +void setAge(int age)
        +void setUsername(string uname)
        +void setPassword(string rawPassword)
        +bool checkPassword(string rawPassword) const
        +bool hasLogin() const
        +void addDigitalRental(const DigitalRental& rental)
        +vector~DigitalRental~ getDigitalRentals() const
        +bool hasActiveDigitalRental(int movieId, time_point currentTime) const
        +int cleanupExpiredRentals(time_point currentTime)
        +void displayDigitalRentals(ostream& os, time_point currentTime) const
    }

    class LibrarySerializer {
        +static void saveToFile(string path, const LibraryManager& mgr)
        +static void loadFromFile(string path, LibraryManager& mgr)
    }

    class LibraryManager {
        -vector~unique_ptr~Media~~ catalog
        -vector~User~ users
        -int nextMediaId
        -int nextUserId
        -int simulatedHoursOffset
        +LibraryManager()
        +int addBook(...)
        +int addMovie(...)
        +int addUser(...)
        +int addUserWithLogin(...)
        +User* findUserByLogin(string username)
        +bool loginExists(string username) const
        +void removeMedia(int id)
        +Media* findMediaById(int id)
        +User* findUserById(int userId)
        +void updateMediaTitle(int id, string newTitle)
        +void displayAllMedia() const
        +void displayAllUsers() const
        +void borrowMedia(int mediaId, int userId)
        +ReturnInfo returnMedia(int mediaId)
        +void extendLoan(int mediaId, int userId)
        +double rentMovieOnline(int movieId, int userId)
        +int cleanupAllExpiredDigitalRentals()
        +void displayUserDigitalRentals(int userId) const
        +void advanceSimulatedTime(int hours)
        +time_point getCurrentTime() const
        +void seedData()
    }

    Media <|-- Book : dziedziczenie
    Media <|-- Movie : dziedziczenie
    LibraryManager *-- Media : zarzadzanie pamiecia (unique_ptr)
    LibraryManager o-- User : agregacja
    User *-- DigitalRental : kompozycja subskrypcji
    LibrarySerializer ..> LibraryManager : odczyt i zapis stanu (persistence)
```

---

## Opis Relacji i Wzorców Architektonicznych

1. **Dwa Niezależne Panele Konsolowe**:
   - `library_app.exe` — interfejs administratora (CRUD mediów, użytkowników, tradycyjne wypożyczenia i zwroty z karami, symulacja upływu czasu).
   - `user_app.exe` — interfejs klienta (uwierzytelnianie, rejestracja, VOD na 48h, darmowe przedłużanie wypożyczeń o 7 dni, sprawdzanie subskrypcji).
2. **Persistence i Synchronizacja Stanu (`LibrarySerializer`)**:
   - Stan obu aplikacji synchronizowany jest w czasie rzeczywistym poprzez plik tekstowy `data/library.dat`. Każda operacja zapisu aktualizuje plik, a operacje odczytu synchronizują stan przed prezentacją.
3. **Bezpieczeństwo i Hashowanie Haseł**:
   - Klasa `User` przechowuje wyłącznie skrót hasła wyliczony algorytmem djb2.
4. **Model Czasu i Dystrybucja VOD**:
   - Automatyczne wygaszanie dostępu do wypożyczeń online po 48h oraz automatyczna kalkulacja kar za zwroty fizyczne.
