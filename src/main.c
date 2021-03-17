/* tennis game utilizing ncurses for a text based user interface */
#include <ncurses.h>

#include "Ball.h"
#include "Gamepiece.h"
#include "Gamestate.h"
#include "Tui.h"
#include "Paddle.h"

struct Gamestate game;
struct Gamepiece ball;
struct Gamepiece paddleRight;
struct Gamepiece paddleLeft;

#ifdef DEBUG
char debug[80];
#endif

int main() {
    TuiInit();
    GamestatePrintTopLines();
    PaddleInit(&paddleLeft);
    PaddlePaint(&paddleLeft);
    PaddleInit(&paddleRight);
    PaddlePaint(&paddleRight);
    BallInit(&ball);
    GamestateInit(&game, &paddleLeft, &paddleRight, &ball);
    while (game.run) {
        /* catch user input, this is non-blocking */
        if ((game.input = getch()) != ERR) GamestateOnInput(&game);
        if (game.countdown) {
            if (game.newFrameFlag) GamestatePrintCountdown(&game);
        } else if (game.newFrameFlag) {
            if (GamestateComputerWillMove(&game))
                PaddleAiMove(&paddleRight, &ball);
            if (GamestatePlayerWillMove(&game)) {
                PaddleMove(&paddleLeft, game.move);
                game.move = ERR;
            }
            BallMove(&ball); // ball move
            GamestateCollisionCheck(&game);
            BallPaint(&ball);
            if (game.gameOver) {
                GamestateReset(&game);
                continue;
            }
#ifdef DEBUG
            sprintf(debug, "ball: (%3g,%3g), (%1.1g,%1.1g)", ball.x,ball.y,ball.speedX,ball.speedY);
            GamestatePrintMessage(debug);
#endif
        }
        if (GamestateTimeUpdate(&game)) refresh();
    }
    TuiExit();
}
