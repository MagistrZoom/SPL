CC=gcc
CFLAGS=-c -ansi -pedantic -Wall -Werror

run: lab5 
	./lab5

lab5: lab5.o llist.o
	$(CC) lab5.o llist.o -o $@

lab5.o: lab5.c 
	$(CC) -g $(CFLAGS) lab5.c -o $@

llist.o: llist.c
	$(CC) $(CFLAGS) llist.c -o $@

clean:
	rm -f lab5 lab5.o llist.o
