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

    void removeMedia(int id);
    Media* findMediaById(int id);
    const Media* findMediaById(int id) const;

    User* findUserById(int userId);
    const User* findUserById(int userId) const;

    void updateMediaTitle(int id, const string& newTitle);

    void displayAllMedia() const;
    void displayAllUsers() const;

    void borrowMedia(int mediaId, int userId);
    double returnMedia(int mediaId, int daysOverdue);

    // Dystrybucja cyfrowa (wypozyczenia online na 48h)
    double rentMovieOnline(int movieId, int userId);
    int cleanupAllExpiredDigitalRentals();
    void displayUserDigitalRentals(int userId) const;

    // Symulacja uplywu czasu dla demonstracji automatycznego zwrotu
    void advanceSimulatedTime(int hours);
    int getSimulatedHoursOffset() const;
    chrono::system_clock::time_point getCurrentTime() const;

    void seedData();

    const vector<unique_ptr<Media>>& getCatalog() const;
    const vector<User>& getUsers() const;
};

#endif // LIBRARYMANAGER_HPP
