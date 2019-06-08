/* tennis game utilizing ncurses for a text based user interface */
#include "tuitennis.h"

int main() {
    /* curses initialization */
    initscr();                  /* start curses mode */
    curs_set(FALSE);            /* no cursor shown */
    nodelay(stdscr, TRUE);      /* getch will be non-blocking */
    cbreak();                   /* get characters immediately */
    noecho();                   /* do not echo characters to the screen */
    /* end curses initialization */

    struct Gamestate g = {
        .speed = 4,
        .newFrameFlag = 1,
        .gameOver = 1,
        .nextServe = 1,
        .run = 1,
        .interpolateTry = 0
    };

    /* timer init */
    gettimeofday(&g.t0, NULL);

    /* paddles init */
    struct Gamepiece player, comp, ball;
    initPaddle(&player);
    initPaddle(&comp);
    player.direction = 1;
    comp.x = COLS - 1;
    comp.direction = -1;

    initBall(&ball);

    g.ball = ball;
    g.player = player;
    g.comp = comp;

    /* print some static strings to the screen */
    mvaddstr(0, 0, BANNER_STRING);
    mvaddstr(1, 0, "Game Speed: ");
    mvhline(CEILING, 0, '_', COLS);

    /* seed rand for the comp player actions */
    srand(time(NULL));

    /* MAIN LOOP */
    while (g.run) {
        /* catch user input, this is non-blocking */
        if ((g.input = getch()) != ERR) { /* ERR is the default */
            handleInput(&g);
        }

        if (g.gameOver) {
            g.gameOver = 0;
            initGame(&g);
        }
        updateBall(&g);
        updatePaddles(&g);
        updateTime(&g);
    }

    /* exit routine */
    nodelay(stdscr, FALSE); /* allow getch to block */
    mvprintw(LINES / 2, (COLS / 2) - strlen(EXIT_MESSAGE)/2, EXIT_MESSAGE);
    refresh();
    getch();
    endwin();

    exit(0);
}
