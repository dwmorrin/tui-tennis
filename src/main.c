/* tennis game utilizing ncurses for a text based user interface */
#include "tuitennis.h"

struct Gamestate game;
struct Gamepiece ball;
struct Gamepiece comp;
struct Gamepiece player;

#ifdef DEBUG
char debug[80];
#endif

int main() {
    NcursesInit();
    GamestatePrintTopLines();
    PaddleInit(&player);
    PaddleInit(&comp);
    BallInit(&ball);
    GamestateInit(&game, &player, &comp, &ball);
    while (game.run) {
        /* catch user input, this is non-blocking */
        if ((game.input = getch()) != ERR) GamestateOnInput(&game);
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
#ifdef DEBUG
            sprintf(debug, "ball: (%g,%g), (%g,%g)", ball.x,ball.y,ball.speedX,ball.speedY);
            GamestatePrintMessage(debug);
#endif
        }
        if (TimeUpdate(&game)) refresh();
    }
    NcursesExit();
}
