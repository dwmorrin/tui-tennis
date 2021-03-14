#include <time.h>
#include <stdlib.h>

#include "Ball.h"
#include "Gamestate.h"
#include "Ncurses.h"
#include "Paddle.h"
#include "Time.h"
#include "input.h"

void GamestatePrintTopLines() {
    mvaddstr(0, 0, BANNER_STRING);
    mvaddstr(1, 0, "Frame delay: ");
    mvhline(CEILING, 0, '_', COLS);
}

void GamestateCollisionCheck(struct Gamestate *g) {
    /* bounce ball off top of screen */
    if (g->ball->y <= CEILING) {
        g->ball->y = CEILING + 1;
        g->ball->speedY = -g->ball->speedY;
    }
    /* bounce ball off bottom of screen */
    if (g->ball->y >= LINES - 2) {
        g->ball->y = LINES - 3;
        g->ball->speedY = -g->ball->speedY;
    }
    /* bounce ball off paddle, or end round */
    if (g->ball->x <= g->player->x) {
        if (! PaddleCollisionHandler(g->player, g->ball)) {
            PaddleScore(g->comp);
            g->nextServe = 1;
            g->gameOver = true;
        }
    }
    if (g->ball->x >= g->comp->x) {
        if (! PaddleCollisionHandler(g->comp, g->ball)) {
            PaddleScore(g->player);
            g->nextServe = -1;
            g->gameOver = true;
        }
    }
}

void GamestateDifficultyUpdate(struct Gamestate *g) {
    if (g->input == '+' && g->difficulty > DIFFICULTY_MIN) {
        g->difficulty--;
    }
    if (g->input == '-' && g->difficulty < DIFFICULTY_MAX) {
        g->difficulty++;
    }
    mvprintw(LINES - 1, COLS / 2, "diff: %d", g->difficulty);
}

void GamestateInit(
    struct Gamestate *gamestate,
    struct Gamepiece *player,
    struct Gamepiece *comp,
    struct Gamepiece *ball
) {
    gamestate->difficulty = 95;
    gamestate->speed = FRAME_DELAY_uS;
    gamestate->newFrameFlag = true;
    gamestate->gameOver = true;
    gamestate->nextServe = 1;
    gamestate->run = true;
    gamestate->countdown = 0;
    player->direction = 1;
    comp->x = COLS - 1;
    comp->direction = -1;
    gamestate->ball = ball;
    gamestate->player = player;
    gamestate->comp = comp;
    gettimeofday(&gamestate->t0, NULL);
    /* seed rand for the comp player actions */
    srand(time(NULL));
}

void GamestateReset(struct Gamestate *gamestate) {
    gamestate->gameOver = false;
    gamestate->countdown = 10;
    if (gamestate->nextServe == 1) { /* player's serve */
        gamestate->ball->x = 2;
    } else {
        gamestate->ball->x = COLS - 3;
    }
    gamestate->ball->y = LINES / 2;
    gamestate->ball->speedY = 0.5;
    gamestate->ball->speedX = DEFAULT_BALL_SPEED * gamestate->nextServe;
    PaddlePaint(gamestate->player);
    PaddlePaint(gamestate->comp);
    GamestateSpeedUpdate(gamestate);
}

void GamestatePrintCountdown(struct Gamestate *gamestate) {
    NcursesPrintCenter("      ");
    // if == 1, just blank and decrement, else print message
    if (gamestate->countdown > 1)
        NcursesPrintCenter(gamestate->countdown > 6
            ? "READY"
            : gamestate->countdown > 3
                ? "SET"
                : "GO"
        );
    --gamestate->countdown;
}

void GamestateOnInput(struct Gamestate *g) {
    switch (g->input) {
        case KEY_RESIZE:
            GamestateOnResize(g);
            break;
        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            PaddleMove(g->player, g->input);
            break;
        case 'j':
        case 'k':
            GamestateSpeedUpdate(g);
            break;
        case QUIT:
            g->run = false;
            break;
        case '+':
        case '-':
            GamestateDifficultyUpdate(g);
            break;
        // for debugging: toggle nodelay to step or run
        case 'r':
            nodelay(stdscr, true);
            break;
        case 'b':
            nodelay(stdscr, false);
            break;
    }
    g->input = ERR;
}

void GamestateOnResize(struct Gamestate *g) {
    nodelay(stdscr, false);
    endwin();
    refresh();
    nodelay(stdscr, false);
    int key = 0;
    do {
        clear();
        NcursesPrintCenter("Done resizing?");
        refresh();
        key = getch();
    } while (key != 'y' && key != 'Y');
    clear();
    nodelay(stdscr, true);
    GamestatePrintTopLines();
    PaddleInit(g->comp);
    PaddleInit(g->player);
    BallInit(g->ball);
    GamestateInit(g, g->player, g->comp, g->ball);
    GamestateReset(g);
}

void GamestateSpeedUpdate(struct Gamestate *g) {
    int step = FRAME_DELAY_uS / 20;
    if (g->input == 'k') g->speed += step;
    else if (g->input == 'j') g->speed -= step;
    mvprintw(1, 13, "%d mS (less delay: j, more delay: k)", g->speed/1000);
}

/*
 * Prints message to 3rd line
 * good for debugging
 */
void GamestatePrintMessage(const char* str) {
    mvprintw(2, 0, str);
}

// true if new flag set
bool GamestateTimeUpdate(struct Gamestate *g) {
    gettimeofday(&g->t1, NULL);
    if (TimeGetElapsed(g->t0, g->t1) > g->speed) {
        g->frame++;
        g->newFrameFlag = true;
        gettimeofday(&g->t0, NULL);
        return true;
    }
    g->newFrameFlag = false;
    return false;
}
