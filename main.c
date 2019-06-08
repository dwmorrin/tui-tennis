/* tennis game utilizing ncurses for a text based user interface */
#include "tuitennis.h"

int main() {
    NcursesInit();
    GamestateInit(&gamestate);
    while (gamestate.run) {
        /* catch user input, this is non-blocking */
        if ((gamestate.input = getch()) != ERR) { /* ERR is the default */
            handleInput(&gamestate);
        }

        if (gamestate.gameOver) {
            gamestate.gameOver = false;
            GameReset(&gamestate);
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
