/* tennis game utilizing ncurses for a text based user interface */
#include <time.h>
#include "tuitennis.h"

int main() {
    /* curses initialization */
    initscr();                  /* start curses mode */
    curs_set(FALSE);            /* no cursor shown */
    nodelay(stdscr, TRUE);      /* getch will be non-blocking */
    cbreak();                   /* get characters immediately */
    noecho();                   /* do not echo characters to the screen */
    /* end curses initialization */

    int cols, lines, ch = 0;
    struct Gamestate g;
    g.input = 0;
    g.speed = 4;
    g.frame = 0;
    g.newFrameFlag = 1;
    g.gameOver = 1;

    /* timer init */
    struct timeval t1, t0;
    gettimeofday(&t0, NULL);

    /* paddles init */
    struct Gamepiece paddle, comp, ball;
    paddle.x = 0;
    paddle.y = 5;
    paddle.size = 5;
    paddle.score = 0;
    comp.x = COLS - 1;
    comp.y = 5;
    comp.size = 5;
    comp.score = 0;

    ball.x = 1;
    ball.y = LINES / 2;
    ball.size = 1;
    ball.speedY = 1;
    ball.speedX = 1;

    g.ball = ball;
    g.player = paddle;
    g.comp = comp;

    /* print some static strings to the screen */
    mvaddstr(0, 0, "TEXT USER INTERFACE TENNIS | up: w, down: s, quit: q");
    mvaddstr(1, 0, "Game Speed: ");
    mvhline(CEILING, 0, '_', COLS);

    /* print the paddles */
    paintPaddle(paddle);
    paintPaddle(comp);

    /* seed rand for the comp player actions */
    srand(time(NULL));

    /* MAIN LOOP */
    while(ch != 'q') {
        /* catch user input, this is non-blocking */
        if ((ch = getch()) != ERR) { /* ERR is the default */
            g.input = ch;
        }

        /* ball animation */
        updateBall(&g);

        /* paddle animation */
        updatePaddles(&g);

        /* finally, check the time and paint if time is right
         * and put the new-frame-flag up and down to control 
         * flag-based logic (prevent multiple calls)
         * also check if COLS or LINES change and handle resize
         */
        gettimeofday(&t1, NULL);
        if (getElapsed(t0, t1) > DELAY_60FPS) {
            g.frame++;
            g.newFrameFlag = 1;
            cols = COLS;
            lines = LINES;
            refresh();
            if (COLS != cols || LINES != lines) {
                // handle resize
                break;
            }
            gettimeofday(&t0, NULL);
        } else {
            g.newFrameFlag = 0;
        }
    }

    /* exit routine */
    nodelay(stdscr, FALSE); /* allow getch to block */
    mvprintw(LINES / 2, (COLS / 2) - strlen(EXIT_MESSAGE)/2, EXIT_MESSAGE);
    refresh();
    getch();
    endwin();

    exit(0);
}
