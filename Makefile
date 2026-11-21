CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
BIN_DIR = bin

SRC_COMMON = src/Media.cpp src/User.cpp src/Book.cpp src/Movie.cpp src/LibraryManager.cpp src/DigitalRental.cpp src/LibrarySerializer.cpp
ADMIN_SRC = $(SRC_COMMON) src/main.cpp
USER_SRC = $(SRC_COMMON) src/user_main.cpp

ADMIN_TARGET = $(BIN_DIR)/library_app.exe
USER_TARGET = $(BIN_DIR)/user_app.exe

all: $(ADMIN_TARGET) $(USER_TARGET)

$(ADMIN_TARGET): $(ADMIN_SRC)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(ADMIN_SRC) -o $(ADMIN_TARGET)

$(USER_TARGET): $(USER_SRC)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(USER_SRC) -o $(USER_TARGET)

clean:
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

run-admin: $(ADMIN_TARGET)
	$(ADMIN_TARGET)

run-user: $(USER_TARGET)
	$(USER_TARGET)
