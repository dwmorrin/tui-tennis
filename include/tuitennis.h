#ifndef TUITENNIS
#define TUITENNIS

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/* magic numbers */
#define CEILING 3
#define DEFAULT_BALL_SPEED 4
#define DELAY_60FPS 16667 
#define DIFFICULTY_MIN 50
#define DIFFICULTY_MAX 150
#define MOVE_SCALE 1 /*LINES / 10*/

/* string constants */
#define BANNER_STRING "TEXT USER INTERFACE TENNIS | up: w, down: s, quit: q"
#define EXIT_MESSAGE "DONE press any key"

struct Gamepiece {
    bool moved;
    int x, y, direction, size, moveX, moveY,
        speedX, speedY, angle, power,
        score;
} player, comp, ball;

struct Gamestate {
    int difficulty, input, speed, frame, nextServe;
    bool newFrameFlag, gameOver, run, interpolateTry;
    struct Gamepiece *player, *comp, *ball;
    struct timeval t0, t1;
} gamestate;

double BallGetPathY(struct Gamepiece*, int);
void BallInit(struct Gamepiece*);
void BallPaint(struct Gamepiece*);
void BallUpdate(struct Gamestate*);
void GamestateCollisionCheck(struct Gamestate*);
void GamestateDifficultyUpdate(struct Gamestate*);
void GamestateInit(struct Gamestate*);
void GamestateReset(struct Gamestate*);
void handleInput(struct Gamestate*);
void handleResize(struct Gamestate*, int oldCols, int oldLines);
void NcursesInit(void);
void NcursesExit(void);
void PaddleAiMove(struct Gamestate*);
void PaddleBoundsCheck(struct Gamepiece*);
bool PaddleCollisionCheck(struct Gamepiece*, struct Gamepiece*);
bool PaddleCollisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball);
void PaddleInit(struct Gamepiece*);
void PaddleMove(struct Gamepiece*, int);
void PaddlePaint(struct Gamepiece*);
void PaddleScore(struct Gamepiece*);
void SpeedUpdate(struct Gamestate*);
long long TimeGetElapsed(struct timeval t0, struct timeval t1);
void TimeUpdate(struct Gamestate*);

#endif
