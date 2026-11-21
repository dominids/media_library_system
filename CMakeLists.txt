cmake_minimum_required(VERSION 3.15)
project(MediaLibrarySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(library_app ${SOURCES})

if (MSVC)
    target_compile_options(library_app PRIVATE /W4)
else()
    target_compile_options(library_app PRIVATE -Wall -Wextra -pedantic)
endif()
