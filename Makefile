# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra

# Source and object files
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
TARGET = atm

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^

# Pattern rule for building object files from source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c src/header.h
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target for cleaning up
.PHONY: clean

# Clean target
clean:
	rm -f $(OBJ_FILES) $(TARGET)