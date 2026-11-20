CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

OBJ = main.o hamiltoniano.o tps.o

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

main.o: main.c hamiltoniano.h tps.h
	$(CC) $(CFLAGS) -c main.c

hamiltoniano.o: hamiltoniano.c hamiltoniano.h
	$(CC) $(CFLAGS) -c hamiltoniano.c

tps.o: tps.c tps.h hamiltoniano.h
	$(CC) $(CFLAGS) -c tps.c

clean:
	rm -f *.o main main.exe
