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
            GamestateReset(&gamestate);
        }

        updateBall(&gamestate);
        updatePaddles(&gamestate);
        updateTime(&gamestate);
    }
    NcursesExit();
}
