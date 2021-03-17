#ifndef TUI_NCURSES
#define TUI_NCURSES

#define CEILING 3
#define EXIT_MESSAGE "DONE press any key"

void TuiInit(void);
void TuiExit(void);
void TuiPrintCenter(const char*);

#endif
