/* Some replacement routines missing in gcc */

// This defines were in now non-existent files. Moved here as it is most basic and must-have header file.
#define VERSION "3.30"
#define EOL "\n"

#define _NORMALCURSOR 1
#define _NOCURSOR 0
#define O_BINARY O_RDWR

void lincurses_startup();
int cprintf (const char *format, ... );
int putch(int chr);
char getche();
int window(int x1, int y1, int x2, int y2);
int clrscr();
void _setcursortype(int curstype);
void textcolor(int col);
int gotoxy(int x, int y);
int wherex();
int wherey();
void delay(int time);
int kbhit();
void textbackground(int bg);
void lincurses_shutdown();

#ifndef _LINUXLIB_IMPLEMENTATION
/* Some stuff from curses, to remove compiling warnings.. */
extern "C" {
int getstr(char *);
int getch(void);
int noecho(void);
int echo(void);
}
#endif

