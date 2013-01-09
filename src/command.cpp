
#include "linuxlib.h"

#include <string.h>

#include "externs.h"
#include "enum.h"
#include "ouch.h"
#include "message.h"
#include "stuff.h"
#include "items.h"
#include "itemname.h"
#include "invent.h"
#include "spells0.h"

void adjust_item(void);
void adjust_spells(void);

extern int wield_change;


#ifdef DEBUG

void stethoscope(int mwh)
{
// STETH can't examine spaces in cardinal directions more than 1 space from you

	int i, nothing;

	if (mwh == 250)
	{

		mpr("Which monster?");

		struct dist stmove [1];

		direction(1, stmove);

		if (nothing == -1) return;

		int cell = env[0].cgrid [you[0].x_pos + stmove[0].move_x] [you[0].y_pos + stmove[0].move_y];
		if (cell != CNG)
		{
 			msg("@1/@2") << env[0].cloud_type[cell] << env[0].cloud_decay[cell];
		}

		if (mgrd [you[0].x_pos + stmove[0].move_x] [you[0].y_pos + stmove[0].move_y] == MNG)
		{

			itoa((int) igrd [you[0].x_pos + stmove[0].move_x] [you[0].y_pos + stmove[0].move_y], st_prn, 10);
			mpr(st_prn);

/*	move_x = 0;
	move_y = 0;*/
			return;
		}

		i = mgrd [you[0].x_pos + stmove[0].move_x] [you[0].y_pos + stmove[0].move_y];

	} else i = mwh;

	msg("Ghost damage: ") << ghost.ghs[7];
} // end of stethoscope()


#endif

void quit_game(void)
{

	mpr("Really quit?");

	int keyin = get_ch();

	if (keyin == 'y' || keyin == 'Y')
	{
		ouch(-9999, 0, 13);

	}

} // end of void quit_game




void version(void)
{
 	mpr("This is Cybercrawl v"VERSION". (Last build 26/3/99)");
}


void adjust(void)
{

	mpr("Adjust (i)tems or program(s)?");

	int keyin = get_ch();

	if (keyin == 'i' || keyin == 'I')
	{
 		adjust_item();
 		return;
	}
	if (keyin == 's' || keyin == 'S')
	{
 		adjust_spells();
 		return;
	}

	mpr("Huh?");
	return;

}



void adjust_item(void)
{

	int throw_2, throw_3;
    int nthing = 0;

	if (you[0].inv_no == 0)
	{
		mpr("You aren't carrying anything.");
		return;
	}

	query : mpr("Adjust which item?");

	int keyin = get_ch();

	if (keyin == '?' || keyin == '*')
	{
		if (keyin == '*' || keyin == '?') nthing = get_invent(-1);
		if ((nthing >= 65 && nthing <= 90) || (nthing >= 97 && nthing <= 122))
		{
			keyin = nthing;
		} else
		{
		    mesclr();
	    	goto query;
		}
	}

	int throw_1 = (int) keyin;

	if (throw_1 < 65 || (throw_1 > 90 && throw_1 < 97) || throw_1 > 122)
	{
		mpr("You don't have any such object.");
		return;
	}

	throw_2 = conv_lett(throw_1);

	if (you[0].inv_quant [throw_2] == 0)
	{
		mpr("You don't have any such object.");
		return;
	}

   	char slot = (throw_2 <= 25) ? (throw_2 + 97) :(throw_2 + 39);
	msg("@1 - @2") << slot << in_name(throw_2, 3);
	mpr("Adjust to which letter?");

	keyin = get_ch();

	if (keyin == '?' || keyin == '*')
	{
		if (keyin == '*' || keyin == '?') nthing = get_invent(-1);
		if ((nthing >= 65 && nthing <= 90) || (nthing >= 97 && nthing <= 122))
		{
			keyin = nthing;
		} else
		{
		    mesclr();
	    	goto query;
		}
	}

	throw_1 = (int) keyin;

	if (throw_1 < 65 || (throw_1 > 90 && throw_1 < 97) || throw_1 > 122)
	{
		mpr("What?");
		return;
	}

	throw_3 = conv_lett(throw_1);

	if (you[0].inv_quant [throw_3] != 0)
	{
		mpr("Sorry, that inventory letter is already in use.");
		return;
	}

	you[0].inv_ident [throw_3] = you[0].inv_ident [throw_2];
	you[0].inv_class [throw_3] = you[0].inv_class [throw_2];
	you[0].inv_type [throw_3] = you[0].inv_type [throw_2];
	you[0].inv_plus [throw_3] = you[0].inv_plus [throw_2];
	you[0].inv_plus2 [throw_3] = you[0].inv_plus2 [throw_2];
	you[0].inv_dam [throw_3] = you[0].inv_dam [throw_2];
	you[0].inv_col [throw_3] = you[0].inv_col [throw_2];
	you[0].inv_quant [throw_3] = you[0].inv_quant [throw_2];

	int i = 0;

	for (i = 0; i < 10; i ++)
	{
	 	if (you[0].equip [i] == throw_2) you[0].equip [i] = throw_3;
	}

	you[0].inv_quant [throw_2] = 0;

	slot = (throw_3 <= 25) ? (throw_3 + 97) : (throw_3 + 39);
	msg("@1 - @2") << slot << in_name(throw_3, 3);

	if (throw_3 == you[0].equip [EQ_WEAPON]) wield_change = 1;

}





void adjust_spells(void)
{

	int throw_2, throw_3;
    int nthing = 0;

	if (you[0].spell_no == 0)
	{
		mpr("You don't have any program installed.");
		return;
	}

	query : mpr("Adjust which program?");

	int keyin = get_ch();

	if (keyin == '?' || keyin == '*')
	{
		if (keyin == '*' || keyin == '?') nthing = spell_list();
		if ((nthing >= 65 && nthing <= 90) || (nthing >= 97 && nthing <= 122))
		{
			keyin = nthing;
		} else
		{
		    mesclr();
		    goto query;
		}
	}

	int throw_1 = (int) keyin;

	if (throw_1 < 97 || throw_1 > 119)
	{
		mpr("You don't have that program.");
		return;
	}

	throw_2 = conv_lett(throw_1);

	if (you[0].spells [throw_2] == 210)
	{
		mpr("You don't have that program.");
		return;
	}

	spell_name(you[0].spells [throw_2], str_pass);
	msg("@1 - @2") << char(throw_2 + 97) << str_pass;

	mpr("Adjust to which letter?");

	keyin = get_ch();

	if (keyin == '?' || keyin == '*')
	{
		if (keyin == '*' || keyin == '?') nthing = spell_list();
		if ((nthing >= 65 && nthing <= 90) || (nthing >= 97 && nthing <= 122))
		{
			keyin = nthing;
		} else
		{
		    mesclr();
	    	goto query;
		}
	}

	throw_1 = (int) keyin;

	//if (throw_1 < 97 || throw_1 > 122)
	if (throw_1 < 97 || throw_1 > 118)
	{
		mpr("What?");
		return;
	}

	throw_3 = conv_lett(throw_1);

	int backup = you[0].spells [throw_3];
	you[0].spells [throw_3] = you[0].spells [throw_2];
	you[0].spells [throw_2] = backup;

	spell_name(you[0].spells [throw_3], str_pass);
	msg("@1 - @2") << char(throw_3 + 97) << str_pass;

	if (you[0].spells [throw_2] != 210)
	{
		spell_name(you[0].spells [throw_2], str_pass);
		msg("@1 - @2") << char(throw_2 + 97) << str_pass;
	}

}

