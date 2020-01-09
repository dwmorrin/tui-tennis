#include "tuitennis.h"

void PaddleAiMove(struct Gamestate *g) {
    if (! g->newFrameFlag) {
        return;
    }
    int randomNumber = rand() % 100,
        nearnessFactor = g->ball->x * g->ball->speedX / 4;
    bool compMoves = randomNumber > g->difficulty - nearnessFactor;
    if (compMoves) {
        /*track ball */
        if (g->ball->y > g->comp->y + 2 &&
            g->comp->y < LINES - 3 - g->comp->size) {
            g->comp->y++;
            g->comp->moved = true;
        } else if (g->ball->y < g->comp->y + 2 &&
                   g->comp->y > CEILING + 1) {
            g->comp->y--;
            g->comp->moved = true;
        }
    }
    if (g->comp->moved) {
        g->comp->moved = false;
        PaddlePaint(g->comp);
    }
}

void PaddleBoundsCheck(struct Gamepiece *paddle) {
    if (paddle->moveY == 1 && paddle->y > CEILING + 1) {
        paddle->y -= 2;
        paddle->moved = true;
        paddle->moveY = 0;
    } else if (paddle->moveY == -1 && paddle->y < LINES - 3 - paddle->size) {
        paddle->y += 2;
        paddle->moved = true;
        paddle->moveY = 0;
    }
}

/**
 * PaddleCollisionHandler tests for collision between paddle and ball
 */
bool PaddleCollisionCheck(struct Gamepiece *paddle, struct Gamepiece *ball) {
   if (ball->y >= paddle->y &&
       ball->y <= paddle->y + paddle->size) {
       return true;
   } 
   return false;
}

/**
 * PaddleCollisionHandler determines resulting speed for ball
 */
bool PaddleCollisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball) {
    if (! PaddleCollisionCheck(paddle, ball)) {
        return false;
    }
    ball->speedX = -ball->speedX;
    /* move ball off paddle */
    if (ball->speedX > 0) {
        ball->x += 1;
    } else {
        ball->x -= 1;
    }
    if (ball->y == paddle->y) {
        ball->speedY = -2;
    } else if (ball->y == paddle->y + 1) {
        ball->speedY = -1;
    } else if (ball->y == paddle->y + 2) {
        ball->speedY = 0;
    } else if (ball->y == paddle->y + 3) {
        ball->speedY = 1;
    } else if (ball->y == paddle->y + 4) {
        ball->speedY = 2;
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
        case 'w':
            paddle->moveY = MOVE_SCALE;
            break;
        case 's':
            paddle->moveY = -MOVE_SCALE;
            break;
        case 'a':
            paddle->moveX = MOVE_SCALE;
            break;
        case 'd':
            paddle->moveX = -MOVE_SCALE;
            break;
    }
    PaddleBoundsCheck(paddle);
    PaddlePaint(paddle);
    return;
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
