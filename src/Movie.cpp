#include "Movie.hpp"
#include "Exceptions.hpp"

using namespace std;

Movie::Movie(int id, const string& title, int releaseYear,
             const string& director, int durationMinutes, int ageRating)
    : Media(id, title, releaseYear), director(director),
      durationMinutes(durationMinutes), ageRating(ageRating) {
    if (durationMinutes <= 0) {
        throw InvalidDataException("Czas trwania filmu musi byc wiekszy od zera (podano: " + to_string(durationMinutes) + " min).");
    }
    if (ageRating < 0) {
        throw InvalidDataException("Ograniczenie wiekowe filmu nie moze byc ujemne (podano: " + to_string(ageRating) + ").");
    }
}

string Movie::getDirector() const {
    return director;
}

int Movie::getDurationMinutes() const {
    return durationMinutes;
}

int Movie::getAgeRating() const {
    return ageRating;
}

void Movie::setDirector(const string& newDirector) {
    if (newDirector.empty()) {
        throw InvalidDataException("Rezyser nie moze byc pusty.");
    }
    director = newDirector;
}

void Movie::setDurationMinutes(int duration) {
    if (duration <= 0) {
        throw InvalidDataException("Czas trwania filmu musi byc wiekszy od zera (podano: " + to_string(duration) + " min).");
    }
    durationMinutes = duration;
}

void Movie::setAgeRating(int rating) {
    if (rating < 0) {
        throw InvalidDataException("Ograniczenie wiekowe filmu nie moze byc ujemne (podano: " + to_string(rating) + ").");
    }
    ageRating = rating;
}

void Movie::printDetails(ostream& os) const {
    Media::printDetails(os);
    os << " | Rezyser: " << director
       << " | Czas: " << durationMinutes << " min"
       << " | Od lat: " << ageRating << "+";
}

double Movie::calculateLateFee(int daysOverdue) const {
    if (daysOverdue <= 0) return 0.0;
    return daysOverdue * 2.00; // 2.00 PLN za kazdy dzien spoznienia
}

string Movie::getType() const {
    return "Film";
}
