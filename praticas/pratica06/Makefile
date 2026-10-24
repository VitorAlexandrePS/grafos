CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: main

main: main.o dijkstra.o
	$(CC) $(CFLAGS) -o main main.o dijkstra.o

main.o: main.c dijkstra.h
	$(CC) $(CFLAGS) -c main.c

dijkstra.o: dijkstra.c dijkstra.h
	$(CC) $(CFLAGS) -c dijkstra.c

clean:
	rm -f *.o main main.exe