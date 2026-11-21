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

public:
    Movie(int id, const string& title, int releaseYear,
          const string& director, int durationMinutes, int ageRating);

    string getDirector() const;
    int getDurationMinutes() const;
    int getAgeRating() const;

    void setDirector(const string& newDirector);
    void setDurationMinutes(int duration);
    void setAgeRating(int rating);

    void printDetails(ostream& os = cout) const override;
    double calculateLateFee(int daysOverdue) const override;
    string getType() const override;
};

#endif // MOVIE_HPP
