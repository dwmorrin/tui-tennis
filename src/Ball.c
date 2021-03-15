#include <math.h>
#include <stdbool.h>

#include "Ncurses.h"
#include "Gamestate.h"
#include "Ball.h"


void BallInit(struct Gamepiece *ball) {
    ball->x = 1.0;
    ball->y = LINES / 2.;
    ball->size = 1;
    ball->speedY = 0.5;
    ball->speedX = DEFAULT_BALL_SPEED;
}

void BallUpdate(struct Gamestate *g) {
    if (! g->newFrameFlag) return;

    /* delete the old ball */
    int x = round(g->ball->x);
    int y = round(g->ball->y);
    mvaddch(y, x, y == CEILING ? '_' : ' ');

    int newX = g->ball->x + g->ball->speedX;
    /* update ball position, x limited to paddles */
    if (newX < g->player->x) {
        g->ball->y = BallGetPathY(g->ball, g->player->x);
        g->ball->x = g->player->x;
    } else if (newX > g->comp->x) {
        g->ball->y = BallGetPathY(g->ball, g->comp->x);
        g->ball->x = g->comp->x;
    } else {
        g->ball->y = BallGetPathY(g->ball, newX);
        g->ball->x = newX;
    }
}

void BallPaint(struct Gamepiece *ball) {
    // alternate between two characters for animation
    static bool alt = false;
    alt = !alt;
    mvaddch(round(ball->y), round(ball->x), alt ? '-' : '_');
}


/**
 * gets expected y (line) value for when ball is at
 * position x (column).  use to lookahead at ball's
 * path.
 */
double BallGetPathY(struct Gamepiece *ball, double x) {
    double rate = ball->speedY / ball->speedX;
    return rate * (x - (double)ball->x) + (double)ball->y;
}
