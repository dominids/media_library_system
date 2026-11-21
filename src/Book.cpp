#include "Book.hpp"
#include "Exceptions.hpp"

using namespace std;

Book::Book(int id, const string& title, int releaseYear,
           const string& author, int pageCount, const string& isbn)
    : Media(id, title, releaseYear), author(author), pageCount(pageCount), isbn(isbn) {
    if (pageCount <= 0) {
        throw InvalidDataException("Liczba stron ksiazki musi byc wieksza od zera (podano: " + to_string(pageCount) + ").");
    }
}

string Book::getAuthor() const {
    return author;
}

int Book::getPageCount() const {
    return pageCount;
}

string Book::getIsbn() const {
    return isbn;
}

void Book::setAuthor(const string& newAuthor) {
    if (newAuthor.empty()) {
        throw InvalidDataException("Autor nie moze byc pusty.");
    }
    author = newAuthor;
}

void Book::setPageCount(int pages) {
    if (pages <= 0) {
        throw InvalidDataException("Liczba stron ksiazki musi byc wieksza od zera (podano: " + to_string(pages) + ").");
    }
    pageCount = pages;
}

void Book::setIsbn(const string& newIsbn) {
    if (newIsbn.empty()) {
        throw InvalidDataException("Numer ISBN nie moze byc pusty.");
    }
    isbn = newIsbn;
}

void Book::printDetails(ostream& os) const {
    Media::printDetails(os);
    os << " | Autor: " << author
       << " | Strony: " << pageCount
       << " | ISBN: " << isbn;
}

double Book::calculateLateFee(int daysOverdue) const {
    if (daysOverdue <= 0) return 0.0;
    return daysOverdue * 0.50; // 0.50 PLN za kazdy dzien spoznienia
}

string Book::getType() const {
    return "Ksiazka";
}
