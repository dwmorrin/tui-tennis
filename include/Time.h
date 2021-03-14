#ifndef GAME_TIME
#define GAME_TIME

#include <stdbool.h>
#include <sys/time.h>

long long TimeGetElapsed(struct timeval t0, struct timeval t1);

#endif
