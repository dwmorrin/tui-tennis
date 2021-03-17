#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "Tui.h"

void TuiExit(void) {
    nodelay(stdscr, FALSE); /* allow getch to block */
    mvprintw(LINES / 2, (COLS / 2) - strlen(EXIT_MESSAGE)/2, EXIT_MESSAGE);
    refresh();
    getch();
    endwin();
    exit(EXIT_SUCCESS);
}

void TuiInit(void) {
    initscr();             /* start curses mode */
    curs_set(FALSE);       /* no cursor shown */
    nodelay(stdscr, TRUE); /* getch be non-blocking */
    cbreak();              /* get chars immediately */
    noecho();              /* do not echo chars */
    /* print some static strings to the screen */
}

void TuiPrintCenter(const char* message) {
    mvprintw(LINES/2, COLS/2 - strlen(message)/2, "%s", message);
}

