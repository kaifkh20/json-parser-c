# Define variables for the compiler and flags
CC_LIN = gcc
CC_WIN = x86_64-w64-mingw32-gcc
CFLAGS = -O3 -Wall -Wextra -I./include

# Define all source files for the project and the test runner
SRCS_LIB = src/json_parser.c src/parser.c src/lexer.c src/utility_func.c src/stack.c
SRCS_TEST = main.c
SRCS_ALL = $(SRCS_LIB) $(SRCS_TEST)

# Define targets for the final executables
TARGET_LIN = jp
TARGET_WIN = jp-w.exe

.PHONY: all lin win clean clean-lin clean-win

# Default target: builds both Linux and Windows versions
all: lin win

# Linux build target
lin: $(SRCS_ALL)
	$(CC_LIN) $(CFLAGS) $^ -o $(TARGET_LIN)

# Windows build target
win: $(SRCS_ALL)
	$(CC_WIN) $(CFLAGS) $^ -o $(TARGET_WIN)

# Clean up all build artifacts
clean: clean-lin clean-win

# Clean up build artifacts for Linux
clean-lin:
	rm -f $(TARGET_LIN)

# Clean up build artifacts for Windows
clean-win:
	rm -f $(TARGET_WIN)
