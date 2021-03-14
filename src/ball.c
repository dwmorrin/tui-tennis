#include "tuitennis.h"


void BallInit(struct Gamepiece *ball) {
    ball->x = 1;
    ball->y = LINES / 2;
    ball->size = 1;
    ball->speedY = 1;
    ball->speedX = DEFAULT_BALL_SPEED;
}

void BallUpdate(struct Gamestate *g) {
    if (! g->newFrameFlag) return;
    /* delete the old ball */
    if (g->ball->y == CEILING) {
        mvaddch(g->ball->y, g->ball->x, '_');
    } else {
        mvaddch(g->ball->y, g->ball->x, ' ');
    }
    int newX = g->ball->x + g->ball->speedX;
    int newY = g->ball->y + g->ball->speedY;
    /* update ball position, x limited to paddles */
    if (newX < g->player->x) {
        newY = BallGetPathY(g->ball, g->player->x);
        newX = g->player->x;
    } else if (newX > g->comp->x) {
        newY = BallGetPathY(g->ball, g->comp->x);
        newX = g->comp->x;
    }
    g->ball->y = newY;
    g->ball->x = newX;
}

void BallPaint(struct Gamepiece *ball) {
    mvaddch(ball->y, ball->x, 'o');
}


/**
 * gets expected y (line) value for when ball is at
 * position x (column).  use to lookahead at ball's
 * path.
 */
double BallGetPathY(struct Gamepiece *ball, int x) {
    double rate = (double)ball->speedY / (double)ball->speedX;
    return rate * (x - ball->x) + ball->y;
}
