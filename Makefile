CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
SRC = src/Media.cpp src/User.cpp src/Book.cpp src/Movie.cpp src/LibraryManager.cpp src/DigitalRental.cpp src/main.cpp
BIN_DIR = bin
TARGET = $(BIN_DIR)/library_app.exe

all: $(TARGET)

$(TARGET): $(SRC)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

run: $(TARGET)
	$(TARGET)
