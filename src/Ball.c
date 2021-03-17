#include <math.h>
#include <ncurses.h>
#include <stdbool.h>

#include "Tui.h"
#include "Gamestate.h"
#include "Ball.h"


void BallInit(struct Gamepiece *ball) {
    ball->x = 1.0;
    ball->y = LINES / 2.;
    ball->size = 1;
    ball->speedY = 0.5;
    ball->speedX = DEFAULT_BALL_SPEED;
}

void BallMove(struct Gamepiece *ball) {
    /* delete the old ball */
    int y = round(ball->y);
    mvaddch(y, round(ball->x), y == CEILING ? '_' : ' ');

    ball->x += ball->speedX;
    ball->y += ball->speedY;
}

void BallPaint(struct Gamepiece *ball) {
    // alternate between two characters for animation
    static bool alt = false;
    alt = !alt;

    //mvaddch(round(ball->y), round(ball->x), alt ? '-' : '_');
    //if (alt) mvaddch(round(ball->y), round(ball->x), 'o');
    mvaddch(round(ball->y), round(ball->x), 'o');
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
