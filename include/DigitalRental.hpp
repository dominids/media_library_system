#ifndef DIGITALRENTAL_HPP
#define DIGITALRENTAL_HPP

#include <string>
#include <chrono>
#include <ctime>

using namespace std;

class DigitalRental {
private:
    int movieId;
    string movieTitle;
    chrono::system_clock::time_point rentalTime;
    chrono::system_clock::time_point expirationTime;
    double feePaid;

public:
    DigitalRental();
    DigitalRental(int movieId, const string& title, double fee, int durationHours = 48,
                  chrono::system_clock::time_point startTime = chrono::system_clock::now());

    int getMovieId() const;
    string getMovieTitle() const;
    chrono::system_clock::time_point getRentalTime() const;
    chrono::system_clock::time_point getExpirationTime() const;
    double getFeePaid() const;

    bool isExpired(chrono::system_clock::time_point currentTime = chrono::system_clock::now()) const;
    int getRemainingHours(chrono::system_clock::time_point currentTime = chrono::system_clock::now()) const;
    string getFormattedExpiration() const;
};

#endif // DIGITALRENTAL_HPP
