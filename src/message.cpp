#include <list>
#include <string>
#include <string.h>
#include "enum.h"
#include "externs.h"
#include "linuxlib.h"
#include "macro.h"
#include "message.h"

Message msg(const std::string & format_string) {
	return Message(format_string);
}

char scrloc = 1; // Line of next (previous?) message.
std::list<std::string> history;

// Juho Snellman wrote this function.
void mesclr(void)
{
	_setcursortype(_NOCURSOR);
	gotoxy(1, 18);
	for(int del_line_no = 0; del_line_no < 8; del_line_no ++) {
		cprintf("                                                                               ");
		if (del_line_no != 7) {
			cprintf(EOL);
		}
	}
	scrloc = 0;
	_setcursortype(_NORMALCURSOR);
	gotoxy(18,9);
}

void more()
{
	gotoxy(2,25);
	_setcursortype(_NORMALCURSOR);
	textcolor(7);
	cprintf("\r--more--");
	char keypress = 0;
	while (keypress != 32 && keypress != 13) {
		keypress = getch();
	}

	mesclr();
} 

void mpr(const char * inf)
{
	you[0].running = 0;

	textcolor(7);

	if (scrloc == 7) {
		more();
	}

	char info2 [80];
	gotoxy(1, scrloc + 18);
	strncpy(info2, inf, 78);
	info2 [78] = 0;
	cprintf(info2);

	history.push_back(inf);
	while(history.size() > 23) {
		history.pop_front();
	}

	scrloc ++;
}

void replay_messages()
{
	_setcursortype(_NOCURSOR);
	clrscr();
	gotoxy(1,1);

	for(std::list<std::string>::const_iterator line = history.begin(); line != history.end(); ++line) {
		cprintf(line->c_str());
		cprintf(EOL);
	}

	if (getch() == 0) getch();
	_setcursortype(_NORMALCURSOR);
}

