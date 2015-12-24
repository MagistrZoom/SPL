CC=gcc
CFLAGS=-c -g # -pedantic -Wall -Werror

run: lab7 


lab7: lab7.o mem.o llist.o
	$(CC)  -o $@ $^

lab7.o: lab7.c
	$(CC) $(CFLAGS) -o $@ $<
mem.o: mem.c
	$(CC) $(CFLAGS) -o $@ $<
llist.o: llist.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f lab7 *.o
