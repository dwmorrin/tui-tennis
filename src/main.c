/* tennis game utilizing ncurses for a text based user interface */
#include "tuitennis.h"

struct Gamestate game;
struct Gamepiece ball;
struct Gamepiece comp;
struct Gamepiece player;

int main() {
    NcursesInit();
    GamestatePrintTopLines();
    PaddleInit(&player);
    PaddleInit(&comp);
    BallInit(&ball);
    GamestateInit(&game, &player, &comp, &ball);
    while (game.run) {
        /* catch user input, this is non-blocking */
        if ((game.input = getch()) != ERR) { /* ERR is the default */
            GamestateOnInput(&game); // player move
        }
        if (game.countdown) {
            if (game.newFrameFlag) GamestatePrintCountdown(&game);
        } else {
            PaddleAiMove(&game); // comp move
            BallUpdate(&game); // ball move
            GamestateCollisionCheck(&game);
            if (game.gameOver) {
                GamestateReset(&game);
                continue;
            }
            BallPaint(&ball);
        }
        if (TimeUpdate(&game)) {
            // TODO handle screen resize event
            refresh();
        }
    }
    NcursesExit();
}
