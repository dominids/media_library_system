#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

using namespace std;

class LibraryException : public runtime_error {
public:
    explicit LibraryException(const string& message)
        : runtime_error(message) {}
};

class MediaNotFoundException : public LibraryException {
public:
    explicit MediaNotFoundException(const string& message)
        : LibraryException(message) {}
};

class MediaUnavailableException : public LibraryException {
public:
    explicit MediaUnavailableException(const string& message)
        : LibraryException(message) {}
};

class AgeRestrictionException : public LibraryException {
public:
    explicit AgeRestrictionException(const string& message)
        : LibraryException(message) {}
};

class UserNotFoundException : public LibraryException {
public:
    explicit UserNotFoundException(const string& message)
        : LibraryException(message) {}
};

#endif // EXCEPTIONS_HPP
