#ifndef LIBRARYSERIALIZER_HPP
#define LIBRARYSERIALIZER_HPP

#include "LibraryManager.hpp"
#include <string>

using namespace std;

// Zapisuje i wczytuje stan LibraryManager do prostego pliku tekstowego.
// Format pliku: sekcje oznaczone naglowkami [BOOKS], [MOVIES], [USERS], [RENTALS], [META]
// Kazdy obiekt zajmuje jedna linie, pola oddzielone |
class LibrarySerializer {
public:
    static void saveToFile(const string& path, const LibraryManager& mgr);
    static void loadFromFile(const string& path, LibraryManager& mgr);
};

#endif // LIBRARYSERIALIZER_HPP
