all: myPing

myPing: myPing.o
	gcc -Wall -g myPing.o -o myPing

myPing.o: myPing.c
	gcc -Wall -g -c myPing.c -o myPing.o

.PHONY: clean all
	
clean: 
	rm *.o myPing
