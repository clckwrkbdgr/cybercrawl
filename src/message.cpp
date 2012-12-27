
#include "linuxlib.h"
/* Message File */

#include "externs.h"
#include "enum.h"

#include <string.h>

#include "macro.h"

void more(void);

char scrloc = 1; /* Line of next (previous?) message */

char store_message [30] [200]; /* buffer of old messages */
unsigned char store_count = 0; /* current position in store_message */

void mpr(const char * inf)
{
	you[0].running = 0;

	textcolor(7);

	if (scrloc == 7) {
		gotoxy(2,25);
		_setcursortype(_NORMALCURSOR);
		textcolor(7);
		cprintf("\r--more--");
		char keypress = 0;
		while (keypress != 32 && keypress != 13) {
			keypress = getch();
		}
		gotoxy(1, 18);
		for(int del_line_no = 0; del_line_no < 7; del_line_no ++) {
			cprintf("                                                                               ");
			if (del_line_no != 6) { 
				cprintf(EOL);
			}
		}

		scrloc = 0;
	}

	char info2 [80];
	gotoxy(1, scrloc + 18);
	strncpy(info2, inf, 78);
	info2 [78] = 0;
	cprintf(info2);

	/* Put the message into store_message, and move the '---' line forward */
	strncpy(store_message [store_count], inf, 78);
	store_count ++;
	if (store_count > 23) {
		store_count -= 24;
	}
	strcpy(store_message [store_count], "------------------------------------------------------------------------------");

	scrloc ++;
} // end of message function

void mesclr(void)
{
 _setcursortype(_NOCURSOR);

char del_line_no = 0;
gotoxy(1, 18);
for (del_line_no = 0; del_line_no < 8; del_line_no ++)
{
 cprintf("                                                                               ");
 if (del_line_no != 7) cprintf(EOL);
}
 scrloc = 0;
 _setcursortype(_NORMALCURSOR);
 gotoxy(18,9);
}


void more(void)
{
char keypress = 0;

 gotoxy(2,25);
 _setcursortype(_NORMALCURSOR);
 textcolor(7);
 cprintf("\r--more--");
 do
 {
	keypress = getch();
 } while (keypress != 32 && keypress != 13);

/* Juho Snellman rewrote this part of the function: */
 keypress = 0;
 /* clrscr() should be inside the DOS-define, and the message
  * buffer cleared in a different way for Linux to fix annoying
  * redraw bug whenever the more-prompt showed up. -- jsnell */
 char del_line_no = 0;
 gotoxy(1, 18);
 for (del_line_no = 0; del_line_no < 8; del_line_no ++)
 {
   cprintf("                                                                              ");
  if (del_line_no != 7) cprintf(EOL);
 }
 scrloc = 0;
} 
										 



/*
 keypress = 0;
 clrscr();
 scrloc = 0;

}*/






void replay_messages(void)
{
 _setcursortype(_NOCURSOR);

   int i = 0;
   int j= 0;
   int line = 0;

   clrscr();

   gotoxy(1,1);

   line = 0;

   for (j = 0; j < 24; j ++)
   {
    if (strncmp(store_message [j], "--------------", 10) == 0)
    {
     line = j;
     break;
    }
   }

i = line + 1;
if (i == 24) i = 0;

do
{
 cprintf(store_message [i]);
 cprintf(EOL);
 if (i == line) break;
 i ++;
 if (i == 24) i = 0;
} while (1);

/*
			for (i = 0; i < 24; i ++)
			{
				cprintf(store_message [i]);
				cprintf("\n\r");
			}
*/
   if (getch() == 0) getch();



 _setcursortype(_NORMALCURSOR);

   return;

}

