#ifndef NCURSES_HELP
#define NCURSES_HELP

#include <ncurses.h>

#define CEILING 3
#define EXIT_MESSAGE "DONE press any key"

void NcursesInit(void);
void NcursesExit(void);
void NcursesPrintCenter(const char*);
#endif
