objs = main.o tuitennis.o
tuitennis: $(objs)
	gcc -Wall -o tuitennis $(objs) -lncurses
main.o: main.c tuitennis.h
	gcc -Wall -c main.c
tuitennis.o: tuitennis.c tuitennis.h
	gcc -Wall -c tuitennis.c
clean:
	rm tuitennis $(objs)
