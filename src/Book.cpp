#include "Book.hpp"

using namespace std;

Book::Book(int id, const string& title, int releaseYear,
           const string& author, int pageCount, const string& isbn)
    : Media(id, title, releaseYear), author(author), pageCount(pageCount), isbn(isbn) {}

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
    author = newAuthor;
}

void Book::setPageCount(int pages) {
    pageCount = pages;
}

void Book::setIsbn(const string& newIsbn) {
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
