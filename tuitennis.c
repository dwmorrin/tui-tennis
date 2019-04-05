#include "tuitennis.h"

long long getElapsed(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_usec - t0.tv_usec);
}

void paintPaddle(struct Gamepiece paddle) {
    move(CEILING, paddle.x);
    vline(' ', LINES - CEILING - 2);
    move(paddle.y, paddle.x);
    vline('|', paddle.size);
    return;
}

void initBall(struct Gamestate *g, int direction) {
    char *readysetgo[] = {"READY", "SET", "GO"};
    int i, l;
    if (direction == 1) { /* player's serve */
        g->ball.x = 2;
    } else {
        g->ball.x = g->cols - 3;
    }
    g->ball.y = g->lines / 2;
    g->ball.speedY = 1;
    g->ball.speedX = 1 * direction;

    for (i = 0; i < 3; i++) {
        l = strlen(readysetgo[i]);
        mvprintw(g->lines/2, g->cols/2 - l/2, "%s", readysetgo[i]);
        refresh();
        sleep(1);
        mvprintw(g->lines/2, g->cols/2 - l/2, "%s", "          ");
    } // TODO something better for blanking the line

    return;
}

void updateBall(struct Gamestate *g) {
    if (g->newFrameFlag && g->frame % g->speed == 0) {
        if (g->input == 'k' && g->speed > 1) {
            g->speed--;
            g->input = ERR;
        } else if (g->input == 'j' && g->speed < 10) {
            g->speed++;
            g->input = ERR;
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
        /* check for collisions */
        if (g->ball.y <= CEILING + 1 || g->ball.y >= LINES - 3) {
            g->ball.speedY = -g->ball.speedY;
        }
        if (g->ball.x == 0) { // || ball.x == COLS) {
            g->comp.score++;
            mvprintw(LINES - 1, COLS - 3, "%d", g->comp.score);
            initBall(g, 1);
            g->gameOver = 1;
        } else if (g->ball.x == COLS) {
            g->player.score++;
            mvprintw(LINES - 1, 0, "%d", g->player.score);
            initBall(g, -1);
            g->gameOver = 1;
        }
        if (g->ball.x == 1) {
           if (g->ball.y >= g->player.y && g->ball.y <= g->player.y + g->player.size) {
              g->ball.speedX = -g->ball.speedX;
              if (g->ball.y == g->player.y) {
                  g->ball.speedY = -2;
              } else if (g->ball.y == g->player.y + 1) {
                  g->ball.speedY = -1;
              } else if (g->ball.y == g->player.y + 2) {
                  g->ball.speedY = 0;
              } else if (g->ball.y == g->player.y + 3) {
                  g->ball.speedY = 1;
              } else if (g->ball.y == g->player.y + 4) {
                  g->ball.speedY = 2;
              }
           } 
        }
        if (g->ball.x == COLS - 2) {
           if (g->ball.y >= g->comp.y && g->ball.y <= g->comp.y + g->comp.size) {
              g->ball.speedX = -g->ball.speedX;
           } 
        }
        mvaddch(g->ball.y, g->ball.x, 'o');
        /* update current speed */
        mvprintw(1, 13, "%d  (slower: j, faster: k)", 11 - g->speed);
    }
    return;
}

void updatePaddles(struct Gamestate *g) {
    if (g->input == 'w' && g->player.y > CEILING + 1) {
        g->input = ERR;
        g->player.y -= 2;
        g->player.moved = 1;
    } else if (g->input == 's' && g->player.y < LINES - 3 - g->player.size) {
        g->input = ERR;
        g->player.y += 2;
        g->player.moved = 1;
    }
    /* current method: randomly decide if comp player moves to align itself with the ball 
     * problem: somewhat jerky motion, and the 'following' pattern is unnatural
     * next method to try: store a 'plan' in the comp struct, maybe even calculate where 
     * ball is going to hit, and randomly offset the calculation so sometimes the 
     * comp is perfect, other times the comp guess wrong, either way the comp can move
     * gracefully to a certain position */
    if (g->newFrameFlag && rand() % 100 > 95 - ((g->ball.x / 4) * g->ball.speedX)) {
        /*track ball */
        if (g->ball.y > g->comp.y + 2 && g->comp.y < LINES - 3 - g->comp.size) {
            g->comp.y++;
            g->comp.moved = 1;
        } else if (g->ball.y < g->comp.y + 2 && g->comp.y > CEILING + 1) {
            g->comp.y--;
            g->comp.moved = 1;
        }
    }
    if (g->player.moved) {
        g->player.moved = 0;
        paintPaddle(g->player);
    }
    if (g->comp.moved) {
        g->comp.moved = 0;
        paintPaddle(g->comp);
    }
    return;
}
