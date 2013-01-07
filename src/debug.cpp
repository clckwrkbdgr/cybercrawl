
#include "linuxlib.h"

#include <stdlib.h>
#include <string.h>

#include "direct.h"
#include "externs.h"
#include "enum.h"
#include "itemname.h"
#include "player.h"
#include "shopping.h"
#include "dungeon.h"
#include "stuff.h"
#include "monplace.h"
#include "mstruct.h"
#include "skills.h"
#include "spell.h"

#define DEBUG

/*
Some debugging functions, accessable through keys like %, $, &, ) etc when
a section of code in input() in acr.cc is uncommented.
*/


#ifdef DEBUG
// Debug:
void cast_spec_spell(void)
{

	char specs [3];

	mpr("Cast which spell? ");

	specs [0] = getche();
	specs [1] = getche();
	specs [2] = getche();

	your_spells(atoi(specs), magic_ability(player_mag_abil(), you[0].intel), 0);

}
#endif


#ifdef DEBUG
void create_spec_monster(void)
{

	char specs [3];

	mpr("Create which monster? ");

	specs [0] = getche();
	specs [1] = getche();
	specs [2] = getche();


	create_monster(atoi(specs), 0, 0, you[0].x_pos, you[0].y_pos, MHITNOT, 250);

}

void level_travel(void)
{

	char specs [3];

	mpr("Travel to which level? ");

	specs [0] = getche();
	specs [1] = getche();
	specs [2] = 0;

	you[0].your_level = atoi(specs);

}

// Debug:
void create_spec_object(void)
{

	int class_wanted = 0;
	int type_wanted = 0;

	mpr("Create which item (class then type)? ");

	class_wanted = (getche() - 48) * 10;
	class_wanted += getche() - 48;

	type_wanted = (getche() - 48) * 10;
	type_wanted += getche() - 48;

	itoa(property(class_wanted, type_wanted, 2), st_prn, 10);
	mpr(st_prn);

	int thing_created = items(1, class_wanted, type_wanted, 1, you[0].your_level, 250);

	if (you[0].species != SP_NAGA) mpr("Something appears at your feet!");
	else mpr("Something appears before you!");

	int what_was_there = igrd [you[0].x_pos] [you[0].y_pos];
	mitm.ilink [thing_created] = what_was_there;
	igrd [you[0].x_pos] [you[0].y_pos] = thing_created;
	//mitm.idam [thing_created] = 13;

}

void create_spec_object2(void)
{

	int class_wanted = 0;
	int type_wanted = 0;
	int dam_wanted = 0;

	mpr("Create which item (class, type, then dam)? ");

	class_wanted = (getche() - 48) * 10;
	class_wanted += getche() - 48;

	type_wanted = (getche() - 48) * 10;
	type_wanted += getche() - 48;

	dam_wanted = (getche() - 48) * 10;
	dam_wanted += getche() - 48;

//itoa(property [class_wanted] [type_wanted] [2], st_prn, 10);
//strcpy(info, st_prn);
//mpr(info);

//int thing_created = items(you[0].unique_items, 1, you[0].item_description,
//       grd, class_wanted, type_wanted, 1, 100, 250);

	int thing_created = items(1, class_wanted, type_wanted, 1, you[0].your_level, 250);


	mitm.idam [thing_created] = dam_wanted;

	if (you[0].species != SP_NAGA) mpr("Something appears at your feet!");
	else mpr("Something appears before you!");

	int what_was_there = igrd [you[0].x_pos] [you[0].y_pos];
	mitm.ilink [thing_created] = what_was_there;
	igrd [you[0].x_pos] [you[0].y_pos] = thing_created;
	//mitm.idam [thing_created] = 13;

}


#endif



// STETH can't examine spaces in cardinal directions more than 1 space from you
void stethoscope(int mwh)
{
	struct dist stth [1];
	int i = mwh;
	if(mwh == 250) {
		mpr("Which monster?");
		direction(1, stth);
		if (stth[0].nothing == -1) {
			return;
		}
		int x = you[0].x_pos + stth[0].move_x;
		int y = you[0].y_pos + stth[0].move_y;
		int env_at_pos = env[0].cgrid[x][y];
		if (env_at_pos != CNG) {
 			msg("@1/@2") << env[0].cloud_type [env_at_pos] << env[0].cloud_decay [env_at_pos];
		}
		if (mgrd[x][y] == MNG) {
			msg("@1") << igrd[x][y];
			return;
		}
		i = mgrd[x][y];
	}

	std::string name = monam(menv[i].m_sec, menv[i].m_class, menv[i].m_ench[2], 0);
	msg("@1: @2: cl @3 - @4/@5   @6/@7") << name << i << menv[i].m_class << menv[i].m_hp << menv[i].m_hp_max << menv[i].m_beh << menv[i].m_hit;
	msg("speed: @1/@2") << menv [i].m_speed << menv [i].m_speed_inc;
	msg("sec:") << menv [i].m_sec;
	msg("target: @1, @2") << menv [i].m_targ_1_x << menv [i].m_targ_1_y;
	msg("ench0: @1 ench1: @2 ench2: @3") << menv [i].m_ench [0] << menv [i].m_ench [1] << menv [i].m_ench [2];
	msg("Ghost damage: @1") << ghost.ghs [7];
}


void debug_add_skills(void)
{

	char specs [2];

	mpr("Practice which skill? ");

	specs [0] = getche();
	specs [1] = getche();

	exercise(atoi(specs), 100);

}
