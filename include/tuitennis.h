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
#define DEFAULT_BALL_SPEED 1
#define FRAME_DELAY_uS 100000
#define DIFFICULTY_MIN 50
#define DIFFICULTY_MAX 150
#define MOVE_SCALE 2

/* controls */
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define QUIT 'q'

/* string constants */
#define BANNER_STRING "TEXT USER INTERFACE TENNIS | up: w, down: s, quit: q"
#define EXIT_MESSAGE "DONE press any key"

struct Gamepiece {
    bool moved;
    int x, y, direction, size, moveX, moveY,
        speedX, speedY, angle, power,
        score;
};

struct Gamestate {
    int difficulty, input, speed, frame, nextServe, countdown;
    bool newFrameFlag, gameOver, run;
    struct Gamepiece *player, *comp, *ball;
    struct timeval t0, t1;
};

double BallGetPathY(struct Gamepiece*, int);
void BallInit(struct Gamepiece*);
void BallPaint(struct Gamepiece*);
void BallUpdate(struct Gamestate*);
void GamestateCollisionCheck(struct Gamestate*);
void GamestateDifficultyUpdate(struct Gamestate*);
void GamestateInit(
    struct Gamestate*,
    struct Gamepiece*,
    struct Gamepiece*,
    struct Gamepiece*
);
void GamestateOnInput(struct Gamestate*);
void GamestateOnResize(struct Gamestate*);
void GamestatePrintCountdown(struct Gamestate *);
void GamestatePrintMessage(const char*);
void GamestatePrintTopLines(void);
void GamestateReset(struct Gamestate*);
void GamestateSpeedUpdate(struct Gamestate*);
void NcursesInit(void);
void NcursesExit(void);
void NcursesPrintCenter(const char*);
void PaddleAiMove(struct Gamestate*);
void PaddleBoundsCheck(struct Gamepiece*);
bool PaddleCollisionCheck(struct Gamepiece*, struct Gamepiece*);
bool PaddleCollisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball);
void PaddleInit(struct Gamepiece*);
void PaddleMove(struct Gamepiece*, int);
void PaddlePaint(struct Gamepiece*);
void PaddleScore(struct Gamepiece*);
long long TimeGetElapsed(struct timeval t0, struct timeval t1);
bool TimeUpdate(struct Gamestate*);

#endif
