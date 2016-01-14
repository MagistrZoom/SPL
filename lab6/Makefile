CC=gcc
CFLAGS=-g -c -ansi -pedantic -Wall -Werror

run: lab6 


lab6: lab6.o bmp.o lib.o
	$(CC)  -o $@ $^

lab6.o: lab6.c
	$(CC) $(CFLAGS) -o $@ $<
bmp.o: bmp.c
	$(CC) $(CFLAGS) -o $@ $<
lib.o: lib.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f lab6 *.o
