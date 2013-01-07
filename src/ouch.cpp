
#include "linuxlib.h"

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

void highscore(char death_string [80], long points);
void item_corrode(char itco);
void end_game(char end_status);
int set_status(int stat);


char death_string [80];
long points = 0;

extern char wield_change; /* defined in output.cc */

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

//      if (stricmp(beam_name, "hellfire") == 0 || beam[0].flavour == 20) // 20 is lava
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

unsigned char splc = 0;

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
        unsigned char rusty = 0;
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

        item_name(you[0].inv_plus2 [itco], you[0].inv_class [itco], you[0].inv_type [itco], you[0].inv_dam [itco], you[0].inv_plus [itco], you[0].inv_quant [itco], you[0].inv_ident [itco], 4, str_pass);


        msg("@1 is eaten away!") << str_pass;

        rusty --;
        if (you[0].inv_class [itco] == 0) you[0].inv_plus2 [itco] = rusty;
                else you[0].inv_plus [itco] = rusty;

        you[0].AC_ch = 1;

        if (you[0].equip [EQ_WEAPON] == itco) wield_change = 1;

}



void scrolls_burn(char burn_strength, char target_class)
{

unsigned char burnc;
unsigned char burn2;
unsigned char burn_no = 0;

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



void ouch(int dam, char death_source_char, char death_type)
{
	int death_source = death_source_char;
char point_print [10];
int d = 0;
int e = 0;


 if (you[0].deaths_door > 0 && death_type != 4 && death_type != 5 && death_type != 6)
 {
  return;
 }

if (dam > -9000)
{

 switch(you[0].religion)
 {
 case 5:
 if (random2(you[0].hp_max) > you[0].hp && dam > random2(you[0].hp) && random2(5) == 0)
 {
  mpr("The Anonimous protects you from harm!");
  return;
 }
 break;
 case 1:
 case 2:
 case 3:
 case 7:
 case 12:
 if (dam >= you[0].hp && you[0].duration [DUR_PRAYER] > 0 && random2(you[0].piety) >= 30)
 {
  msg("@1 protects you from harm!") << god_name(you[0].religion);
  return;
 }
 break;

 }

}

you[0].hp_ch = 1;

if (dam > 300) return; /* assume it's a bug */

 if (dam > -9000)
        {
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
   char temp_id [4] [50];

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
itoa(points, point_print, 10);

strcpy(death_string, point_print);
if (points < 10000000) strcat(death_string, " ");
if (points < 1000000) strcat(death_string, " ");
if (points < 100000) strcat(death_string, " ");
if (points < 10000) strcat(death_string, " ");
if (points < 1000) strcat(death_string, " ");
if (points < 100) strcat(death_string, " ");
if (points < 10) strcat(death_string, " ");
/*for (i = 0; i < 12 - strlen(death_string); i ++)
{
 strcat(death_string, " ");
}*/

        strncat(death_string, you[0].your_name, 14);
        strcat(death_string, "-");
        switch(you[0].species)
        {
         case 1: strcat(death_string, "Hu"); break;
         case 2: strcat(death_string, "SN"); break;
         case 3: strcat(death_string, "Ni"); break;
         case 4: strcat(death_string, "GN"); break;
         case 5: strcat(death_string, "CN"); break;
         case 6: strcat(death_string, "NS"); break;
         case 7: strcat(death_string, "CE"); break;
         case 8: strcat(death_string, "ME"); break;
         case 9: strcat(death_string, "FE"); break;
         case 10: strcat(death_string, "Te"); break;
         case 11: strcat(death_string, "Sm"); break;
         case 12: strcat(death_string, "Ro"); break;
         case 13: strcat(death_string, "As"); break;
         case 14: strcat(death_string, "Ap"); break;
         case 15: strcat(death_string, "Mu"); break;
         case 16: strcat(death_string, "Bu"); break;
         case 17: strcat(death_string, "MH"); break;
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
         case 29: strcat(death_string, "Dr"); break;
         case 30: strcat(death_string, "Ra"); break;
         case 31: strcat(death_string, "Fr"); break;
         case 32: strcat(death_string, "Th"); break;
         case 33: strcat(death_string, "BM"); break;
         case 34: strcat(death_string, "Su"); break;
         case 35: strcat(death_string, "Lo"); break;
         case 36: strcat(death_string, "BM"); break;
        }
        death_string [strlen(death_string)] = you[0].clasnam [0];

itoa(you[0].xl, point_print, 10);
strcat(death_string, point_print);
//if (strlen(point_print) == 1) strcat(death_string, " ");

//      strcat(death_string, ", killed by ");
 switch(death_type)
        {

        case 0: // monster
                strcat(death_string, ", killed by ");
                if ((menv [death_source].m_class < 250 || menv [death_source].m_class > 310) && menv [death_source].m_class != 400) strcat(death_string, "a");
                strcat(death_string, monam(menv [death_source].m_sec, menv [death_source].m_class, 0, 99));
 break;

        case 1: // you[0].poison
//              if (dam == -9999) strcat(death_string, "an overload of ");
                strcat(death_string, ", killed by a lethal dose of poison");
 break;

        case 2: // cloud
                strcat(death_string, ", killed by a cloud");
        break;

        case 3: // beam - beam[0].name is a local variable, so can't access it without horrible hacks
                strcat(death_string, ", killed from afar by ");
                if ((menv [death_source].m_class < 250 || menv [death_source].m_class > 310) && menv [death_source].m_class != 400) strcat(death_string, "a");
                strcat(death_string, monam(menv [death_source].m_sec, menv [death_source].m_class, 0, 99));
 break;

/* case 4: // death's door running out - NOTE: This is no longer fatal
  strcat(death_string, " ran out of time");
 break;*/

 case 5: // falling into lava
  strcat(death_string, " took a swim in radioactive waste");
 break;

 case 6: // falling into water
 if (you[0].species == SP_MUMMY) strcat(death_string, " soaked and fell apart");
   else strcat(death_string, " drowned");
 break;

// these three are probably only possible if you wear a you[0].ring of >= +3 ability,
//  get drained to 3, then take it off, or have a very low abil and wear a -ve you[0].ring.
// or, as of 2.7x, mutations can cause this
 case 7: // lack of intelligence
  strcat(death_string, " died of stupidity");
        break;
 case 8: // lack of str
  strcat(death_string, " died of muscular atrophy");
        break;
 case 9: // lack of dex
  strcat(death_string, " died of clumsiness"); // crappy message
        break;

 case 10:
  strcat(death_string, ", killed by a trap");
 break;

 case 11:
  strcat(death_string, " got out of the installation alive.");
 break;

 case 12:
  strcat(death_string, " escaped with the Disk.");
 break;

 case 13:
  strcat(death_string, " quit");
 break;

 case 14:
  strcat(death_string, " was drained of all life");
 break;

 case 15:
  strcat(death_string, " starved to death");
 break;

 case 16:
  strcat(death_string, " froze to death");
 break;

 case 17:
  strcat(death_string, " burned to death");
 break;

 case 18: /* from function miscast_effect */
  strcat(death_string, ", killed by corrupted program");
 break;

 case 19:
  strcat(death_string, ", killed by The Anonimous");
 break;

 case 20:
  strcat(death_string, ", killed by a statue");
 break;

 case 21:
  strcat(death_string, " rotted away");
 break;

 case 22:
  strcat(death_string, ", killed by bad targetting");
 break;


 } // end switch

if (death_type != 11 && death_type != 12)
{

 if (you[0].level_type == 2)
 {
   strcat(death_string, " in the Dump.");
   goto ending;
 }
 if (you[0].level_type == 3)
 {
   strcat(death_string, " in Bioengineerings.");
   goto ending;
 }
 if (you[0].level_type == 1)
 {
   strcat(death_string, " in a labyrinth.");
   goto ending;
 }

        itoa((you[0].your_level + 1), st_prn, 10);

 if (you[0].where_are_you >= 1 && you[0].where_are_you <= 9)
    itoa(you[0].your_level + 1 - 26, st_prn, 10);

 if (you[0].where_are_you >= 10)
 {
  itoa(you[0].your_level - you[0].branch_stairs [you[0].where_are_you - 10], st_prn, 10);
 }
/* switch(you[0].where_are_you)
 {
  case 10: itoa(you[0].your_level - you[0].branch_stairs [0], st_prn, 10); break;
  case 11: itoa(you[0].your_level - you[0].branch_stairs [1], st_prn, 10); break;
  case 12: itoa(you[0].your_level - you[0].branch_stairs [2], st_prn, 10); break;
  case 13: itoa(you[0].your_level - you[0].branch_stairs [3], st_prn, 10); break;
  case 14: itoa(you[0].your_level - you[0].branch_stairs [4], st_prn, 10); break;
 }*/
if (you[0].where_are_you != 3)
{
        strcat(death_string, " on L");
        strcat(death_string, st_prn);
}

switch (you[0].where_are_you)
{
 case 1: strcat(death_string, " of Iron Works"); break;
 case 2: strcat(death_string, " of Refuelings"); break;
 case 3: strcat(death_string, " in the Lobby"); break;
 case 4: strcat(death_string, " of Cooling Plants"); break;
 case 5: strcat(death_string, " of Cyborg Researchs"); break;
 case 10: strcat(death_string, " of the Terrorists"); break;
 case 11: strcat(death_string, " of the Hive"); break;
 case 12: strcat(death_string, " of the Biodome"); break;
 case 13: strcat(death_string, " of the Waste Pits"); break;
 case 14: strcat(death_string, " of the Storage Area"); break;
 case 15: strcat(death_string, " of the Cyborg Manufactory"); break;
 case 16: strcat(death_string, " of the Hall"); break;
 case 17: strcat(death_string, " of Alice's Hall"); break;
 case 18: strcat(death_string, " of the Terminal Hub"); break;
 case 19: strcat(death_string, " of the Snake Pit"); break;
 case 20: strcat(death_string, " of the Ninja Palace"); break;
 case 21: strcat(death_string, " of the Cyborg Hub"); break;
 case 22: strcat(death_string, " of the Greenhouse"); break;
}



        strcat(death_string, ".");

  save_ghost();

        ending : end_game(1);
}

 end_game(0); // must have won! (or at least escaped)

}





void end_game(char end_status)
{
   int handle, i;

   char status2 = end_status;

   set_status(end_status);


        char del_file [55];

        int sysg = 0;

strcpy(del_file, "");
for (i = 0; i < 6; i ++)
{
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
char st_prn [6];

for (fi2 = 0; fi2 < 30; fi2 ++)
{
 for (fi = 0; fi < 50; fi ++)
 {
  strcpy(del_file, glorpstr);
  strcat(del_file, ".");
  if (fi < 10) strcat(del_file, "0");
  itoa(fi, st_prn, 10);
  strcat(del_file, st_prn);
  st_prn [0] = fi2 + 97;
  st_prn [1] = 0;
  strcat(del_file, st_prn);
  strcat(del_file, "\0");
  handle = open(del_file, S_IWRITE, S_IREAD);

  if (handle != -1)
  {
        close(handle);
        unlink(del_file);
  } else close(handle);
 }
}

/*int fi = 0;

for (fi = 0; fi < 100; fi ++)
{
strcpy(del_file, glorpstr);
strcat(del_file, ".");
itoa(fi, st_prn, 10);
strcat(del_file, st_prn);
strcat(del_file, "\0");
handle = open(del_file, S_IWRITE, S_IREAD);

if (handle != -1)
{
        close(handle);
        sysg = unlink(del_file);
} else close(handle);
}*/


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

if (status2 == 0) /* invent(you[0].inv_plus2, -1, you[0].inv_quant, you[0].inv_dam, you[0].inv_class, you[0].inv_type, you[0].inv_plus, you[0].inv_ident, you[0].equip [EQ_WEAPON], you[0].equip [EQ_BODY_ARMOUR], you[0].equip [EQ_SHIELD], you[0].equip [EQ_HELMET], you[0].equip [EQ_CLOAK], you[0].equip [EQ_GLOVES], you[0].equip [EQ_BOOTS], you[0].ring, 1); */
        invent(-1, 1);
  else /* invent(you[0].inv_plus2, -1, you[0].inv_quant, you[0].inv_dam, you[0].inv_class, you[0].inv_type, you[0].inv_plus, you[0].inv_ident, you[0].equip [EQ_WEAPON], you[0].equip [EQ_BODY_ARMOUR], you[0].equip [EQ_SHIELD], you[0].equip [EQ_HELMET], you[0].equip [EQ_CLOAK], you[0].equip [EQ_GLOVES], you[0].equip [EQ_BOOTS], you[0].ring, 0); */
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
        cprintf(EOL EOL" Best Crawlers - "EOL);


highscore(death_string, points);
get_ch();
        end(0);
}



void highscore(char death_string [80], long points)
{

char high_scores [20] [80];
long scores [20];
int hc = 0;
int hc2 = 0;
int hc3 = 0;
char ready [2];
int multip = 1;

ready [0] = 0;
ready [1] = 0;

for (hc2 = strlen(death_string); hc2 < 80; hc2 ++)
{
 death_string [hc2] = ' ';
}

for (hc = 0; hc < 20; hc ++)
{
 strcpy(high_scores [hc], "-empty");
 scores [hc] = 0;
}

int handle = open("scores", S_IWRITE, S_IREAD);

if (handle == -1)
{
for (hc = 0; hc < 15; hc ++)
{
 for (hc2 = 0; hc2 < 80; hc2 ++)
 {
  high_scores [hc] [hc2] = 32;
 }
   strcpy(high_scores [hc], "0       empty");
   scores [hc] = 0;

}
goto out_of_ready;
}


for (hc = 0; hc < 15; hc ++)
{
 for (hc2 = 0; hc2 < 80; hc2 ++)
 {
  read(handle, ready, 1);
  if (ready [0] == 13 || ready [0] == 0 || ready [0] == '\r' || ready [0] == '\n')
  {
   for (hc3 = hc2; hc3 < 80; hc3 ++)
   {
    high_scores [hc] [hc3] = 32;
    goto out_of_inner;
   }
  }
  if (ready [0] == EOF) goto out_of_ready;
  if (ready [0] == 0 || ready [0] == 7) ready [0] = 32;
  high_scores [hc] [hc2] = ready [0];
 } // end for hc2
out_of_inner : hc3 = 0;
} // end for hc

for (hc = 0; hc < 15; hc ++)
{
 multip = 1;
 for (hc2 = 6; hc2 >= 0; hc2 --)
 {
  if (high_scores [hc] [hc2] == 32) continue;
  scores [hc] += (high_scores [hc] [hc2] - 48) * multip;
  multip *= 10;
 }
}

out_of_ready :

int placed = 0;
char has_printed = 0;

for (hc = 0; hc < 15; hc ++)
{
 placed ++;
if (points > scores [hc] && has_printed == 0)
{
 textcolor(YELLOW);
  itoa(placed, ready, 10);
  cprintf(ready);
  if (strlen(ready) == 1) cprintf("- "); else cprintf("-");
 for (hc2 = 0; hc2 < 80; hc2 ++)
 {
  if (death_string [hc2] == 32 && death_string [hc2 + 1] == 32 && hc2 > 15)
  {
   cprintf(EOL);
   break;
  }
  putch(death_string [hc2]);
 }
 has_printed = 1;
 hc --;
 textcolor(LIGHTGREY);
 continue;
}
  itoa(placed, ready, 10);
  cprintf(ready);
  if (strlen(ready) == 1) cprintf("- "); else cprintf("-");
 for (hc2 = 0; hc2 < 80; hc2 ++)
 {
  if (high_scores [hc] [hc2] == 32 && high_scores [hc] [hc2 + 1] == 32 && hc2 > 15)
  {
   cprintf(EOL);
   break;
  }
  putch(high_scores [hc] [hc2]);
 }
}

close(handle);

//handle = open("scores", O_CREAT || O_TRUNC | O_BINARY, S_IWRITE | S_IREAD);
//handle = open("scores", O_WRONLY | O_BINARY, S_IWRITE, S_IREAD);
//handle = open("scores", O_BINARY, 0660);
handle = open("scores", O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0660); //S_IREAD | S_IWRITE);

if (handle == -1)
{
        perror("oh NO!!!");
        if (getch() == 0) getch();
        return;
}

for (hc = 0; hc < 15; hc ++)
{
 for (hc2 = strlen(high_scores [hc]); hc2 < 80; hc2 ++)
 {
  high_scores [hc] [hc2] = ' ';
 }
}
has_printed = 0;
for (hc = 0; hc < 15; hc ++)
{
 if (has_printed == 1 && hc == 14) break;
 if (points > scores [hc] && has_printed == 0)
 {
  write(handle, death_string, 80);
  hc --;
  has_printed = 1;
 } else write(handle, high_scores [hc], 80);
}

close(handle);

} // end void highscores




void lose_level(void)
{
 if (you[0].xl == 1)
 {
    ouch(-9999, 0, 14);
 }
// because you[0].xp is unsigned long, if it's going to be -ve must die straightaway.

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

unsigned long total_exp = exp_needed(you[0].xl + 2, you[0].species) - exp_needed(you[0].xl + 1, you[0].species);
unsigned long exp_drained = total_exp * (10 + random() % 11);
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


