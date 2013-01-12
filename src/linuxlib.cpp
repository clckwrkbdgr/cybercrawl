/* Some replacement routines missing in gcc
   Some of these are inspired by/stolen from the Linux-conio package
   by Mental EXPlotion. Hope you guys don't mind.
   The colour exchange system is perhaps a little overkill, but I wanted
   it to be general to support future changes.. The only thing not
   supported properly is black on black (used by curses for "normal" mode)
   and white on white (used by me for "bright black" (darkgray) on black

   Jan 1998 Svante Gerhard <svante@algonet.se>                          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <stdarg.h>
#include <ctype.h>
#define _LINUXLIB_IMPLEMENTATION
#include "linuxlib.h"
#include "defines.h"

// Globals holding current text/backg. colors
int FG_COL = COLOR_WHITE;
int BG_COL = COLOR_BLACK;


// Translate DOS colors to curses. 128 just means use high intens./bold.
int translatecolor(int col)
{
        switch (col)
        {
               case BLACK        : return COLOR_BLACK;         break;
               case BLUE         : return COLOR_BLUE;          break;
               case GREEN        : return COLOR_GREEN;         break;
               case CYAN         : return COLOR_CYAN;          break;
               case RED          : return COLOR_RED;           break;
               case MAGENTA      : return COLOR_MAGENTA;       break;
               case BROWN        : return COLOR_YELLOW;        break;
               case LIGHTGREY    : return COLOR_WHITE;         break;
               case DARKGREY     : return COLOR_BLACK   + 128; break;
               case LIGHTBLUE    : return COLOR_BLUE    + 128; break;
               case LIGHTGREEN   : return COLOR_GREEN   + 128; break;
               case LIGHTCYAN    : return COLOR_CYAN    + 128; break;
               case LIGHTRED     : return COLOR_RED     + 128; break;
               case LIGHTMAGENTA : return COLOR_MAGENTA + 128; break;
               case YELLOW       : return COLOR_YELLOW  + 128; break;
               case WHITE        : return COLOR_WHITE   + 128; break;
               default: return COLOR_GREEN; break; //mainly for debugging
        }
}



void setupcolorpairs()
{
        int i, j;

        for (i = 0; i < 8; i++)
                for (j = 0; j < 8; j++)
                        if ((i > 0) || (j > 0))
                                init_pair(i * 8 + j, j, i);

        init_pair(63, COLOR_BLACK, COLOR_BLACK);
}


void lincurses_startup()
{
        initscr();
        savetty();
        noecho();
        keypad(stdscr, FALSE);
        cbreak();
        meta(stdscr, TRUE);
        start_color();
        setupcolorpairs();
	scrollok(stdscr, TRUE);
}


void lincurses_shutdown()
{
        resetty();
        endwin();
}

int cprintf (const char *format, ... )
{
   int i;
   char buffer[2048]; // One full screen if no control seq...
   va_list argp;
   va_start(argp,format);
   vsprintf(buffer,format,argp);
   va_end(argp);
   i=addstr(buffer);
   refresh();
   return(i);
}


int putch(int chr)
{
        if (chr == 0)
                chr = ' ';
        return(addch(chr));
}


char getche()
{
        char chr;

        chr = getch();
        addch(chr);
        refresh();
        return(chr);
}


int window(int x1, int y1, int x2, int y2)
{
	x1 = y1 = x2 = y2 = 0; /* Do something to them.. makes gcc happy :) */

        return(refresh());
}


int clrscr()
{
	int retval;

        retval = clear();
        refresh();
	return(retval);
}


void _setcursortype(int curstype)
{
        curs_set(curstype);
}


void textcolor(int col)
{
        int fg, bg;

        FG_COL = col;
        fg = translatecolor(FG_COL);
        bg = translatecolor(BG_COL);
        if (bg & 128)
               bg = bg - 128;
        if ((fg == 128) && (bg == 0)) // Special case: 0/0/bold = darkgray/black!
                attrset(COLOR_PAIR(63) | A_BOLD);
        else
        {
                if (fg & 128)
                {
                        attrset((COLOR_PAIR((fg - 128) + bg * 8)) | A_BOLD);
                }
                else
                        attrset(COLOR_PAIR(fg + bg * 8) | A_NORMAL);
        }
}


void textbackground(int col)
{
        int fg, bg;

        BG_COL = col;
        fg = translatecolor(FG_COL);
        bg = translatecolor(BG_COL);
        if (bg & 128)
               bg = bg - 128;
        if ((fg == 128) && (bg == 0)) // Special case: 0/0/bold = darkgray/black!
                attrset(COLOR_PAIR(63) | A_BOLD);
        else
        {
                if (fg & 128)
                {
                        attrset((COLOR_PAIR((fg - 128) + bg * 8)) | A_BOLD);
                }
                else
                        attrset(COLOR_PAIR(fg + bg * 8) | A_NORMAL);
        }
}


int gotoxy(int x, int y)
{
        return(move(y-1, x-1));
}


int wherex()
{
        int x, y;

        getyx(stdscr, y, x);
	return(x+1);
}


int wherey()
{
        int x, y;

        getyx(stdscr, y, x);
	return(y+1);
}


void delay(int time)
{
	usleep(time * 1000);
}

/*
Note: kbhit now in macro.cc
*/

/* This is Juho Snellman's modified kbhit, to work with macros */
int kbhit()
{
    int i;
	
	nodelay(stdscr, TRUE);
	i = wgetch(stdscr);
	nodelay(stdscr, FALSE);
	if (i == -1)
	  i = 0;
	else
	  ungetch(i);
	
	return(i);
}

/*int kbhit()
{
        int i;

        nodelay(stdscr, TRUE);
        i = getch();
        nodelay(stdscr, FALSE);
        if (i == -1)
              i = 0;
        return(i);
}*/

/*

Now in stuff.cc

// this function is used for systems without gettext/puttext to redraw the
// playing screen after a call to for example inventory.
void redraw_screen(void)
{

char title [40];

strcpy(title, skill_title(best_skill(you, 0, 50, 99), you[0].skills [best_skill(you, 0, 50, 99)], you[0].clas, you[0].xl));
draw_border(BROWN, you[0].your_name, title, you[0].species);

//draw_border(BROWN, you[0].your_name, title);

you[0].hp_ch = 1;
you[0].ep_ch = 1;
you[0].strength_ch = 1;
you[0].intel_ch = 1;
you[0].dex_ch = 1;
you[0].AC_ch = 1;
you[0].evasion_ch = 1;
you[0].gp_ch = 1;
you[0].xp_ch = 1;
you[0].hung_ch = 1;
you[0].burden_ch = 1;

print_stats();

new_level();

viewwindow(1);


}

*/

