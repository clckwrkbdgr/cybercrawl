
#include "linuxlib.h"

#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
//#include <fstream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "externs.h"
#include "enum.h"

#include "chardump.h"
#include "view.h"
#include "invent.h"
#include "shopping.h"
#include "itemname.h"
#include "macro.h"
#include "mstruct.h"
#include "ouch.h"
#include "player.h"
#include "randart.h"
#include "religion.h"
#include "skills2.h"
#include "stuff.h"
#include "files.h"

void highscore(std::string death_string, int points);
void item_corrode(char itco);
void end_game(char end_status, const char death_string [80]);
int set_status(int stat);


int points = 0;

extern int wield_change; /* defined in output.cc */

/* NOTE: DOES NOT check for hellfire!!! */
int check_your_resists(int hurted, int flavour)
{

        switch(flavour)
        {
                case 2: /* fire */
                if (player_res_fire() > 100)
                {
                        mpr("You resist.");
                        hurted /= 2 + ((player_res_fire() - 100) * (player_res_fire() - 100));
                } else
                       if (player_res_fire() < 100)
                       {
                        mpr("It burns terribly!");
                        hurted *= 15;
                        hurted /= 10;
                       }
                break;

                case 3: /* cold */
                if (player_res_cold() > 100)
                {
                        mpr("You resist.");
                        hurted /= 2 + ((player_res_cold() - 100) * (player_res_cold() - 100));
                } else
                       if (player_res_cold() < 100)
                       {
                        mpr("You feel a terrible chill!");
                        hurted *= 15;
                        hurted /= 10;
                       }
                break;

 case 5: /* electricity */
 if (you[0].attribute [ATTR_RESIST_LIGHTNING] > 0)
 {
  mpr("You resist.");
  hurted = 0;
 }
 break;


 case 6: /* poison */
 if (player_res_poison() > 0)
 {
  mpr("You resist.");
  hurted = 0;
 } else you[0].poison += random() % 2 + 1;
        break;

 case 7: /* negativity */
 if (player_prot_life() != 0)
        {
  mpr("You resist.");
                hurted = 0;
                break;
        }
 mpr("You feel drained!");
 drain_exp();
        break;

                case 23: /* ice */
                if (player_res_cold() > 100)
                {
                        mpr("You partially resist.");
                        hurted /= 2; /* should really be more complex, but I don't have time right now */
                } else
                       if (player_res_cold() < 100)
                       {
                        mpr("You feel a painful chill!");
                        hurted *= 13;
                        hurted /= 10;
                       }
                break;


 } /* end switch */

        if (flavour == 20)
        {
                if (player_res_fire() > 100)
                {
                        mpr("You partially resist.");
                        hurted /= 1 + (player_res_fire() - 100);
                }
                if (player_res_fire() < 100)
                {
                        mpr("It burns terribly!");
                        hurted *= 15;
                        hurted /= 10;
                }
        }

return hurted;

}




void splash_with_acid(char acid_strength)
{

/* affects equip only?

 assume that a message has already been sent, also that damage has
 already been done */


//abort();

int splc = 0;

for (splc = 1; splc < 7; splc++)
{
        if (you[0].equip [splc] == -1)
 {
        ouch(random2(acid_strength) / 5, 0, 3);

    continue;
    /* should take extra damage if little armour worn*/
 }

        if (random2(20) > acid_strength) continue;

        item_corrode(you[0].equip [splc]);

}

}


void weapon_acid(char acid_strength)
{
        char hand_thing = you[0].equip [EQ_WEAPON];

        if (you[0].equip [EQ_WEAPON] == -1)
        {
                if (you[0].equip [EQ_GLOVES] != -1) hand_thing = you[0].equip [EQ_GLOVES];
                        else return; /* take extra damage */
        }

        if (random2(20) > acid_strength) return;

        item_corrode(hand_thing);

}



void item_corrode(char itco_char)
{
	int itco = itco_char;

        int chance_corr = 0;
        int rusty = 0;
        if (you[0].inv_class [itco] == 0) rusty = you[0].inv_plus2 [itco];
                else rusty = you[0].inv_plus [itco];

        if (you[0].inv_class [itco] == 0 || you[0].inv_class [itco] == 1)
        {
         if (you[0].inv_dam [itco] / 30 == 5 && random() % 5 != 0) return;
         /* dwarven stuff is resistant to acids */
        }

        if (rusty < 45) return;
 if (you[0].inv_class [itco] == 0 && you[0].inv_dam [itco] > 180) return; // unique
 if (you[0].inv_class [itco] == 0 && you[0].inv_dam [itco] % 30 >= 25) return; // unique
 if (you[0].inv_class [itco] == 2 && you[0].inv_dam [itco] % 30 >= 25) return; // unique
 if (wearing_amulet(AMU_RESIST_CORROSION) == 1 && random() % 10 != 0)
 {
#ifdef DEBUG
 mpr("Implant protects.");
#endif
 return; /* amulet of resist corrosion/cloak of preservation */
 }
        chance_corr = rusty;
        if (chance_corr > 130) chance_corr -= 100;

        if (chance_corr < 50)
        {
                chance_corr = 50 + ((chance_corr - 50) * -1);
        }

        chance_corr -= 50;

        switch(chance_corr)
        {
                case 0: if (random2(100) < 10) return; break;
                case 1: if (random2(100) < 25) return; break;
                case 2: if (random2(100) < 40) return; break;
                case 3: if (random2(100) < 70) return; break;
                case 4: if (random2(100) < 95) return; break;
                default: return;
        }



        msg("@1 is eaten away!") << in_name(itco, 4);

        rusty --;
        if (you[0].inv_class [itco] == 0) you[0].inv_plus2 [itco] = rusty;
                else you[0].inv_plus [itco] = rusty;

        you[0].AC_ch = 1;

        if (you[0].equip [EQ_WEAPON] == itco) wield_change = 1;

}



void scrolls_burn(char burn_strength, char target_class)
{

int burnc;
int burn2;
int burn_no = 0;

if (wearing_amulet(AMU_CONSERVATION) == 1 && random() % 10 != 0)
{
#ifdef DEBUG
 mpr("Implant conserves.");
#endif
 return;
}

for (burnc = 0; burnc < 52; burnc++)
{
        if (you[0].inv_quant [burnc] == 0) continue;
        if (you[0].inv_class [burnc] != target_class) continue; /* 6 scrolls, 8 potions */

        for (burn2 = 0; burn2 < you[0].inv_quant [burnc]; burn2++)
        {
                if (random2(70) < burn_strength)
                {
                        you[0].inv_quant [burnc] --;
                        burn_no++;
                        if (you[0].inv_quant [burnc] <= 0)
                        {
                                you[0].inv_no--;
                                if (burnc == you[0].equip [EQ_WEAPON]) // I can't assume any level of intelligence on the player's behalf.
                                {
                                        you[0].equip [EQ_WEAPON] = -1;
                                        mpr("You are now empty handed.");
                                }
                                break;
                        }
                }
        }
}

if (burn_no == 1)
{
        if (target_class == 6) mpr("A device you are carrying catches fire!");
        if (target_class == 8) mpr("A vial you are carrying freezes and shatters!");
 if (target_class == 4) mpr("Some of your food is covered with spores!");
}

if (burn_no > 1)
{
        if (target_class == 6) mpr( "Some of the devices you are carrying catch fire!");
        if (target_class == 8) mpr( "Some of the vials you are carrying freeze and shatter!");
 if (target_class == 4) mpr( "Some of your food is covered with spores!");
}
/* burn_no could be 0 */

}


std::string species_abbr(int species)
{
	switch(species) {
		case 1: return "Hu";
		case 2: return "SN";
		case 3: return "Ni";
		case 4: return "GN";
		case 5: return "CN";
		case 6: return "NS";
		case 7: return "CE";
		case 8: return "ME";
		case 9: return "FE";
		case 10: return "Te";
		case 11: return "Sm";
		case 12: return "Ro";
		case 13: return "As";
		case 14: return "Ap";
		case 15: return "Mu";
		case 16: return "Bu";
		case 17: return "MH";
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29: return "Dr";
		case 30: return "Ra";
		case 31: return "Fr";
		case 32: return "Th";
		case 33: return "BM";
		case 34: return "Su";
		case 35: return "Lo";
		case 36: return "BM";
	}
	return "";
}

std::string death_reason(int death_type, int death_source)
{
	bool append_a = ((menv [death_source].m_class < 250 || menv [death_source].m_class > 310) && menv [death_source].m_class != 400);
	std::string a = append_a ? "a" : "";
	switch(death_type) {
		case 0: return ", killed by " + a + monam(menv [death_source].m_sec, menv [death_source].m_class, 0, 99);
		case 1: return ", killed by a lethal dose of poison";
		case 2: return ", killed by a cloud";
		case 3: return ", killed from afar by " + a + monam(menv [death_source].m_sec, menv [death_source].m_class, 0, 99);
		case 5: return " took a swim in radioactive waste";
		case 6: return (you[0].species == SP_MUMMY) ? " soaked and fell apart" : " drowned";
		case 7: return " died of stupidity";
		case 8: return " died of muscular atrophy";
		case 9: return " died of clumsiness";
		case 10: return ", killed by a trap";
		case 11: return " got out of the installation alive.";
		case 12: return " escaped with the Disk.";
		case 13: return " quit";
		case 14: return " was drained of all life";
		case 15: return " starved to death";
		case 16: return " froze to death";
		case 17: return " burned to death";
		case 18: return ", killed by corrupted program";
		case 19: return ", killed by The Anonimous";
		case 20: return ", killed by a statue";
		case 21: return " rotted away";
		case 22: return ", killed by bad targetting";
	}
	return "";
}

std::string location_name(int location)
{
	switch(location) {
		case 1:  return " of Iron Works";
		case 2:  return " of Refuelings";
		case 3:  return " in the Lobby";
		case 4:  return " of Cooling Plants";
		case 5:  return " of Cyborg Researchs";
		case 10: return " of the Terrorists";
		case 11: return " of the Hive";
		case 12: return " of the Biodome";
		case 13: return " of the Waste Pits";
		case 14: return " of the Storage Area";
		case 15: return " of the Cyborg Manufactory";
		case 16: return " of the Hall";
		case 17: return " of Alice's Hall";
		case 18: return " of the Terminal Hub";
		case 19: return " of the Snake Pit";
		case 20: return " of the Ninja Palace";
		case 21: return " of the Cyborg Hub";
		case 22: return " of the Greenhouse";
	}
	return "";
}

std::string death_location()
{
	if (you[0].level_type == 2) {
		return " in the Dump.";
	}
	if (you[0].level_type == 3) {
		return " in Bioengineerings.";
	}
	if (you[0].level_type == 1) {
		return " in a labyrinth.";
	}

	if (you[0].where_are_you != 3) {
		int level = (you[0].your_level + 1);
		if (you[0].where_are_you >= 1 && you[0].where_are_you <= 9) {
			level = you[0].your_level + 1 - 26;
		}
		if (you[0].where_are_you >= 10) {
			level = you[0].your_level - you[0].branch_stairs [you[0].where_are_you - 10];
		}
		Format format(" on L@1@2.");
		format << level << location_name(you[0].where_are_you);
		return format.str();
	}
	return location_name(you[0].where_are_you) + ".";
}

void ouch(int dam, char death_source_char, char death_type)
{
	int death_source = death_source_char;
	int d = 0;
	int e = 0;

	if (you[0].deaths_door > 0 && death_type != 4 && death_type != 5 && death_type != 6) {
		return;
	}

	if (dam > -9000) {
		switch(you[0].religion) {
			case 5:
				if (random2(you[0].hp_max) > you[0].hp && dam > random2(you[0].hp) && random2(5) == 0) {
					mpr("The Anonimous protects you from harm!");
					return;
				}
				break;
			case 1:
			case 2:
			case 3:
			case 7:
			case 12:
				if (dam >= you[0].hp && you[0].duration [DUR_PRAYER] > 0 && random2(you[0].piety) >= 30) {
					msg("@1 protects you from harm!") << god_name(you[0].religion);
					return;
				}
				break;
		}
	}

	you[0].hp_ch = 1;

	if (dam > 300) return; /* assume it's a bug */

	if (dam > -9000) {
		you[0].hp -= dam;
		if (you[0].hp > 0) return;
	}

#ifdef DEBUG
	if (death_type != 13 && death_type != 12 && death_type != 11) // quit or escaped
	{
		mpr("Since you're a debugger, I'll let you live.");
		mpr("Be more careful next time, okay?");
		// more2();
		you[0].hp = you[0].hp_max;
		you[0].hp_ch = 1;
		return;
	}
#endif

	// okay, so you're dead.

	points += you[0].gp;
	points += (you[0].xp * 7) / 10;
	//if (death_type == 12) points += points / 2;
	//if (death_type == 11) points += points / 10; // these now handled by giving player the value of their inventory
	int temp_id [4] [50];

	for (d = 0; d < 4; d ++)
	{
		for (e = 0; e < 50; e ++)
		{
			temp_id [d] [e] = 1;
		}
	}
	if (death_type == 11 || death_type == 12)
	{
		for (d = 0; d < 52; d ++)
		{
			points += item_value(you[0].inv_class [d], you[0].inv_type [d], you[0].inv_dam [d], you[0].inv_plus [d], you[0].inv_plus2 [d], you[0].inv_quant [d], 3, temp_id);
		}
	}

	if (points > 99999999) points = 99999999;

	std::string death_string;
	death_string += to_string(points, 9);

	death_string += std::string(you[0].your_name, 14);
	death_string += "-";
	death_string += species_abbr(you[0].species);
	death_string += you[0].clasnam [0];

	death_string += to_string(you[0].xl);

	death_string += death_reason(death_type, death_source);

	if (death_type != 11 && death_type != 12) {
		death_string += death_location();
		save_ghost();
		end_game(1, death_string.c_str());
	}
	end_game(0, death_string.c_str()); // must have won! (or at least escaped)
}

void end_game(char end_status, const char death_string [80])
{
	int handle, i;
	char status2 = end_status;
	set_status(end_status);

	char del_file [55];
	int sysg = 0;
	strcpy(del_file, "");
	for (i = 0; i < 6; i ++) {
		del_file [i] = you[0].your_name [i];
		if (you[0].your_name [i] == 0)
			break;
	}
	del_file [6] = 0;

	char glorpstr [40];
	strncpy(glorpstr, you[0].your_name, 6);

	/* This is broken. Length is not valid yet! We have to check if we got a
	   trailing NULL; if not, write one: */
	if (strlen(you[0].your_name) > 5)    /* is name 6 chars or more? */
		glorpstr[6] = (char) NULL;   /* if so, char 7 should be NULL */

	int fi = 0;
	int fi2 = 0;

	for (fi2 = 0; fi2 < 30; fi2 ++) {
		for (fi = 0; fi < 50; fi ++) {
			Format format("@1.@2@3@4");
			format << glorpstr << ((fi < 10) ? "0" : "") << fi << char(fi2 + 97);
			handle = open(format.str().c_str(), S_IWRITE, S_IREAD);

			if (handle != -1)
			{
				close(handle);
				unlink(del_file);
			} else close(handle);
		}
	}

	strcpy(del_file, glorpstr);

	strcat(del_file, ".lab");
	sysg = unlink(del_file);

	strcpy(del_file, glorpstr);

	strcat(del_file, ".sav");
	sysg = unlink(del_file);

	status2 = set_status(100);


	if (status2 == 1)
	{
		mpr("You die...");
	}

	viewwindow(1);
	more();


	for (i = 0; i < 52; i ++)
	{
		you[0].inv_ident [i] = 3;
	}

	for (i = 0; i < 52; i ++)
	{
		if (you[0].inv_class [i] != 0)
		{
			set_id(you[0].inv_class [i], you[0].inv_type [i], 1);
		}
	}

	if (status2 == 0) 
		invent(-1, 1);
	else 
		invent(-1, 0);
	if (dump_char((status2 == 0), "morgue.txt") == 1)
		mpr("Char dumped successfully (morgue.txt).");
	else mpr("Char dump unsuccessful! Sorry about that.");
	more();
	int p = 0;
	for (p = 0; p < 52; p ++)
	{
		for (i = 0; i < ITEMS; i++)
		{
			if (mitm.iquant [i] == 0)
			{
				mitm.iid [i] = 0;
				mitm.iclass [i] = you[0].inv_class [p];
				mitm.itype [i] = you[0].inv_type [p];
				mitm.iplus [i] = you[0].inv_plus [p];
				mitm.iplus2 [i] = you[0].inv_plus2 [p];
				mitm.idam [i] = you[0].inv_dam [p];
				mitm.icol [i] = you[0].inv_col [p];
				mitm.ix [i] = you[0].x_pos;
				mitm.iy [i] = you[0].y_pos;
				mitm.iquant [i] = you[0].inv_quant [p];
				/*                it_no ++; */
				break;
			}
		} // end of i loop
	} // end of p loop


	for (i = 0; i < ITEMS; i ++)
	{
		mitm.iid [i] = 0;
	}


	clrscr();
	clrscr();
	cprintf("Goodbye, ");
	cprintf(you[0].your_name);
	cprintf(".");
	cprintf(EOL EOL);
	cprintf(death_string);

	get_ch();
	end(0);
}



void lose_level(void)
{
 if (you[0].xl == 1)
 {
    ouch(-9999, 0, 14);
 }
// because you[0].xp is int, if it's going to be -ve must die straightaway.

 you[0].xp = exp_needed(you[0].xl + 1, you[0].species) - 1;
        you[0].xl --;

                msg("You are now a level @1 @2!") << you[0].xl << you[0].clasnam;

//you[0].res_magic -= 3;

int brek = random2(3) + 4;
if (you[0].xl >= 12) brek = random2(3) + 2;
if (you[0].xl >= 21) brek = random2(2) + 2;

ouch(brek, 0, 14);
you[0].base_hp2 -= brek;

 brek = random2(4) + 3;
 if (you[0].xl > 12) brek = random2(3) + 2;
 if (you[0].xl > 21) brek = random2(2) + 2;


 you[0].ep -= 1;
 you[0].base_ep2 --;

 you[0].spell_levels --;

calc_hp();
calc_ep();

if (you[0].spell_levels < -40) you[0].spell_levels = -40;
if (you[0].ep < 0) you[0].ep = 0;

        you[0].xp_ch = 1;
        you[0].hp_ch = 1;
        you[0].ep_ch = 1;

        brek = 0;

}


void drain_exp(void)
{

if (you[0].duration [DUR_PRAYER] != 0 &&  (you[0].religion == GOD_ZIN || you[0].religion == GOD_SHINING_ONE || you[0].religion == GOD_ELYVILON) && random2(150) < you[0].piety)
{
         msg("@1 protects your life force!") << god_name(you[0].religion);
         return;
}


if (you[0].xp == 0)
{
 ouch(-9999, 0, 14);
}

if (you[0].xl == 1)
{
 you[0].xp = 0;
 return;
}

int total_exp = exp_needed(you[0].xl + 2, you[0].species) - exp_needed(you[0].xl + 1, you[0].species);
int exp_drained = total_exp * (10 + random() % 11);
exp_drained /= 100;

you[0].xp -= exp_drained;

#ifdef DEBUG
msg("You lose @1 experience points.") << exp_drained;
#endif

you[0].xp_ch = 1;

if (you[0].xp < exp_needed(you[0].xl + 1, you[0].species))
{
 lose_level();
}

}



int set_status(int stat)
{

 static int stat2;

 if (stat != 100) stat2 = stat;

 return stat2;

}


