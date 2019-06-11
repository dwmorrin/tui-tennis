#include "tuitennis.h"

void BallCollisionCheck(struct Gamestate *g) {
    if (g->ball->y <= CEILING + 1) {
        g->ball->y = CEILING + 1;
        g->ball->speedY = -g->ball->speedY;
    }
    if (g->ball->y >= LINES - 3) {
        g->ball->y = LINES - 3;
        g->ball->speedY = -g->ball->speedY;
    }
    if (g->ball->x <= 0) {
        PaddleScore(g->comp);
        g->nextServe = 1;
        g->gameOver = true;
    } else if (g->ball->x >= COLS) {
        PaddleScore(g->player);
        g->nextServe = -1;
        g->gameOver = true;
    }
    if (g->ball->x == g->player->x + 1) {
        PaddleCollisionHandler(g->player, g->ball);
    }
    if (g->ball->x == g->comp->x - 1) {
        PaddleCollisionHandler(g->comp, g->ball);
    }
    return;
}

void BallInit(struct Gamepiece *ball) {
    ball->x = 1;
    ball->y = LINES / 2;
    ball->size = 1;
    ball->speedY = 1;
    ball->speedX = DEFAULT_BALL_SPEED;
}

void BallUpdate(struct Gamestate *g) {
    int lookAheadX;
    if (! g->newFrameFlag ||
          g->frame % g->speed != 0) {
        return;
    }
    /* delete the old ball */
    if (g->ball->y == CEILING) {
        mvaddch(g->ball->y, g->ball->x, '_');
    } else {
        mvaddch(g->ball->y, g->ball->x, ' ');
    }
    /* update ball */
    lookAheadX = g->ball->x + g->ball->speedX;
    if (lookAheadX <= g->player->x + 1 &&
        ! g->interpolateTry) {
        g->interpolateTry = true;
        g->ball->x = g->player->x + 1;
    } else if (lookAheadX >= g->comp->x - 1 &&
        ! g->interpolateTry) {
        g->interpolateTry = true;
        g->ball->x = g->comp->x - 1;
    } else {
        g->interpolateTry = false;
        g->ball->x += g->ball->speedX;
    }
    g->ball->y += g->ball->speedY;
    BallCollisionCheck(g);
    mvaddch(g->ball->y, g->ball->x, 'o');
    return;
}
