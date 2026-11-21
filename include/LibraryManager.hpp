#ifndef LIBRARYMANAGER_HPP
#define LIBRARYMANAGER_HPP

#include "Media.hpp"
#include "Book.hpp"
#include "Movie.hpp"
#include "User.hpp"
#include "Exceptions.hpp"

#include <vector>
#include <memory>
#include <string>

using namespace std;

class LibraryManager {
private:
    vector<unique_ptr<Media>> catalog;
    vector<User> users;
    int nextMediaId;
    int nextUserId;

public:
    LibraryManager();

    int addBook(const string& title, int year, const string& author, int pageCount, const string& isbn);
    int addMovie(const string& title, int year, const string& director, int durationMinutes, int ageRating);
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

    void seedData();

    const vector<unique_ptr<Media>>& getCatalog() const;
    const vector<User>& getUsers() const;
};

#endif // LIBRARYMANAGER_HPP
