
# Makefile - Pratica 09

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2

TARGET = main.exe
SRC = cobertura.c main.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c cobertura.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
