CC=gcc
FLAGS= -Wall -g

all: main main.o

main: main.o
	$(CC) $(FLAGS) -o main main.o

main.o: main.c header.h
	$(CC) $(FLAGS) -c main.c header.h

.PHONY: clean all

clean:
	rm -f main *.o *.gch