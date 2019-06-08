#include "tuitennis.h"

void PaddleAiMove(struct Gamestate *g) {
    if (! g->newFrameFlag) {
        return;
    }
    bool compMoves = rand() % 100 > 95 - ((g->ball.x / 4) * g->ball.speedX);
    if (compMoves) {
        /*track ball */
        if (g->ball.y > g->comp.y + 2 &&
            g->comp.y < LINES - 3 - g->comp.size) {
            g->comp.y++;
            g->comp.moved = 1;
        } else if (g->ball.y < g->comp.y + 2 &&
                   g->comp.y > CEILING + 1) {
            g->comp.y--;
            g->comp.moved = true;
        }
    }
}

void PaddleBoundsCheck(struct Gamestate *g) {
    if (g->player.moveY == 1 && g->player.y > CEILING + 1) {
        g->player.y -= 2;
        g->player.moved = true;
        g->player.moveY = 0;
    } else if (g->player.moveY == -1 && g->player.y < LINES - 3 - g->player.size) {
        g->player.y += 2;
        g->player.moved = true;
        g->player.moveY = 0;
    }
}

/**
 * PaddleCollisionHandler tests for collision between paddle and ball
 *   and determines resulting speed for ball
 */
void PaddleCollisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball) {
   if (ball->y >= paddle->y &&
       ball->y <= paddle->y + paddle->size) {
      ball->speedX = -ball->speedX;
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
   } 
}

void PaddleInit(struct Gamepiece *p) {
    p->x = p->score = p->direction = p->moveX = p->moveY = 0;
    p->moved = false;
    p->size = 5;
    p->y = LINES / 2 - p->size / 2;
    return;
}

void PaddleMoveCheck(struct Gamepiece *paddle) {
    if (paddle->moved) {
        paddle->moved = false;
        PaddlePaint(*paddle);
    }
    return;
}

void PaddlePaint(struct Gamepiece paddle) {
    move(CEILING, paddle.x);
    vline(' ', LINES - CEILING - 2);
    move(paddle.y, paddle.x);
    vline('|', paddle.size);
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

void PaddlesUpdate(struct Gamestate *g) {
    PaddleBoundsCheck(g);
    PaddleAiMove(g);
    PaddleMoveCheck(&g->player);
    PaddleMoveCheck(&g->comp);
    return;
}
