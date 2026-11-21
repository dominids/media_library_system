#ifndef MOVIE_HPP
#define MOVIE_HPP

#include "Media.hpp"
#include <string>

using namespace std;

class Movie : public Media {
private:
    string director;
    int durationMinutes;
    int ageRating;
    double digitalPrice;

public:
    Movie(int id, const string& title, int releaseYear,
          const string& director, int durationMinutes, int ageRating,
          double digitalPrice = 14.99);

    string getDirector() const;
    int getDurationMinutes() const;
    int getAgeRating() const;
    double getDigitalPrice() const;

    void setDirector(const string& newDirector);
    void setDurationMinutes(int duration);
    void setAgeRating(int rating);
    void setDigitalPrice(double price);

    void printDetails(ostream& os = cout) const override;
    double calculateLateFee(int daysOverdue) const override;
    string getType() const override;
};

#endif // MOVIE_HPP
