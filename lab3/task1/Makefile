CC=gcc
CFLAGS=-g -c -ansi -pedantic -Wall -Werror

all: task1

run: task1 
	./task1

task1: task1.o ../shared.o
	$(CC) -o task1 task1.o ../shared.o

task1.o: task1.c
	$(CC) $(CFLAGS) task1.c -o task1.o

../shared.o: ../shared.c
	$(CC) $(CFLAGS) ../shared.c -o ../shared.o

clean:
	rm -f task1 task1.o
