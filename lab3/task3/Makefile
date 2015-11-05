CC=gcc
CFLAGS=-g -c -ansi -pedantic -Wall -Werror

all: task3

run: task3 
	./task3

task3: task3.o ../shared.o
	$(CC) -o task3 task3.o ../shared.o
task3.o: task3.c
	$(CC) $(CFLAGS) task3.c -o task3.o
../shared.o: ../shared.c
	$(CC) $(CFLAGS) ../shared.c -o ../shared.o
clean:
	rm -f task3 task3.o 
