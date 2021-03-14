#ifndef GAMESTATE
#define GAMESTATE

#include <stdbool.h>
#include <sys/time.h>

#define FRAME_DELAY_uS 100000
#define DIFFICULTY_MIN 50
#define DIFFICULTY_MAX 150

#define BANNER_STRING "TEXT USER INTERFACE TENNIS | up: w, down: s, quit: q"

struct Gamestate {
    int difficulty, input, speed, frame, nextServe, countdown;
    bool newFrameFlag, gameOver, run;
    struct Gamepiece *player, *comp, *ball;
    struct timeval t0, t1;
};

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
bool GamestateTimeUpdate(struct Gamestate*);
#endif
