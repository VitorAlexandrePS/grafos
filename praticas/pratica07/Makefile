CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

main: main.o bellman_ford.o
	$(CC) $(CFLAGS) -o main main.o bellman_ford.o

main.o: main.c bellman_ford.h
	$(CC) $(CFLAGS) -c main.c

bellman_ford.o: bellman_ford.c bellman_ford.h
	$(CC) $(CFLAGS) -c bellman_ford.c

clean:
	rm -f *.o main main.exe
