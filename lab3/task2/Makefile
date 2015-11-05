CC=gcc
CFLAGS=-g -c -ansi -pedantic -Wall -Werror
run: task2 
	./task2

task2: task2.o 
	$(CC) -o task2 task2.o 
task2.o: task2.c
	$(CC) $(CFLAGS) task2.c -o task2.o


clean:
	rm -f task2 task2.o
