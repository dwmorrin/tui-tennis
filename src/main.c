/* tennis game utilizing ncurses for a text based user interface */
#include "tuitennis.h"

int main() {
    NcursesInit();
    struct Gamestate *game = &gamestate;
    GamestateInit(game);
    while (game->run) {
        /* catch user input, this is non-blocking */
        if ((game->input = getch()) != ERR) { /* ERR is the default */
            handleInput(game); // player move
        }
        PaddleAiMove(game); // comp move
        BallUpdate(game); // ball move
        GamestateCollisionCheck(game);
        if (game->gameOver) {
            GamestateReset(game);
            continue;
        }
        BallPaint(game->ball);
        TimeUpdate(game);
    }
    NcursesExit();
}
