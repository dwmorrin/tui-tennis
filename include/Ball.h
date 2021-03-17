#ifndef BALL
#define BALL

#include "Gamepiece.h"
#include "Gamestate.h"

#define DEFAULT_BALL_SPEED 2.0

double BallGetPathY(struct Gamepiece*, double);
void BallInit(struct Gamepiece*);
void BallPaint(struct Gamepiece*);
void BallMove(struct Gamepiece*);
#endif
