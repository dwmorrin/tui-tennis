all: *.c
	gcc -o tuitennis *.c -lncurses
clean:
	rm tuitennis
