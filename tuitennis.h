#ifndef TUITENNIS
#define TUITENNIS

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#define DELAY_60FPS 16667 
#define CEILING 3
#define EXIT_MESSAGE "DONE press any key"

struct Gamepiece {
    int x, y, size, moved, speedX, speedY, score;
};

struct Gamestate {
    int input, speed, frame, newFrameFlag, gameOver;
    struct Gamepiece player, comp, ball;
};

long long getElapsed(struct timeval t0, struct timeval t1);
void initBall(struct Gamestate *g, int direction);
void paintPaddle(struct Gamepiece paddle);
void updateBall(struct Gamestate *g);
void updatePaddles(struct Gamestate *g);

#endif
