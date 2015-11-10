CC=gcc
CFLAGS=-c -ansi -pedantic -Wall -Werror

run: lab6 
	./lab6 ./lab.bmp 1

lab6: lab6.o 
	$(CC) lab6.o  -o $@

lab6.o: lab6.c 
	$(CC) -g $(CFLAGS) lab6.c -o $@

clean:
	rm -f lab6 lab6.o
