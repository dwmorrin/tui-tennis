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

    GamestateInit(&gamestate);

    /* print some static strings to the screen */
    mvaddstr(0, 0, BANNER_STRING);
    mvaddstr(1, 0, "Game Speed: ");
    mvhline(CEILING, 0, '_', COLS);

    /* seed rand for the comp player actions */
    srand(time(NULL));

    /* MAIN LOOP */
    while (gamestate.run) {
        /* catch user input, this is non-blocking */
        if ((gamestate.input = getch()) != ERR) { /* ERR is the default */
            handleInput(&gamestate);
        }

        if (gamestate.gameOver) {
            gamestate.gameOver = false;
            initGame(&gamestate);
        }
        updateBall(&gamestate);
        updatePaddles(&gamestate);
        updateTime(&gamestate);
    }

    /* exit routine */
    nodelay(stdscr, FALSE); /* allow getch to block */
    mvprintw(LINES / 2, (COLS / 2) - strlen(EXIT_MESSAGE)/2, EXIT_MESSAGE);
    refresh();
    getch();
    endwin();

    exit(EXIT_SUCCESS);
}
