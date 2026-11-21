cmake_minimum_required(VERSION 3.15)
project(MediaLibrarySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(COMMON_SOURCES
    src/Media.cpp
    src/User.cpp
    src/Book.cpp
    src/Movie.cpp
    src/LibraryManager.cpp
    src/DigitalRental.cpp
    src/LibrarySerializer.cpp
)

add_executable(library_app ${COMMON_SOURCES} src/main.cpp)
add_executable(user_app ${COMMON_SOURCES} src/user_main.cpp)

if (MSVC)
    target_compile_options(library_app PRIVATE /W4)
    target_compile_options(user_app PRIVATE /W4)
else()
    target_compile_options(library_app PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(user_app PRIVATE -Wall -Wextra -pedantic)
endif()
