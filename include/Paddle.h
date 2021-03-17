#ifndef PADDLE
#define PADDLE

#include "Gamepiece.h"
#include "Gamestate.h"

#define MOVE_SCALE 2

void PaddleAiMove(struct Gamepiece*, struct Gamepiece*);
void PaddleBoundsCheck(struct Gamepiece*);
bool PaddleCollisionCheck(struct Gamepiece*, struct Gamepiece*);
bool PaddleCollisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball);
void PaddleInit(struct Gamepiece*);
void PaddleMove(struct Gamepiece*, int);
void PaddlePaint(struct Gamepiece*);
void PaddleScore(struct Gamepiece*);
#endif
