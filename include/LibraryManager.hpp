#ifndef LIBRARYMANAGER_HPP
#define LIBRARYMANAGER_HPP

#include "Media.hpp"
#include "Book.hpp"
#include "Movie.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include "DigitalRental.hpp"

#include <vector>
#include <memory>
#include <string>
#include <chrono>

using namespace std;

struct ReturnInfo {
    double fee;
    int daysOverdue;
    string dueDate;
};

class LibraryManager {
private:
    vector<unique_ptr<Media>> catalog;
    vector<User> users;
    int nextMediaId;
    int nextUserId;
    int simulatedHoursOffset;

public:
    LibraryManager();

    int addBook(const string& title, int year, const string& author, int pageCount, const string& isbn);
    int addMovie(const string& title, int year, const string& director, int durationMinutes, int ageRating, double digitalPrice = 14.99);
    int addUser(const string& fullName, int age);
    int addUserWithLogin(const string& fullName, int age, const string& username, const string& rawPassword);

    User* findUserByLogin(const string& username);

    void removeMedia(int id);
    Media* findMediaById(int id);
    const Media* findMediaById(int id) const;

    User* findUserById(int userId);
    const User* findUserById(int userId) const;

    void updateMediaTitle(int id, const string& newTitle);

    void displayAllMedia() const;
    void displayAllUsers() const;

    void borrowMedia(int mediaId, int userId);
    ReturnInfo returnMedia(int mediaId);
    void extendLoan(int mediaId, int userId); // przedluzenie o 7 dni (bezplatne)

    // Dystrybucja cyfrowa (wypozyczenia online na 48h)
    double rentMovieOnline(int movieId, int userId);
    int cleanupAllExpiredDigitalRentals();
    void displayUserDigitalRentals(int userId) const;

    // Symulacja uplywu czasu dla demonstracji automatycznego zwrotu i kar
    void advanceSimulatedTime(int hours);
    int getSimulatedHoursOffset() const;
    chrono::system_clock::time_point getCurrentTime() const;

    void seedData();

    const vector<unique_ptr<Media>>& getCatalog() const;
    const vector<User>& getUsers() const;
    vector<User>& getUsers();

    // Metody uzywane przez serializer do odczytu/zapisu stanu
    int getNextMediaId() const;
    int getNextUserId() const;
    void setNextMediaId(int id);
    void setNextUserId(int id);
    void clearAll();

    // Metody ladowania obiektow z pliku (bez walidacji biznesowej)
    void loadBook(int id, const string& title, int year,
                  const string& author, int pageCount, const string& isbn,
                  bool available, int borrowedBy,
                  chrono::system_clock::time_point borrowTime,
                  chrono::system_clock::time_point dueTime);

    void loadMovie(int id, const string& title, int year,
                   const string& director, int durationMinutes, int ageRating,
                   double digitalPrice, bool available, int borrowedBy,
                   chrono::system_clock::time_point borrowTime,
                   chrono::system_clock::time_point dueTime);

    void loadUser(int id, const string& fullName, int age,
                  const string& username, const string& passwordHash);

    void loadDigitalRental(int userId, int movieId, const string& movieTitle,
                           double feePaid,
                           chrono::system_clock::time_point rentalTime,
                           chrono::system_clock::time_point expirationTime);

    bool loginExists(const string& username) const;
};

#endif // LIBRARYMANAGER_HPP
