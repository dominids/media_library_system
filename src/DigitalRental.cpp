#include "DigitalRental.hpp"
#include <iomanip>
#include <sstream>

using namespace std;

DigitalRental::DigitalRental()
    : movieId(0), movieTitle(""), rentalTime(chrono::system_clock::now()),
      expirationTime(chrono::system_clock::now()), feePaid(0.0) {}

DigitalRental::DigitalRental(int movieId, const string& title, double fee, int durationHours,
                             chrono::system_clock::time_point startTime)
    : movieId(movieId), movieTitle(title), rentalTime(startTime), feePaid(fee) {
    expirationTime = rentalTime + chrono::hours(durationHours);
}

int DigitalRental::getMovieId() const {
    return movieId;
}

string DigitalRental::getMovieTitle() const {
    return movieTitle;
}

chrono::system_clock::time_point DigitalRental::getRentalTime() const {
    return rentalTime;
}

chrono::system_clock::time_point DigitalRental::getExpirationTime() const {
    return expirationTime;
}

double DigitalRental::getFeePaid() const {
    return feePaid;
}

bool DigitalRental::isExpired(chrono::system_clock::time_point currentTime) const {
    return currentTime >= expirationTime;
}

int DigitalRental::getRemainingHours(chrono::system_clock::time_point currentTime) const {
    auto diff = chrono::duration_cast<chrono::hours>(expirationTime - currentTime).count();
    return diff > 0 ? static_cast<int>(diff) : 0;
}

string DigitalRental::getFormattedExpiration() const {
    time_t expTime = chrono::system_clock::to_time_t(expirationTime);
    tm* timeinfo = localtime(&expTime);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}
