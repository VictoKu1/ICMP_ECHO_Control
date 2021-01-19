all: main
main: main.o
	gcc main.o -o main
main.o: main.c
	gcc -c main.c -o main.o
.PHONY: clean all
clean:
	rm main *.o
