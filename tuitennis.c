#include "tuitennis.h"

void handleInput(struct Gamestate *g) {
    switch (g->input) {
        case 'w':
            g->player.moveY = 1;
            break;
        case 's':
            g->player.moveY = -1;
            break;
        case 'a':
            g->player.moveX = 1;
            break;
        case 'd':
            g->player.moveX = -1;
            break;
        case 'j':
        case 'k':
            updateSpeed(g);
            break;
        case 'q':
            g->run = 0;
            break;
    }
    g->input = ERR;
}

void handleResize(struct Gamestate *g, int oldCols, int oldLines) {
    // TODO need to blank and redraw screen, possibly refactor main.c
    // into an init function we can call here
    initBall(g);
    return;
}

long long getElapsed(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_usec - t0.tv_usec);
}

void initBall(struct Gamestate *g) {
    char *readysetgo[] = {"READY", "SET", "GO"};
    int i, l;
    if (g->nextServe == 1) { /* player's serve */
        g->ball.x = 2;
    } else {
        g->ball.x = COLS - 3;
    }
    g->ball.y = LINES / 2;
    g->ball.speedY = 1;
    g->ball.speedX = 1 * g->nextServe;
    paintPaddle(g->player);
    paintPaddle(g->comp);
    updateSpeed(g);

    for (i = 0; i < 3; i++) {
        l = strlen(readysetgo[i]);
        mvprintw(LINES/2, COLS/2 - l/2, "%s", readysetgo[i]);
        refresh();
        sleep(1);
        mvprintw(LINES/2, COLS/2 - l/2, "%s", "          ");
    } // TODO something better for blanking the line

    return;
}

void paintPaddle(struct Gamepiece paddle) {
    move(CEILING, paddle.x);
    vline(' ', LINES - CEILING - 2);
    move(paddle.y, paddle.x);
    vline('|', paddle.size);
    return;
}

void updateSpeed(struct Gamestate *g) {
    if (g->input == 'k' && g->speed > 1) {
        g->speed--;
    } else if (g->input == 'j' && g->speed < 10) {
        g->speed++;
    }
    mvprintw(1, 13, "%d  (slower: j, faster: k)", 11 - g->speed);
    return;
}

void updateBall(struct Gamestate *g) {
    if (! g->newFrameFlag ||
          g->frame % g->speed != 0) {
        return;
    }
    /* delete the old ball */
    if (g->ball.y == CEILING) {
        mvaddch(g->ball.y, g->ball.x, '_');
    } else {
        mvaddch(g->ball.y, g->ball.x, ' ');
    }
    /* update ball */
    g->ball.y += g->ball.speedY;
    g->ball.x += g->ball.speedX;
    collisionCheck(g);
    mvaddch(g->ball.y, g->ball.x, 'o');
    return;
}

void score(struct Gamepiece *g) {
    int x = 1; // for player
    if (g->x > 0) { // for comp
        x = COLS - 3;
    }
    g->score++;
    mvprintw(LINES - 1, x, "%d", g->score);
}

void collisionCheck(struct Gamestate *g) {
    if (g->ball.y <= CEILING + 1 || g->ball.y >= LINES - 3) {
        g->ball.speedY = -g->ball.speedY;
    }
    if (g->ball.x == 0) {
        score(&g->comp);
        g->nextServe = 1;
        g->gameOver = 1;
    } else if (g->ball.x == COLS) {
        score(&g->player);
        g->nextServe = -1;
        g->gameOver = 1;
    }
    if (g->ball.x == g->player.x + 1) {
        collisionHandler(&g->player, &g->ball);
    }
    if (g->ball.x == g->comp.x - 1) {
        collisionHandler(&g->comp, &g->ball);
    }
    return;
}
void collisionHandler(struct Gamepiece *paddle, struct Gamepiece *ball) {
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

void updatePaddles(struct Gamestate *g) {
    if (g->player.moveY == 1 && g->player.y > CEILING + 1) {
        g->player.y -= 2;
        g->player.moved = 1;
        g->player.moveY = 0;
    } else if (g->player.moveY == -1 && g->player.y < LINES - 3 - g->player.size) {
        g->player.y += 2;
        g->player.moved = 1;
        g->player.moveY = 0;
    }
    /* current method: randomly decide if comp player moves to align itself with the ball 
     * problem: somewhat jerky motion, and the 'following' pattern is unnatural
     * next method to try: store a 'plan' in the comp struct, maybe even calculate where 
     * ball is going to hit, and randomly offset the calculation so sometimes the 
     * comp is perfect, other times the comp guess wrong, either way the comp can move
     * gracefully to a certain position */
    if (g->newFrameFlag &&
        rand() % 100 > 95 - ((g->ball.x / 4) * g->ball.speedX)) {
        /*track ball */
        if (g->ball.y > g->comp.y + 2 &&
            g->comp.y < LINES - 3 - g->comp.size) {
            g->comp.y++;
            g->comp.moved = 1;
        } else if (g->ball.y < g->comp.y + 2 &&
                   g->comp.y > CEILING + 1) {
            g->comp.y--;
            g->comp.moved = 1;
        }
    }
    moveCheck(&g->player);
    moveCheck(&g->comp);
    return;
}

void moveCheck(struct Gamepiece *g) {
    if (g->moved) {
        g->moved = 0;
        paintPaddle(*g);
    }
    return;
}
