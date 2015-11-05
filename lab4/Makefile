CC=gcc
CFLAGS=-g -c
run: task4 
	./task4

task4: task4.o
	$(CC) -o task4 task4.o 
task4.o: task4.c
	$(CC) $(CFLAGS) task4.c -o task4.o
clean:
	rm -f task4 task4.o
