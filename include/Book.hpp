#ifndef BOOK_HPP
#define BOOK_HPP

#include "Media.hpp"
#include <string>

using namespace std;

class Book : public Media {
private:
    string author;
    int pageCount;
    string isbn;

public:
    Book(int id, const string& title, int releaseYear,
         const string& author, int pageCount, const string& isbn);

    string getAuthor() const;
    int getPageCount() const;
    string getIsbn() const;

    void setAuthor(const string& newAuthor);
    void setPageCount(int pages);
    void setIsbn(const string& newIsbn);

    void printDetails(ostream& os = cout) const override;
    double calculateLateFee(int daysOverdue) const override;
    string getType() const override;
};

#endif // BOOK_HPP
