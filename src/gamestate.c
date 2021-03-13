#include "tuitennis.h"

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
    gamestate->speed = 4;
    gamestate->newFrameFlag = true;
    gamestate->gameOver = true;
    gamestate->nextServe = 1;
    gamestate->run = true;
    gamestate->interpolateTry = false;
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
    gamestate->countdown = 300;
    if (gamestate->nextServe == 1) { /* player's serve */
        gamestate->ball->x = 2;
    } else {
        gamestate->ball->x = COLS - 3;
    }
    gamestate->ball->y = LINES / 2;
    gamestate->ball->speedY = 1;
    gamestate->ball->speedX = DEFAULT_BALL_SPEED * gamestate->nextServe;
    PaddlePaint(gamestate->player);
    PaddlePaint(gamestate->comp);
    GamestateSpeedUpdate(gamestate);
}

void printCenter(const char* message) {
    mvprintw(LINES/2, COLS/2 - strlen(message)/2, "%s", message);
}

void GamestatePrintCountdown(struct Gamestate *gamestate) {
    printCenter("      ");
    // if == 1, just blank and decrement, else print message
    if (gamestate->countdown > 1)
        printCenter(gamestate->countdown > 200
            ? "READY"
            : gamestate->countdown > 100
                ? "SET"
                : "GO"
        );
    --gamestate->countdown;
}

void GamestateOnInput(struct Gamestate *g) {
    switch (g->input) {
        case 'w':
        case 's':
        case 'a':
        case 'd':
            PaddleMove(g->player, g->input);
            break;
        case 'j':
        case 'k':
            GamestateSpeedUpdate(g);
            break;
        case 'q':
            g->run = 0;
            break;
        case '+':
        case '-':
            GamestateDifficultyUpdate(g);
            break;
    }
    g->input = ERR;
}

void GamestateOnResize(struct Gamestate *g, int oldCols, int oldLines) {
    // TODO need to blank and redraw screen, possibly refactor main.c
    // into an init function we can call here
    GamestateReset(g);
}

void GamestateSpeedUpdate(struct Gamestate *g) {
    if (g->input == 'k' && g->speed > 1) {
        g->speed--;
    } else if (g->input == 'j' && g->speed < 10) {
        g->speed++;
    }
    mvprintw(1, 13, "%d  (slower: j, faster: k)", 11 - g->speed);
}

/*
 * Prints message to 3rd line
 * good for debugging
 */
void GamestatePrintMessage(const char* str) {
    mvprintw(2, 0, str);
}
