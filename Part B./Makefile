CC=gcc
FLAGS= -Wall -g

all: icmpSniff main.o

icmpSniff: main.o
	$(CC) $(FLAGS) -o icmpSniff main.o

main.o: main.c header.h
	$(CC) $(FLAGS) -c main.c header.h

.PHONY: clean all

clean:
	rm -f icmpSniff *.o *.gch
