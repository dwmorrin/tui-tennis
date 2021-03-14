#ifndef GAMEPIECE
#define GAMEPIECE
#include <stdbool.h>
struct Gamepiece {
    bool moved;
    double x, y, speedX, speedY;
    int direction, size, moveX, moveY,
        angle, power, score;
};
#endif
