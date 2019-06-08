#include "tuitennis.h"

long long TimeGetElapsed(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_usec - t0.tv_usec);
}

void TimeUpdate(struct Gamestate *g) {
    int cols, lines;
    gettimeofday(&g->t1, NULL);
    if (TimeGetElapsed(g->t0, g->t1) > DELAY_60FPS) {
        g->frame++;
        g->newFrameFlag = true;
        cols = COLS;
        lines = LINES;
        refresh();
        if (COLS != cols || LINES != lines) {
            sleep(2); // wait for resize to end
            handleResize(g, cols, lines);
        }
        gettimeofday(&g->t0, NULL);
    } else {
        g->newFrameFlag = false;
    }
}
