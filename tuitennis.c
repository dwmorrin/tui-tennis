#include "tuitennis.h"

void BallCollisionCheck(struct Gamestate *g) {
    if (g->ball.y <= CEILING + 1) {
        g->ball.y = CEILING + 1;
        g->ball.speedY = -g->ball.speedY;
    }
    if (g->ball.y >= LINES - 3) {
        g->ball.y = LINES - 3;
        g->ball.speedY = -g->ball.speedY;
    }
    if (g->ball.x <= 0) {
        PaddleScore(&g->comp);
        g->nextServe = 1;
        g->gameOver = true;
    } else if (g->ball.x >= COLS) {
        PaddleScore(&g->player);
        g->nextServe = -1;
        g->gameOver = true;
    }
    if (g->ball.x == g->player.x + 1) {
        PaddleCollisionHandler(&g->player, &g->ball);
    }
    if (g->ball.x == g->comp.x - 1) {
        PaddleCollisionHandler(&g->comp, &g->ball);
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

void GamestateInit(struct Gamestate *gamestate) {
    gamestate->speed = 4;
    gamestate->newFrameFlag = true;
    gamestate->gameOver = true;
    gamestate->nextServe = 1;
    gamestate->run = true;
    gamestate->interpolateTry = false;
    PaddleInit(&player);
    PaddleInit(&comp);
    player.direction = 1;
    comp.x = COLS - 1;
    comp.direction = -1;
    BallInit(&ball);
    gamestate->ball = ball;
    gamestate->player = player;
    gamestate->comp = comp;
    gettimeofday(&gamestate->t0, NULL);
    /* seed rand for the comp player actions */
    srand(time(NULL));
}

long long TimeGetElapsed(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_usec - t0.tv_usec);
}

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
            SpeedUpdate(g);
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
    GamestateReset(g);
    return;
}

void NcursesInit(void) {
    initscr();             /* start curses mode */
    curs_set(FALSE);       /* no cursor shown */
    nodelay(stdscr, TRUE); /* getch be non-blocking */
    cbreak();              /* get chars immediately */
    noecho();              /* do not echo chars */
    /* print some static strings to the screen */
    mvaddstr(0, 0, BANNER_STRING);
    mvaddstr(1, 0, "Game Speed: ");
    mvhline(CEILING, 0, '_', COLS);
}

void NcursesExit(void) {
    nodelay(stdscr, FALSE); /* allow getch to block */
    mvprintw(LINES / 2, (COLS / 2) - strlen(EXIT_MESSAGE)/2, EXIT_MESSAGE);
    refresh();
    getch();
    endwin();
    exit(EXIT_SUCCESS);
}

void GamestateReset(struct Gamestate *gamestate) {
    gamestate->gameOver = false;
    if (gamestate->nextServe == 1) { /* player's serve */
        gamestate->ball.x = 2;
    } else {
        gamestate->ball.x = COLS - 3;
    }
    gamestate->ball.y = LINES / 2;
    gamestate->ball.speedY = 1;
    gamestate->ball.speedX = DEFAULT_BALL_SPEED * gamestate->nextServe;
    PaddlePaint(gamestate->player);
    PaddlePaint(gamestate->comp);
    SpeedUpdate(gamestate);

    char *readysetgo[] = {"READY", "SET", "GO"};
    for (int i = 0; i < 3; i++) {
        int length = strlen(readysetgo[i]);
        mvprintw(LINES/2, COLS/2 - length/2, "%s", readysetgo[i]);
        refresh();
        sleep(1);
        mvprintw(LINES/2, COLS/2 - length/2, "%s", "     ");
    }

    return;
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

void BallUpdate(struct Gamestate *g) {
    int lookAheadX;
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
    lookAheadX = g->ball.x + g->ball.speedX;
    if (lookAheadX <= g->player.x + 1 &&
        ! g->interpolateTry) {
        g->interpolateTry = true;
        g->ball.x = g->player.x + 1;
    } else if (lookAheadX >= g->comp.x - 1 &&
        ! g->interpolateTry) {
        g->interpolateTry = true;
        g->ball.x = g->comp.x - 1;
    } else {
        g->interpolateTry = false;
        g->ball.x += g->ball.speedX;
    }
    g->ball.y += g->ball.speedY;
    BallCollisionCheck(g);
    mvaddch(g->ball.y, g->ball.x, 'o');
    return;
}

void PaddlesUpdate(struct Gamestate *g) {
    if (g->player.moveY == 1 && g->player.y > CEILING + 1) {
        g->player.y -= 2;
        g->player.moved = true;
        g->player.moveY = 0;
    } else if (g->player.moveY == -1 && g->player.y < LINES - 3 - g->player.size) {
        g->player.y += 2;
        g->player.moved = true;
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
            g->comp.moved = true;
        }
    }
    PaddleMoveCheck(&g->player);
    PaddleMoveCheck(&g->comp);
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

void updateTime(struct Gamestate *g) {
    int cols, lines;
    gettimeofday(&g->t1, NULL);
    if (TimeGetElapsed(g->t0, g->t1) > DELAY_60FPS) {
        g->frame++;
        g->newFrameFlag = true;
        cols = COLS;
        lines = LINES;
        refresh();
        if (COLS != cols || LINES != lines) {
            sleep(2); // wait for resize to end
            handleResize(g, cols, lines);
        }
        gettimeofday(&g->t0, NULL);
    } else {
        g->newFrameFlag = false;
    }
}
