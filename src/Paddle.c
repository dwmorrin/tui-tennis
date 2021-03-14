#include <stdbool.h>
#include <stdlib.h>

#include "Ncurses.h"
#include "Gamestate.h"
#include "Paddle.h"
#include "input.h"

bool compMoves(struct Gamestate *g) {
    int randomNumber = rand() % 100,
        nearnessFactor = g->ball->x * g->ball->speedX;
    return randomNumber > g->difficulty - nearnessFactor;
}

void PaddleAiMove(struct Gamestate *g) {
    if (! g->newFrameFlag) return;
    if (compMoves(g)) {
        /*track ball */
        if (g->ball->y > g->comp->y + 2)
            PaddleMove(g->comp, DOWN);
        else if (g->ball->y < g->comp->y + 2)
            PaddleMove(g->comp, UP);
    }
}

void PaddleBoundsCheck(struct Gamepiece *paddle) {
    if (paddle->moveY > 0 && paddle->y > CEILING + 1) {
        paddle->y -= paddle->moveY;
        paddle->moved = true;
        paddle->moveY = 0;
    } else if (
            paddle->moveY < 0 &&
            paddle->y < LINES - 3 - paddle->size
    ) {
        paddle->y -= paddle->moveY;
        paddle->moved = true;
        paddle->moveY = 0;
    }
}

/**
 * PaddleCollisionHandler tests for collision between paddle and ball
 */
bool PaddleCollisionCheck(struct Gamepiece *paddle, struct Gamepiece *ball) {
   if (ball->y >= paddle->y - 1 &&
       ball->y <= paddle->y + paddle->size) {
       return true;
   } 
   return false;
}

/**
 * PaddleCollisionHandler determines resulting speed for ball
 */
bool PaddleCollisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball) {
    if (! PaddleCollisionCheck(paddle, ball)) return false;
    ball->speedX = -ball->speedX;
    /* move ball off paddle */
    if (ball->speedX > 0) ball->x += 1;
    else ball->x -= 1;

    if (ball->y == paddle->y) {
        ball->speedY = -1;
    } else if (ball->y == paddle->y + 1) {
        ball->speedY = -0.5;
    } else if (ball->y == paddle->y + 2) {
        ball->speedY = 0;
    } else if (ball->y == paddle->y + 3) {
        ball->speedY = 0.5;
    } else if (ball->y == paddle->y + 4) {
        ball->speedY = 1;
    }
    return true;
}

void PaddleInit(struct Gamepiece *p) {
    p->x = p->score = p->direction = p->moveX = p->moveY = 0;
    p->moved = false;
    p->size = 5;
    p->y = LINES / 2 - p->size / 2;
    return;
}

void PaddleMove(struct Gamepiece *paddle, int input) {
    switch (input) {
        case UP:
            paddle->moveY = MOVE_SCALE;
            break;
        case DOWN:
            paddle->moveY = -MOVE_SCALE;
            break;
        case LEFT:
            paddle->moveX = MOVE_SCALE;
            break;
        case RIGHT:
            paddle->moveX = -MOVE_SCALE;
            break;
    }
    PaddleBoundsCheck(paddle);
    PaddlePaint(paddle);
}

void PaddlePaint(struct Gamepiece *paddle) {
    move(CEILING, paddle->x);
    vline(' ', LINES - CEILING - 2);
    move(paddle->y, paddle->x);
    vline('|', paddle->size);
    return;
}

void PaddleScore(struct Gamepiece *paddle) {
    int x = 1; // for player
    if (paddle->x > 0) { // for comp
        x = COLS - 3;
    }
    paddle->score++;
    mvprintw(LINES - 1, x, "%d", paddle->score);
}
