#include "invent.h"

#include "linuxlib.h"

#include "defines.h"
#include "externs.h"
#include "enum.h"

#include <stdlib.h>
#include <string.h>

#include "itemname.h"
#include "macro.h"

//#include "crawlfnc.h"
#include "shopping.h"
#include "stuff.h"

std::string command_string(int i);

int get_invent(int invent_type)
{
	int nothing = invent(invent_type, 0);
	redraw_screen();
	return nothing;

}

int invent(int item_class_inv, int show_price)
{
	int i;
	int j;
	int lines = 0;
	int anything = 0;
	int temp_id [4] [50];
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 50; j ++) {
			temp_id [i] [j] = 1;
		}
	}

	clrscr();
	int Inv_class2 [15];
	int inv_count = 0;
	int ki = 0;
	for (i = 0; i < 15; i++) {
		Inv_class2 [i] = 0;
	}
	for (i = 0; i < 52; i++) {
		if (you[0].inv_quant [i] != 0) {
			Inv_class2 [you[0].inv_class [i]] ++; // adds up number of each class in invent.
			inv_count++;
		}
	}

	if (inv_count == 0) {
		cprintf("You aren't carrying anything.");
		if (getkey() == 0) getkey();
		goto putty;
	}

	if (item_class_inv != -1) {
		for (i = 0; i < 15; i++) {
			if (item_class_inv == 1 && i == 0) i++;
			if (item_class_inv == 0 && i == 11) i++;
			if (item_class_inv == 6 && i == 10) i++;
			if (item_class_inv != i) Inv_class2 [i] = 0;
		}
	}

	if ((item_class_inv == -1 && inv_count > 0) || (item_class_inv != -1 && Inv_class2 [item_class_inv] > 0) || (item_class_inv == 1 && (Inv_class2 [0] > 0 || Inv_class2 [1] > 0)) || (item_class_inv == 0 && (Inv_class2 [0] > 0 || Inv_class2 [11] > 0)) || (item_class_inv == 0 && (Inv_class2 [0] > 0 || Inv_class2 [13] > 0)) || (item_class_inv == 6 && (Inv_class2 [6] > 0 || Inv_class2 [10] > 0)))// || (item_class_inv == 3 && (Inv_class2 [3] > 0 || Inv_class2 [11] > 0)))
	{
		cprintf("  Inventory");
		lines++;
		for (i = 0; i < 15; i ++) {
			if (Inv_class2 [i] != 0) {
				if (lines > 22) {
					gotoxy(1,25);
					cprintf("-more-");
					ki = getkey();
					if (ki == 27) {
						return 27;
					}
					if (ki >= 65 && ki < 123) {
						return ki;
					}
					if (ki == 0) ki = getkey();
					lines = 0;
					clrscr();
					gotoxy(1,1);
					anything = 0;
				}
				if (lines > 0) cprintf(EOL" ");
				textcolor(BLUE);
				switch(i) {
					case 0: cprintf("Hand weapons"); break;
					case 1: cprintf("Missiles"); break;
					case 2: cprintf("Armour"); break;
					case 3: cprintf("Technological devices"); break;
					case 4: cprintf("Comestibles"); break;
					case 5: cprintf("Program archives"); break;
					case 6: cprintf("Single-use devices"); break;
					case 7: cprintf("Implants and chips"); break;
					case 8: cprintf("Vials"); break;
					case 9: cprintf("Gems"); break;
					case 10: cprintf("Program archives"); break;
					case 11: cprintf("ROM devices"); break;
					case 12: cprintf("Discs of Power"); break;
					case 13: cprintf("Miscellaneous"); break;
					case 14: cprintf("Carrion"); break;
				}
				textcolor(LIGHTGREY);
				lines++;

				for (j = 0; j < 52; j++) {
					if (lines > 23 && inv_count > 0) {
						gotoxy(1,25);
						cprintf("-more-");
						ki = getkey();
						if (ki == 27) {
							return 27;
						}
						if (ki >= 65 && ki < 123) {
							return ki;
						}
						if (ki == 0) ki = getkey();
						lines = 0;
						clrscr();
						gotoxy(1,1);
						anything = 0;
					}
					if (you[0].inv_class [j] == i && you[0].inv_quant [j] > 0) {
						anything ++;
						if(lines > 0) {
							cprintf(EOL);
						}
						lines++;
						cprintf(" ");
						cprintf(" ");
						cprintf("%c", (j < 26) ? (j + 97) : (j  + 65 - 26));
						cprintf(" - ");

						int yps = wherey();

						cprintf(in_name(j, 3).c_str());

						inv_count --;

						if (j == you[0].equip [EQ_WEAPON]) cprintf(" (weapon)");
						if (j == you[0].equip [EQ_CLOAK] || j == you[0].equip [EQ_HELMET] || j == you[0].equip [EQ_GLOVES] || j == you[0].equip [EQ_BOOTS] || j == you[0].equip [EQ_SHIELD] || j == you[0].equip [EQ_BODY_ARMOUR]) cprintf(" (worn)");
						if (j == you[0].equip [EQ_LEFT_RING]) cprintf(" (left hemispehere)");
						if (j == you[0].equip [EQ_RIGHT_RING]) cprintf(" (right hemispehere)");
						if (j == you[0].equip [EQ_AMULET]) cprintf(" (vertebral)");

						if (show_price == 1) {
							Format format(" (@1gp)");
							format << item_value(you[0].inv_class [j], you[0].inv_type [j], you[0].inv_dam [j], you[0].inv_plus [j], you[0].inv_plus2 [j], you[0].inv_quant [j], 3, temp_id);
							cprintf(format.str().c_str());
						}
						if (wherey() != yps) lines++;
					}
				} // end of j loop
			} // end of if inv_class2
		} // end of i loop.
	} else {
		if (item_class_inv == -1) {
			cprintf("You aren't carrying anything.");
		} else {
			if(item_class_inv == 0) {
				cprintf("You aren't carrying any weapons.");
			} else {
				if (item_class_inv == 1) {
					cprintf("You aren't carrying any ammunition.");
				} else {
					cprintf("You aren't carrying any such object.");
				}
			}
			anything++;
		}
	}

	if (anything > 0) {
		ki = getkey();
		if (ki >= 65 && ki < 123) {
			return ki;
		}
		if (ki == 0) ki = getkey();
		return anything;
	}

putty :
	return ki;
}

void list_commands(void)
{

	std::string st_pass;

   int i;
   int j = 0;
   clrscr();

   for (i = 0; i < 5000; i ++)
   {
    st_pass = command_string(i);
    if (st_pass.size() < 5)
    {
//     i--;
     continue;
    }
    if (j % 2 == 0)
    {
     gotoxy(2, j / 2 + 1);
     cprintf(st_pass.c_str());
     j++;
     continue;
    }
    gotoxy(40, j / 2 + 1);
    cprintf(st_pass.c_str());
    j ++;
   }
//   j ++;
   getkey();

//   cprintf("xxxxxxxxxxxxx");
//   last_requested = 0;
return;
}

std::string command_string(int i)
{
	switch(i) {
		case 10: return "a - use special augmentation";
		case 20: return "d(#) - drop (#) item(s)";
		case 30: return "e - eat";
		case 40: return "f - fire an appropriate thing";
		case 50: return "i - inventory";
		case 55: return "m - check skills";
		case 60: return "o/c - open/close a door";
		case 65: return "p - communicate";
		case 70: return "q - quaff a vial";
		case 80: return "r - execute a device or an archive";
		case 90: return "s/./del - wait";
		case 100: return "t - throw or shoot something";
		case 110: return "v - get version number";
		case 120: return "w - wield a weapon";
		case 130: return "x - look around you";
		case 135: return "z - shoot a gun";
		case 140: return "A - check mutations";
		case 141: return "C - check experience";
		case 142: return "D - butcher (dissect) a corpse";
		case 145: return "I - Invoke power of wielded item";
		case 150: return "M - Install a program";
		case 160: return "P/R - Put on/Remove an implant";
		case 165: return "Q - commit suicide (quit)";
		case 180: return "S - Save your game and quit";
		case 209: return "V - Examine an identified item";
		case 200: return "W/T - Wear/Take off armour";
		case 210: return "X - show map of level";
		case 220: return "Z - execute a program";
		case 240: return ", or g - pick something up";
		case 250: return "<> - go up/down a staircase";
		case 270: return "; - see what items you're standing on";
		case 280: return "\\ - check your item knowledge";
		case 300: return "Ctrl-P - see old messages";
		case 310: return "Shift & direction - long walk";
		case 320: return "Shift & 5 on keypad - rest 100 turns";
		case 340: return "+ & - on map screen - fast scroll";
		case 350: return "! - shout/command allies";
		case 360: return "Ctrl & dir - door; untrap; attack";
		case 365: return "^ - describe your employment";
		case 367: return "@ - character status";
		case 370: return "# - dump character to file <name>.txt";
		case 380: return "= - adjust inventory/program letters";
		case 390: return "\' - wield item a, or switch to b";
		case 400: return "` - add macro";
		case 410: return "~ - save macros";
		default: return "";
	}
}

