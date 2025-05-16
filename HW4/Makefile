# Makefile for PL/ONE compiler

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable
TARGET = plone

# Source files
SRCS = plone.c resword.c
OBJS = $(SRCS:.c=.o)
HEADERS = scanner.h err.h followsym.h idobj.h procobj.h sym.h resword.h symbol.h symname.h

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation rules
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)

# Run with a test file
test: $(TARGET)
	./$(TARGET) test1.pl

# Phony targets
.PHONY: all clean test