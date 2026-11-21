#ifndef MEDIA_HPP
#define MEDIA_HPP

#include <iostream>
#include <string>

using namespace std;

class Media {
protected:
    int id;
    string title;
    int releaseYear;
    bool isAvailable;
    int borrowedByUserId;

public:
    Media();
    Media(int id, const string& title, int releaseYear);
    virtual ~Media() = default;

    int getId() const;
    string getTitle() const;
    int getReleaseYear() const;
    bool getIsAvailable() const;
    int getBorrowedByUserId() const;

    void setTitle(const string& newTitle);
    void setReleaseYear(int year);
    void setAvailable(bool status);
    void setBorrowedByUserId(int userId);

    virtual void printDetails(ostream& os = cout) const;
    virtual double calculateLateFee(int daysOverdue) const = 0;
    virtual string getType() const = 0;

    bool operator==(const Media& other) const;
    friend ostream& operator<<(ostream& os, const Media& media);
};

#endif // MEDIA_HPP
