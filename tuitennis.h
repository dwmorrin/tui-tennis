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
    int x, y, size, moved, moveX, moveY,
        speedX, speedY, angle, power,
        score;
};

struct Gamestate {
    int input, speed, frame, newFrameFlag, gameOver, nextServe, run;
    struct Gamepiece player, comp, ball;
};

void collisionCheck(struct Gamestate *g);
long long getElapsed(struct timeval t0, struct timeval t1);
void handleInput(struct Gamestate *g);
void handleResize(struct Gamestate *g, int oldCols, int oldLines);
void initBall(struct Gamestate *g);
void paintPaddle(struct Gamepiece paddle);
void updateBall(struct Gamestate *g);
void updatePaddles(struct Gamestate *g);
void updateSpeed(struct Gamestate *g);

#endif
