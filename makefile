all: *.c
	gcc -Wall -o tuitennis *.c -lncurses
clean:
	rm tuitennis
