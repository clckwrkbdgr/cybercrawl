#include <sstream>
#include <iomanip>
#include <string.h>

#include "linuxlib.h"

#include <stdlib.h>

#include "externs.h"
#include "enum.h"
#include "itemname.h"
#include "player.h"
#include "ouch.h"
#include "stuff.h"

int wield_change;

void print_stats(void)
{
	textcolor(7);

	if (you[0].hp_ch == 1)
	{
		if (you[0].hp <= you[0].hp_max / 4) textcolor(YELLOW);
		if (you[0].hp <= you[0].hp_max / 10) textcolor(RED);
		gotoxy (44,3);
		cprintf(to_string(you[0].hp).c_str());
		textcolor(LIGHTGREY);
		Format format("(@1)    ");
		format << you[0].hp_max;
		cprintf(format.str().c_str());
		you[0].hp_ch = 0;
	}

	if (you[0].ep_ch == 1)
	{
		gotoxy (47,4);
		Format format("@1(@2)   ");
		format << you[0].ep << you[0].ep_max;
		cprintf(format.str().c_str());
		you[0].ep_ch = 0;
	}

	if (you[0].strength_ch == 1)
	{
		if (you[0].strength <= 0) you[0].strength = 0;
		if (you[0].strength >= 72) you[0].strength = 72;
		if (you[0].max_strength >= 72) you[0].max_strength = 72;
		gotoxy (45,7);
		if (you[0].strength < you[0].max_strength) textcolor(YELLOW);
		cprintf(to_string(you[0].strength).c_str());
		if (you[0].strength != you[0].max_strength)
		{
			textcolor(LIGHTGREY);
			Format format("(@1)    ");
			format << you[0].max_strength;
			cprintf(format.str().c_str());
		} else cprintf("      ");
		you[0].strength_ch = 0;
		if (you[0].strength <= 0) ouch(-9999, 0, 8);
		burden_change();
	}

	if (you[0].intel_ch == 1)
	{
		if (you[0].intel <= 0) you[0].intel = 0;
		if (you[0].intel >= 72) you[0].intel = 72;
		if (you[0].max_intel >= 72) you[0].max_intel = 72;
		gotoxy (45,8);
		if (you[0].intel < you[0].max_intel) textcolor(YELLOW);
		cprintf(to_string(you[0].intel).c_str());
		if (you[0].intel != you[0].max_intel)
		{
			textcolor(LIGHTGREY);
			Format format("(@1)    ");
			format << you[0].max_intel;
			cprintf(format.str().c_str());
		} else cprintf("      ");
		you[0].intel_ch = 0;
		if (you[0].intel <= 0) ouch(-9999, 0, 7);
	}

	if (you[0].dex_ch == 1)
	{
		if (you[0].dex <= 0) you[0].dex = 0;
		if (you[0].dex >= 72) you[0].dex = 72;
		if (you[0].max_dex >= 72) you[0].max_dex = 72;
		gotoxy (45,9);
		if (you[0].dex < you[0].max_dex) textcolor(YELLOW);
		cprintf(to_string(you[0].dex).c_str());
		if (you[0].dex != you[0].max_dex)
		{
			textcolor(LIGHTGREY);
			Format format("(@1)    ");
			format << you[0].max_dex;
			cprintf(format.str().c_str());
		} else cprintf("      ");
		you[0].dex_ch = 0;
		if (you[0].dex <= 0) ouch(-9999, 0, 9);
		textcolor(LIGHTGREY);
	}

	if (you[0].AC_ch == 1)
	{
		gotoxy (44,5);
		cprintf(to_string(player_AC(), 3).c_str());
		cprintf("(");
		cprintf(to_string(player_shield_class()).c_str());
		cprintf(")   ");
		you[0].AC_ch = 0;
	}

	if (you[0].evasion_ch == 1)
	{
		gotoxy (44,6);
		cprintf(to_string(player_evasion()).c_str());
		cprintf("  ");
		you[0].evasion_ch = 0;
	}

	if (you[0].gp_ch == 1)
	{
		gotoxy (46,10);
		cprintf(to_string(you[0].gp).c_str());
		cprintf("    ");
		you[0].gp_ch = 0;
	}

	if (you[0].xp_ch == 1)
	{
		gotoxy (52,11);
		Format format("@1/@2");
		format << you[0].xl << you[0].xp;
		cprintf(format.str().c_str());
		cprintf("      ");
		you[0].xp_ch = 0;
	}

	if (you[0].hung_ch == 1)
	{
		gotoxy(40,14);
		switch(you[0].hung_state)
		{
			case 5: textcolor(BLUE); cprintf("Engorged"); textcolor(LIGHTGREY); break;
			case 4: textcolor(GREEN); cprintf("Full    "); textcolor(LIGHTGREY); break;
			case 3: cprintf("        "); break;
			case 2: textcolor(YELLOW); cprintf("Hungry  "); textcolor(LIGHTGREY); break;
			case 1: textcolor(RED); cprintf("Starving"); textcolor(LIGHTGREY); break;
		}
		you[0].hung_ch = 0;
	}

	if (you[0].burden_ch == 1)
	{
		gotoxy(40,15);
		switch(you[0].burden_state)
		{
			case 5: textcolor(YELLOW); cprintf("Overloaded"); textcolor(LIGHTGREY); break;
			case 2: textcolor(LIGHTRED); cprintf("Encumbered"); textcolor(LIGHTGREY); break;
			case 0: cprintf("          "); break;
		}
		you[0].burden_ch = 0;
	}
	if (wield_change == 1)
	{
		gotoxy(40,13);
		cprintf("                                       ");
		if (you[0].equip [EQ_WEAPON] != -1)
		{
			gotoxy(40,13);
			textcolor(you[0].inv_col [you[0].equip [EQ_WEAPON]]);
			if (you[0].equip [EQ_WEAPON] <= 25) putch(you[0].equip [EQ_WEAPON] + 97);
			else putch(you[0].equip [EQ_WEAPON] + 39);
			cprintf(" - ");
			cprintf(substring(in_name(you[0].equip [EQ_WEAPON], 3), 34).c_str());
			textcolor(LIGHTGREY);
		} else
		{
			gotoxy(40,13);
			if (you[0].attribute [ATTR_TRANSFORMATION] == TRAN_BLADE_HANDS)
			{
				textcolor(RED);
				cprintf("Blade Hands");
				textcolor(LIGHTGREY);
			} else
			{
				textcolor(LIGHTGREY);
				cprintf("Empty hands");
			}
		}
		wield_change = 0;
	}

}


