#include "tuitennis.h"

long long TimeGetElapsed(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_usec - t0.tv_usec);
}

// true if new flag set
bool TimeUpdate(struct Gamestate *g) {
    gettimeofday(&g->t1, NULL);
    if (TimeGetElapsed(g->t0, g->t1) > g->speed) {
        g->frame++;
        g->newFrameFlag = true;
        gettimeofday(&g->t0, NULL);
        return true;
    }
    g->newFrameFlag = false;
    return false;
}
