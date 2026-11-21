#include "LibrarySerializer.hpp"
#include "Book.hpp"
#include "Movie.hpp"
#include "DigitalRental.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

// Pomocnicze: zamiana time_point na liczbe sekund od epoki i z powrotem
static long long tpToSeconds(chrono::system_clock::time_point tp) {
    return chrono::duration_cast<chrono::seconds>(tp.time_since_epoch()).count();
}

static chrono::system_clock::time_point secondsToTp(long long secs) {
    return chrono::system_clock::time_point(chrono::seconds(secs));
}

// Pomocnicze: escape/unescape znaku | w polach
static string escapeField(const string& s) {
    string out;
    for (char c : s) {
        if (c == '|') out += "\\p";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else out += c;
    }
    return out;
}

static string unescapeField(const string& s) {
    string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char next = s[i + 1];
            if (next == 'p') { out += '|'; ++i; }
            else if (next == '\\') { out += '\\'; ++i; }
            else if (next == 'n') { out += '\n'; ++i; }
            else { out += s[i]; }
        } else {
            out += s[i];
        }
    }
    return out;
}

// Parsowanie linii na pola rozdzielone |
static vector<string> splitLine(const string& line) {
    vector<string> fields;
    string cur;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '\\' && i + 1 < line.size()) {
            cur += line[i];
            cur += line[i + 1];
            ++i;
        } else if (line[i] == '|') {
            fields.push_back(unescapeField(cur));
            cur.clear();
        } else {
            cur += line[i];
        }
    }
    fields.push_back(unescapeField(cur));
    return fields;
}

void LibrarySerializer::saveToFile(const string& path, const LibraryManager& mgr) {
    ofstream f(path, ios::out | ios::trunc);
    if (!f.is_open()) {
        throw runtime_error("Nie mozna otworzyc pliku do zapisu: " + path);
    }

    // META
    f << "[META]\n";
    f << mgr.getNextMediaId() << "|" << mgr.getNextUserId() << "\n";

    // BOOKS
    f << "[BOOKS]\n";
    for (const auto& item : mgr.getCatalog()) {
        const Book* b = dynamic_cast<const Book*>(item.get());
        if (!b) continue;
        // id|title|year|available|borrowedBy|borrowTimeSec|dueTimeSec|author|pages|isbn
        long long bt = b->getIsAvailable() ? 0 : tpToSeconds(b->getBorrowTime());
        long long dt = b->getIsAvailable() ? 0 : tpToSeconds(b->getDueTime());
        f << b->getId() << "|"
          << escapeField(b->getTitle()) << "|"
          << b->getReleaseYear() << "|"
          << (b->getIsAvailable() ? 1 : 0) << "|"
          << b->getBorrowedByUserId() << "|"
          << bt << "|"
          << dt << "|"
          << escapeField(b->getAuthor()) << "|"
          << b->getPageCount() << "|"
          << escapeField(b->getIsbn()) << "\n";
    }

    // MOVIES
    f << "[MOVIES]\n";
    for (const auto& item : mgr.getCatalog()) {
        const Movie* m = dynamic_cast<const Movie*>(item.get());
        if (!m) continue;
        // id|title|year|available|borrowedBy|borrowTimeSec|dueTimeSec|director|duration|ageRating|digitalPrice
        long long bt = m->getIsAvailable() ? 0 : tpToSeconds(m->getBorrowTime());
        long long dt = m->getIsAvailable() ? 0 : tpToSeconds(m->getDueTime());
        f << m->getId() << "|"
          << escapeField(m->getTitle()) << "|"
          << m->getReleaseYear() << "|"
          << (m->getIsAvailable() ? 1 : 0) << "|"
          << m->getBorrowedByUserId() << "|"
          << bt << "|"
          << dt << "|"
          << escapeField(m->getDirector()) << "|"
          << m->getDurationMinutes() << "|"
          << m->getAgeRating() << "|"
          << m->getDigitalPrice() << "\n";
    }

    // USERS
    f << "[USERS]\n";
    for (const auto& u : mgr.getUsers()) {
        // id|fullName|age|username|passwordHash
        f << u.getUserId() << "|"
          << escapeField(u.getFullName()) << "|"
          << u.getAge() << "|"
          << escapeField(u.getUsername()) << "|"
          << escapeField(u.getPasswordHash()) << "\n";
    }

    // RENTALS (cyfrowe)
    f << "[RENTALS]\n";
    for (const auto& u : mgr.getUsers()) {
        for (const auto& r : u.getDigitalRentals()) {
            // userId|movieId|movieTitle|feePaid|rentalTimeSec|expirationTimeSec
            f << u.getUserId() << "|"
              << r.getMovieId() << "|"
              << escapeField(r.getMovieTitle()) << "|"
              << r.getFeePaid() << "|"
              << tpToSeconds(r.getRentalTime()) << "|"
              << tpToSeconds(r.getExpirationTime()) << "\n";
        }
    }

    f.close();
}

void LibrarySerializer::loadFromFile(const string& path, LibraryManager& mgr) {
    ifstream f(path);
    if (!f.is_open()) {
        // Plik nie istnieje - nic nie ladujemy (pierwsze uruchomienie)
        return;
    }

    mgr.clearAll();

    string line;
    string section;
    while (getline(f, line)) {
        if (line.empty()) continue;
        // Usun \r jesli CRLF
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        if (line[0] == '[') {
            section = line;
            continue;
        }

        auto fields = splitLine(line);

        if (section == "[META]") {
            if (fields.size() >= 2) {
                mgr.setNextMediaId(stoi(fields[0]));
                mgr.setNextUserId(stoi(fields[1]));
            }
        }
        else if (section == "[BOOKS]") {
            if (fields.size() < 10) continue;
            int id       = stoi(fields[0]);
            string title = fields[1];
            int year     = stoi(fields[2]);
            bool avail   = (stoi(fields[3]) == 1);
            int borrowBy = stoi(fields[4]);
            long long bt = stoll(fields[5]);
            long long dt = stoll(fields[6]);
            string auth  = fields[7];
            int pages    = stoi(fields[8]);
            string isbn  = fields[9];

            mgr.loadBook(id, title, year, auth, pages, isbn, avail, borrowBy,
                         secondsToTp(bt), secondsToTp(dt));
        }
        else if (section == "[MOVIES]") {
            if (fields.size() < 11) continue;
            int id        = stoi(fields[0]);
            string title  = fields[1];
            int year      = stoi(fields[2]);
            bool avail    = (stoi(fields[3]) == 1);
            int borrowBy  = stoi(fields[4]);
            long long bt  = stoll(fields[5]);
            long long dt  = stoll(fields[6]);
            string dir    = fields[7];
            int dur       = stoi(fields[8]);
            int age       = stoi(fields[9]);
            double price  = stod(fields[10]);

            mgr.loadMovie(id, title, year, dir, dur, age, price, avail, borrowBy,
                          secondsToTp(bt), secondsToTp(dt));
        }
        else if (section == "[USERS]") {
            if (fields.size() < 5) continue;
            int id         = stoi(fields[0]);
            string name    = fields[1];
            int age        = stoi(fields[2]);
            string uname   = fields[3];
            string phash   = fields[4];

            mgr.loadUser(id, name, age, uname, phash);
        }
        else if (section == "[RENTALS]") {
            if (fields.size() < 6) continue;
            int userId      = stoi(fields[0]);
            int movieId     = stoi(fields[1]);
            string title    = fields[2];
            double fee      = stod(fields[3]);
            long long rt    = stoll(fields[4]);
            long long et    = stoll(fields[5]);

            mgr.loadDigitalRental(userId, movieId, title, fee,
                                  secondsToTp(rt), secondsToTp(et));
        }
    }

    f.close();
}
