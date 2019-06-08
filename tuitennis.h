#ifndef TUITENNIS
#define TUITENNIS

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#define DELAY_60FPS 16667 
#define CEILING 3
#define DEFAULT_BALL_SPEED 4
#define BANNER_STRING "TEXT USER INTERFACE TENNIS | up: w, down: s, quit: q"
#define EXIT_MESSAGE "DONE press any key"

struct Gamepiece {
    bool moved;
    int x, y, direction, size, moveX, moveY,
        speedX, speedY, angle, power,
        score;
} player, comp, ball;

/**
 * interpolateTry is a flag when the speed of the ball is >1 and we need to check
 *   if the paddle can intercept it or not.  We need to try the interpolation only
 *   once or else the ball gets stuck.
 */
struct Gamestate {
    int input, speed, frame, nextServe;
    bool newFrameFlag, gameOver, run, interpolateTry;
    struct Gamepiece player, comp, ball;
    struct timeval t0, t1;
} gamestate;

void BallInit(struct Gamepiece*);
void collisionCheck(struct Gamestate *g);
void collisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball);
long long getElapsed(struct timeval t0, struct timeval t1);
void GameReset(struct Gamestate*);
void GamestateInit(struct Gamestate*);
void handleInput(struct Gamestate *g);
void handleResize(struct Gamestate *g, int oldCols, int oldLines);
void NcursesInit(void);
void NcursesExit(void);
void PaddleInit(struct Gamepiece*);
void PaddleMoveCheck(struct Gamepiece*);
void PaddlePaint(struct Gamepiece);
void PaddleScore(struct Gamepiece*);
void updateBall(struct Gamestate *g);
void updatePaddles(struct Gamestate *g);
void updateSpeed(struct Gamestate *g);
void updateTime(struct Gamestate *g);

#endif
