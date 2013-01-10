
#include "linuxlib.h"

#include <string.h>
#include <stdlib.h>

#include "externs.h"
#include "enum.h"

#include "beam.h"
#include "direct.h"
#include "fight.h"
#include "invent.h"
#include "itemname.h"
#include "items.h"
#include "misc.h"
#include "monplace.h"
#include "monstuff.h"
#include "mstruct.h"
#include "player.h"
#include "priest.h"
#include "randart.h"
#include "skills.h"
#include "spells0.h"
#include "spells2.h"
#include "spells3.h"
#include "stuff.h"
#include "view.h"
#include "it_use2.h"

extern int wield_change; /* defined in output.cc */

void blink(void);
void random_blink(void);
void stinkcl(char cl_x, char cl_y, struct bolt beam [1]);

void cast_big_c(int pow, char cty);
void big_cloud(char clouds, char cl_x, char cl_y, int pow);
char cast_lesser_healing(int mabil);
char cast_greater_healing(int mabil);
char cast_greatest_healing(int mabil);
char healing_spell(int healed);
void cast_revivification(int mabil);
void cast_cure_poison(int mabil);
void purification(void);
void cast_deaths_door(int pow);
void abjuration(int pow);
void extension(int pow);
void ice_armour(int pow, char extending);
void stone_scales(int pow);
void missile_prot(int pow);
void deflection(int pow);
void cast_regen(int pow);
void cast_berserk(void);
void cast_swiftness(int power);
void cast_fly(int power);
void cast_insulation(int power);
void cast_teleport_control(int power);
void cast_resist_poison(int power);


void blink(void)
{

struct dist beam [1];

if (you[0].equip [EQ_WEAPON] != -1 && you[0].inv_class [you[0].equip [EQ_WEAPON]] == 0 && you[0].inv_dam [you[0].equip [EQ_WEAPON]] % 30 >= 25)
 if (randart_wpn_properties(you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], 0, RAP_PREVENT_TELEPORTATION) > 0)
 {
  mpr("You feel a weird sense of stasis.");
  return;
 }

if (you[0].conf != 0) random_blink();

start_blink: mpr("Jump to where?");

direction(100, beam);

if (beam[0].nothing == -1)
{
	mpr("The program fizzles.");
	return;
}

if (see_grid(beam[0].target_x, beam[0].target_y) == 0)
{
	mpr("You can't jump there!");
	goto start_blink;
}

if (grd [beam[0].target_x] [beam[0].target_y] <= 10 || mgrd [beam[0].target_x] [beam[0].target_y] != MNG)
{
  mpr("Oops! Maybe something was there already.");
  random_blink();
  return;
}

you[0].x_pos = beam[0].target_x;
you[0].y_pos = beam[0].target_y;


if (you[0].level_type == 2)
{
 abyss_teleport();
 env[0].cloud_no = 0;
 you[0].pet_target = MHITNOT;
}

}

void random_blink(void)
{
 int passed [2];

if (you[0].equip [EQ_WEAPON] != -1 && you[0].inv_class [you[0].equip [EQ_WEAPON]] == 0 && you[0].inv_dam [you[0].equip [EQ_WEAPON]] % 30 >= 25)
 if (randart_wpn_properties(you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], 0, RAP_PREVENT_TELEPORTATION) > 0)
 {
  mpr("You feel a weird sense of stasis.");
  return;
 }


 if (you[0].attribute [ATTR_CONTROL_TELEPORT] != 0 && you[0].conf == 0)
 {
  blink();
  return;
 }

 if (random_near_space(passed) == 0 || (you[0].x_pos == passed [0] && you[0].y_pos == passed [1]))
	{
		mpr("You feel rather strange for a moment.");
		return;
	}

	mpr("You jump.");

	you[0].x_pos = passed [0];
	you[0].y_pos = passed [1];

if (you[0].level_type == 2)
{
 abyss_teleport();
 env[0].cloud_no = 0;
 you[0].pet_target = MHITNOT;
}
}



void fireball(int power)
{
	mpr("Which direction? (* to target)");
	if (you[0].prev_targ != MHITNOT && you[0].prev_targ < MNST) {
		if (mons_near(you[0].prev_targ) == 1 && (menv [you[0].prev_targ].m_ench [2] != 6 || player_see_invis() != 0))
		{
			msg("You are currently targetting @1 (p to target).") << monam(menv[you[0].prev_targ].m_sec, menv[you[0].prev_targ].m_class, menv[you[0].prev_targ].m_ench [2], 1);
		} else {
			mpr("You have no current target.");
		}
	}

	struct dist fire_ball [1];
	direction(1, fire_ball);
	if (fire_ball[0].nothing == -1) {
		mpr("The program fizzles.");
		return;
	}

	struct bolt beam [1];
	beam[0].source_x = you[0].x_pos; beam[0].source_y = you[0].y_pos;
	beam[0].move_x = fire_ball[0].move_x;
	beam[0].move_y = fire_ball[0].move_y;
	beam[0].target_x = fire_ball[0].target_x;
	beam[0].target_y = fire_ball[0].target_y;
	zapping(ZAP_FIREBALL, power, beam);
}

void cast_fire_storm(int powc)
{

   char stx = 0;
   char sty = 0;
   char cl_x = 0;
			char cl_y = 0;
   int summd = 0;

 mpr("Where?");

struct dist fire_storm [1];
struct bolt beam [1];
direction(100, fire_storm);

beam[0].move_x = fire_storm[0].move_x;
beam[0].move_y = fire_storm[0].move_y;
beam[0].target_x = fire_storm[0].target_x;
beam[0].target_y = fire_storm[0].target_y;

if (fire_storm[0].nothing == -1)
{
	mpr("The program fizzles.");
	return;
}

cl_x = beam[0].target_x;
cl_y = beam[0].target_y;

 mpr("A raging storm of fire appears!");

beam[0].colour = RED;

 for (stx = -2; stx < 3; stx++)
   {

      for (sty = -2; sty < 3; sty++)
      {
          if ((stx == -2 && sty == -2) || (stx == -2 && sty == 2) || (stx == 2 && sty == -2) || (stx == 2 && sty == 2))
														continue;

         	beam[0].range = 1 + random2(5) + random2(powc) / 5 + random2(powc) / 5;
                       /* was 1 */
          if (beam[0].range > 20) beam[0].range = 20 + random2(4);
            beam[0].type = 1;

            beam[0].target_x = cl_x + stx;
            beam[0].target_y = cl_y + sty;

            if (random2(4) == 0 && grd [beam[0].target_x] [beam[0].target_y] == 67 && mgrd [beam[0].target_x] [beam[0].target_y] == MNG) // > 10 && grd [beam[0].target_x] [beam[0].target_y] < 100 && (beam[0].target_x != you[0].x_pos || beam[0].target_y != you[0].y_pos))
            {
                   summd = mons_place(21, 1, beam[0].target_x, beam[0].target_y, 4, MHITNOT, 250, you[0].your_level);
            }

            if (grd [beam[0].target_x] [beam[0].target_y] > 10 && env[0].cgrid [beam[0].target_x] [beam[0].target_y] == CNG && (beam[0].target_x != you[0].x_pos || beam[0].target_y != you[0].y_pos))
            {
               place_cloud(1, beam[0].target_x, beam[0].target_y, beam[0].range);
            }

      } /* end of sty */

   } /* end of stx */

viewwindow(1);

} /* end of cast_fire_storm */



char spell_direction(struct dist spelld [1], struct bolt beam [1])
{
	mpr("Which direction? (* to target)");
	if (you[0].prev_targ != MHITNOT && you[0].prev_targ < MNST) {
		if (mons_near(you[0].prev_targ) == 1 && (menv [you[0].prev_targ].m_ench [2] != 6 || player_see_invis() != 0)) {
			msg("You are currently targetting @1 (p to target).") << monam(menv[you[0].prev_targ].m_sec, menv[you[0].prev_targ].m_class, menv[you[0].prev_targ].m_ench [2], 1);
		} else {
			mpr("You have no current target.");
		}
	}

	direction(1, spelld);

	if (spelld[0].nothing == -1)
	{
		mpr("The program fizzles.");
		return -1;
	}
	beam[0].move_x = spelld[0].move_x;
	beam[0].move_y = spelld[0].move_y;
	beam[0].target_y = spelld[0].target_y;
	beam[0].target_x = spelld[0].target_x;
	beam[0].source_x = you[0].x_pos;
	beam[0].source_y = you[0].y_pos;
	return 1;

}

std::string equip_type(int sc_read_2)
{
	if (sc_read_2 == you[0].equip [EQ_WEAPON]) {
		return " (weapon in hand)";
	} else if (sc_read_2 == you[0].equip [EQ_BODY_ARMOUR]) {
		return " (being worn)";
	} else if (sc_read_2 == you[0].equip [EQ_LEFT_RING]) {
		return " (on left hemisphere)";
	} else if (sc_read_2 == you[0].equip [EQ_RIGHT_RING]) {
		return " (on right hemisphere)";
	} else if (sc_read_2 == you[0].equip [EQ_AMULET]) {
		return " (on vertebra)";
	}
	return "";
}

void identify(char pow)
{
   int id_used = 1;

	if (pow == 1 && random2(3) == 0) id_used = random2 (3) + 1;
	int nthing = 0;

	do
	{
	query : mpr("Identify which item?");

	int keyin = get_ch();

	if (keyin == '*' || keyin == '?')
	{
		if (keyin == '?') nthing = get_invent(-1);
		if (keyin == '*') nthing = get_invent(-1);
		if ((nthing >= 65 && nthing <= 90) || (nthing >= 97 && nthing <= 122))
		{
				keyin = nthing;
		} else
		{
/*       mesclr();*/
         goto query;
		}
	}

   if (keyin == 27) return;

	int sc_read_1 = (int) keyin;

	if (sc_read_1 < 65 || (sc_read_1 > 90 && sc_read_1 < 97) || sc_read_1 > 122)
	{
		mpr("You don't have any such object.");
		continue;
	}

	int sc_read_2 = conv_lett(sc_read_1);

	if (you[0].inv_quant [sc_read_2] == 0)
	{
		mpr("You don't have any such object.");
		continue;
	}


	set_id(you[0].inv_class [sc_read_2], you[0].inv_type [sc_read_2], 1);
	you[0].inv_ident [sc_read_2] = 3;

	if (sc_read_2 == you[0].equip [EQ_WEAPON]) {
		wield_change = 1;
	}

	char slot = (sc_read_2 <= 25) ? (sc_read_2 + 97) : (sc_read_2 + 39);
	msg("@1 - @2@3") << slot << in_name(sc_read_2, 3) << equip_type(sc_read_2);

	id_used -= 1;

	} while (id_used > 0);

} /* end of identify */


void conjure_flame(int pow)
{

struct dist spelld [1];

 mpr("You execute a flaming cloud program!");

 mpr("Where?");

direc:
direction(100, spelld);

if (spelld[0].nothing == -1)
{
	mpr("The program fizzles.");
	return;
}

char ig = grd [spelld[0].target_x] [spelld[0].target_y];

if (see_grid(spelld[0].target_x, spelld[0].target_y) == 0)
{
   mpr("You can't see that place!");
   goto direc;
}


if (ig <= 10 || mgrd [spelld[0].target_x] [spelld[0].target_y] != MNG || env[0].cgrid [spelld[0].target_x] [spelld[0].target_y] != CNG)
{
   mpr("There's already something there!");
   goto direc;
}

/* Hmmm... This should really take account of you[0].invis monsters */

/*beam[0].source_x = you[0].x_pos; beam[0].source_y = you[0].y_pos;*/

int durat = 5 + random2(pow) / 2 + random2(pow) / 2;
if (durat > 23) durat = 23;

/*	beam[0].range = 5 + random2(pow) / 2 + random2(pow) / 2;
        if (beam[0].range > 23) beam[0].range = 23;
	beam[0].type = 1; */

 place_cloud(1, spelld[0].target_x, spelld[0].target_y, durat);

}



void stinking_cloud(void)
{

struct dist spelld [1];
struct bolt beam [1];

 mpr("Which direction? (* to target)");

 if (you[0].prev_targ != MHITNOT && you[0].prev_targ < MNST)
 {
  if (mons_near(you[0].prev_targ) == 1 && (menv [you[0].prev_targ].m_ench [2] != 6 || player_see_invis() != 0))
  {
	msg("You are currently targetting @1 (p to target).") << monam(menv[you[0].prev_targ].m_sec, menv[you[0].prev_targ].m_class, menv[you[0].prev_targ].m_ench [2], 1);
  } else {
	  mpr("You have no current target.");
  }
 }

direction(1, spelld);

if (spelld[0].nothing == -1)
{
	mpr("The program fizzles.");
	return;
}

beam[0].move_x = spelld[0].move_x;
beam[0].move_y = spelld[0].move_y;
beam[0].target_x = spelld[0].target_x;
beam[0].target_y = spelld[0].target_y;


beam[0].source_x = you[0].x_pos;
beam[0].source_y = you[0].y_pos;

strcpy(beam[0].beam_name, "ball of vapour");
beam[0].colour = GREEN;
beam[0].range = 7;
beam[0].damage = 0;
beam[0].hit = 20;
beam[0].type = '#';
beam[0].flavour = 8; // exploding gas on target // 7; // gas?
beam[0].thing_thrown = 4;//? ???? ?

missile(beam, -1);

beam[0].damage = magic_ability(30, you[0].intel);

stinkcl(beam[0].bx, beam[0].by, beam);
}


void stinkcl(char cl_x, char cl_y, struct bolt beam [1])
{

   char stx = 0;
   char sty = 0;

	for (stx = -1; stx < 2; stx++)
   {

      for (sty = -1; sty < 2; sty++)
      {

         	beam[0].range = random2(5) + 1 + random2(beam[0].damage / 50);
          if (beam[0].range > 10) beam[0].range = 10 + random2(3);
            beam[0].type = 2;

            beam[0].target_x = cl_x + stx;
            beam[0].target_y = cl_y + sty;

            if (grd [beam[0].target_x] [beam[0].target_y] > 10 && env[0].cgrid [beam[0].target_x] [beam[0].target_y] == CNG)
            {
//               place_cloud();
                 place_cloud(2, beam[0].target_x, beam[0].target_y, beam[0].range);
            }

      } // end of sty

   } // end of stx


}




void cast_big_c(int pow, char cty)
{

 mpr("Where do you want to put it?");

struct dist cdis [1];

direction(100, cdis);

if (cdis[0].nothing == -1)
{
	mpr("The program fizzles.");
	return;
}


//beam[0].damage = pow;

//big_cloud(cty, beam[0].target_x, beam[0].target_y, pow); //, beam);
big_cloud(cty, cdis[0].target_x, cdis[0].target_y, pow);

}


void big_cloud(char clouds, char cl_x, char cl_y, int pow) //, struct bolt beam [1])
{
   char stx = 0;
   char sty = 0;
   int lasts = 0;

/*if (clouds % 100 == 3) beam[0].colour = WHITE;
if (clouds % 100 == 4) beam[0].colour = LIGHTGREEN;*/

 for (stx = -1; stx < 2; stx++)
   {

      for (sty = -1; sty < 2; sty++)
      {

         	lasts = 2 + random2(5) + pow / 2;

          if (lasts > 12) lasts = 12 + random2(4);

            if (grd [cl_x + stx] [cl_y + sty] > 10 && env[0].cgrid [cl_x + stx] [cl_y + sty] == CNG)
            {
                 place_cloud(clouds, cl_x + stx, cl_y + sty, lasts);
            }

      } // end of sty

   } // end of stx

} // end of freezing_cloud(c,c)



char cast_lesser_healing(void)
{

 return healing_spell(5 + random2(4) + random2(4)); // + random2(mabil / 5));

} // end of lesser healing

char cast_greater_healing(void)
{

 return healing_spell(15 + random2(15) + random2(15));// + random2(mabil / 2));

} // end of void cast_greater_healing

char cast_greatest_healing(void)
{

 return healing_spell(20 + random2(25) + random2(25));// + random2(mabil) + random2(mabil));

} // end of void cast_greater_healing



char healing_spell(int healed)
{
int mgr = 0;
struct dist bmove [1];

dirc : mpr("Which direction?");
direction(0, bmove);

mgr = mgrd [you[0].x_pos + bmove[0].move_x] [you[0].y_pos + bmove[0].move_y];

if (bmove[0].nothing == -1)
{
	mpr("Huh?!");
	return 0;
}

if (bmove[0].move_x > 1 || bmove[0].move_y > 1)
{
	mpr("This program doesn't reach that far.");
        goto dirc;
}

if (bmove[0].move_x == 0 && bmove[0].move_y == 0)
{
	you[0].hp += healed;
	you[0].hp_ch = 1;
	if (you[0].hp > you[0].hp_max) you[0].hp = you[0].hp_max;
	mpr("You are healed.");
	return 1;
}

if (mgr == MNG)
{
	mpr("There isn't anything there!");
	return -1;
}

msg("You heal @1.") << monam(menv [mgr].m_sec,menv[mgr].m_class, menv [mgr].m_ench [2], 1);

menv [mgr].m_hp += healed;

if (menv [mgr].m_hp >= menv [mgr].m_hp_max)
{
 menv [mgr].m_hp = menv [mgr].m_hp_max;
 msg("@1 is completely healed.") << monam(menv [mgr].m_sec,menv[mgr].m_class, menv [mgr].m_ench [2], 0);
} else print_wounds(mgr);

return 1;

}




void cast_revivification(int mabil)
{

if (you[0].hp == you[0].hp_max)
{
 mpr("Nothing appears to happen.");
 return;
}

if (you[0].hp_max <= 20)
{
 mpr("You lack the resilience to execute this program.");
 return;
}

you[0].base_hp -= 2;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
if (random2(mabil) < 8) you[0].base_hp -= 1;
//if (you[0].hp_max <= 1) you[0].hp_max = 1;

calc_hp();

/*if (you[0].deaths_door != 0)
{
 strcpy(info, "Your body is healed in an excruciatingly painful way!");
 mpr(info);
 strcpy(info, "The spell is only partially successful.");
 mpr(info);
 you[0].hp = you[0].hp_max / 2 + 1;
 if (you[0].hp >= you[0].hp_max) you[0].hp = you[0].hp_max;
 you[0].hp_ch = 1;
 return;
}*/
 mpr("Your body is healed in an amazingly painful way.");

you[0].hp = you[0].hp_max;
you[0].hp_ch = 1;

} // end of void cast_greater_healing

void cast_cure_poison(int mabil)
{

if (you[0].poison == 0)
{
 mpr("Nothing appears to happen.");
 return;
}

you[0].poison -= 2 + random2(mabil) + random2(3);

if (you[0].poison <= 0)
{
 mpr("You feel the poison leave your system.");
 you[0].poison = 0;
 return;
}

 mpr("You feel most of the poison leave your system.");

} // end of cast_cure_poison

void purification(void)
{

 mpr("You feel purified!");

you[0].poison = 0;
you[0].rotting = 0;
you[0].conf = 0;
you[0].slow = 0;
you[0].paralysis = 0; // ?
you[0].disease = 0;
// this doesn't seem very useful, does it?

}

void cast_deaths_door(int pow)
{

if (you[0].is_undead != 0)
{
 mpr("You're already cyborg!");
 return;
}

if (you[0].deaths_door > 0)
{
 mpr("Your appeal for an extension has been denied.");
 return;
}

 mpr("You feel invincible!");
 mpr("You seem to hear sand running through an hourglass...");

you[0].deaths_door = 8 + random2(5) + random2(5) + random2(5) + random2(pow) / 10;
you[0].hp = you[0].skills [SK_NECROMANCY] + (you[0].religion == GOD_KIKUBAAQUDGHA) * 13;
if (you[0].hp > you[0].hp_max) you[0].hp = you[0].hp_max;
you[0].hp_ch = 1;

}

void abjuration(int pow)
{
/* can't use beam variables here, because of monster_die and the puffs of smoke */

int ab = 0;

 mpr("Send 'em back where they came from!");

for (ab = 0; ab < MNST; ab ++)
{
 if (menv [ab].m_class == -1) continue;
 if (mons_near(ab) == 0) continue;
 if (menv [ab].m_beh == 7) continue;
 if (menv [ab].m_ench_1 == 0 || menv [ab].m_ench [1] < 20 || menv [ab].m_ench [1] > 25) continue;

 menv [ab].m_ench [1] -= 1 + random2(pow) / 3;
 if (menv [ab].m_ench [1] <= 19)
 {
  monster_die(ab, 6, 0);
  continue;
 }
 msg("@1 shudders.") << monam (menv [ab].m_sec, menv [ab].m_class, menv [ab].m_ench [2], 0);

} // end of for ab

} // end of void abjuration

void extension(int pow)
{

//strcpy(info, "You feel magical!");
//mpr(info);

if (you[0].haste > 0)
{
 potion_effect(POT_SPEED, pow);
}

if (you[0].might > 0)
{
 potion_effect(POT_MIGHT, pow);
}

if (you[0].lev > 0)
{
 potion_effect(POT_LEVITATION, pow);
}

if (you[0].invis > 0)
{
 potion_effect(POT_INVISIBILITY, pow);
}

if (you[0].duration [DUR_ICY_ARMOUR] > 0)
{
 ice_armour(pow, 1);
}

if (you[0].duration [DUR_REPEL_MISSILES] > 0)
{
 missile_prot(pow);
}

if (you[0].duration [DUR_REGENERATION] > 0)
{
 cast_regen(pow);
}

if (you[0].duration [DUR_DEFLECT_MISSILES] > 0)
{
 deflection(pow);
}

if (you[0].shock_shield > 0)
{
 you[0].shock_shield += random2(pow) + 4;
 if (you[0].shock_shield > 25) you[0].shock_shield = 25;
 mpr("Your ring of flames program is extended.");
}

if (you[0].duration [DUR_VORPAL_BLADE] > 0 && you[0].duration [DUR_VORPAL_BLADE] < 80) you[0].duration [DUR_VORPAL_BLADE] += 10 + random() % 10;
if (you[0].duration [DUR_FIRE_BRAND] > 0 && you[0].duration [DUR_FIRE_BRAND] < 80) you[0].duration [DUR_FIRE_BRAND] += 10 + random() % 10;
if (you[0].duration [DUR_ICE_BRAND] > 0 && you[0].duration [DUR_ICE_BRAND] < 80) you[0].duration [DUR_ICE_BRAND] += 10 + random() % 10;
if (you[0].duration [DUR_LETHAL_INFUSION] > 0 && you[0].duration [DUR_LETHAL_INFUSION] < 80) you[0].duration [DUR_LETHAL_INFUSION] += 10 + random() % 10;

if (you[0].duration [DUR_SWIFTNESS] > 0) cast_swiftness(pow);
if (you[0].duration [DUR_INSULATION] > 0) cast_insulation(pow);
if (you[0].duration [DUR_STONEMAIL] > 0) stone_scales(pow);
if (you[0].duration [DUR_CONTROLLED_FLIGHT] > 0) cast_fly(pow);
// 13 is teleport countdown
if (you[0].duration [DUR_CONTROL_TELEPORT] > 0) cast_teleport_control(pow);
if (you[0].duration [DUR_RESIST_POISON] > 0) cast_resist_poison(pow);
/* 17 is breath */
if (you[0].duration [DUR_TRANSFORMATION] > 0)
{
 mpr("Your transformation has been extended.");
 you[0].duration [DUR_TRANSFORMATION] += 10 + random2(pow);
 if (you[0].duration [DUR_TRANSFORMATION] > 100) you[0].duration [DUR_TRANSFORMATION] = 100;
}
if (you[0].duration [DUR_DEATH_CHANNEL] > 0) cast_death_channel(pow);

} // end extension

void ice_armour(int pow, char extending)
{

//if (pow > 100) pow = 100;

int dur_change = 0;

if (you[0].equip [EQ_BODY_ARMOUR] != -1 && extending == 0)
{
 if (you[0].inv_type [you[0].equip [EQ_BODY_ARMOUR]] > 1 && you[0].inv_type [you[0].equip [EQ_BODY_ARMOUR]] != 16 && you[0].inv_type [you[0].equip [EQ_BODY_ARMOUR]] != 19 && (you[0].inv_type [you[0].equip [EQ_BODY_ARMOUR]] < 22 || you[0].inv_type [you[0].equip [EQ_BODY_ARMOUR]] > 25))
 {
  mpr("You are wearing too much armour.");
  return;
 }
}

if (you[0].duration [DUR_STONEMAIL] != 0)
{
 mpr("The program conflicts with another program still running.");
 return;
}

if (you[0].duration [DUR_ICY_ARMOUR] == 0)
{
 mpr("A film of ice covers your body!");
/* player_AC(you) += 6;*/
 you[0].AC_ch = 1;
} else mpr("Your icy armour thickens.");


 dur_change = 20 + random2(pow) + random2(pow);

 if (dur_change + you[0].duration [DUR_ICY_ARMOUR] >= 100) you[0].duration [DUR_ICY_ARMOUR] = 100;
  else you[0].duration [DUR_ICY_ARMOUR] += dur_change;
/*
 you[0].duration [DUR_ICY_ARMOUR] += 20 + random2(pow) / 2 + random2(pow) / 2;

 if (you[0].duration [DUR_ICY_ARMOUR] >= 100) you[0].duration [DUR_ICY_ARMOUR] = 100;
*/
}

void stone_scales(int pow)
{

//if (pow > 100) pow = 100;

int dur_change = 0;

if (you[0].duration [DUR_ICY_ARMOUR] != 0)
{
 mpr("The program conflicts with another program still running.");
 return;
}

if (you[0].duration [DUR_STONEMAIL] == 0)
{
 mpr("A set of stone scales covers your body!");
/* player_AC(you) += 7;
 player_evasion(you) -= 2;*/
 you[0].evasion_ch = 1;
 you[0].AC_ch = 1;
} else mpr("Your scaly armour looks firmer.");


 dur_change = 20 + random2(pow) + random2(pow);

 if (dur_change + you[0].duration [DUR_STONEMAIL] >= 100) you[0].duration [DUR_STONEMAIL] = 100;
  else you[0].duration [DUR_STONEMAIL] += dur_change;

 burden_change();

}


void missile_prot(int pow)
{

if (pow > 100) pow = 100;

 mpr("You feel protected from missiles.");

 you[0].duration [DUR_REPEL_MISSILES] += 10 + random2(pow) + random2(pow);

 if (you[0].duration [DUR_REPEL_MISSILES] >= 100) you[0].duration [DUR_REPEL_MISSILES] = 100;

}

void deflection(int pow)
{

if (pow > 100) pow = 100;

 mpr("You feel very safe from missiles.");

 you[0].duration [DUR_DEFLECT_MISSILES] += 15 + random2(pow);

 if (you[0].duration [DUR_DEFLECT_MISSILES] >= 100) you[0].duration [DUR_DEFLECT_MISSILES] = 100;

}

void cast_regen(int pow)
{

int dur_change = 0;

//if (pow > 150) pow = 150;

 mpr("Your skin crawls.");

if (you[0].duration [DUR_REGENERATION] == 0)
{
/* you[0].rate_regen += 100; */
 you[0].hunger_inc += 4;
}

 dur_change = 5 + random2(pow) + random2(pow) + random2(pow);

 if (dur_change + you[0].duration [DUR_REGENERATION] >= 100) you[0].duration [DUR_REGENERATION] = 100;
  else you[0].duration [DUR_REGENERATION] += dur_change;

// if (you[0].duration [4] >= 100) you[0].duration [1] = 100;

}

void cast_berserk(void)
{
  if (go_berserk() == 0)
  {
     mpr("You fail to go in battle mode.");
  }
}


void cast_swiftness(int power)
{

int dur_incr = 0;

 if (you[0].duration [DUR_SWIFTNESS] == 0)
 {
  if (player_fast_run() != 0)
  {
   mpr("You can already move quickly.");
   return;
  }
//  you[0].fast_run ++;
 }

 if (you[0].species != SP_NAGA) mpr("You feel quick on your feet.");
   else mpr("You feel quick.");

 dur_incr = random2(power) + random2(power) + 20;

 if (dur_incr + you[0].duration [DUR_SWIFTNESS] > 100) you[0].duration [DUR_SWIFTNESS] = 100;
   else you[0].duration [DUR_SWIFTNESS] += dur_incr;

}



void cast_fly(int power)
{

int dur_change = 0;

	if (you[0].lev == 0)
	{
		mpr("You fly up into the air.");
	} else
	      {
	       mpr("You feel more buoyant.");
	      }
   dur_change =	random2 (power) + random2(power) + 25;
 if (you[0].lev + dur_change > 100) you[0].lev = 100;
     else you[0].lev += dur_change;

 if (you[0].duration [DUR_CONTROLLED_FLIGHT] + dur_change > 100) you[0].duration [DUR_CONTROLLED_FLIGHT] = 100;
    else you[0].duration [DUR_CONTROLLED_FLIGHT] += dur_change;
 /* duration [12] makes the game think you're wearing an amulet of controlled flight */

        burden_change();

}

void cast_insulation(int power)
{

int dur_incr = 0;

 if (you[0].duration [DUR_INSULATION] == 0)
 {
  you[0].attribute [ATTR_RESIST_LIGHTNING] ++;
 }

 mpr("You feel insulated.");

 dur_incr = random2(power) + 10;

 if (dur_incr + you[0].duration [DUR_INSULATION] > 100) you[0].duration [DUR_INSULATION] = 100;
   else you[0].duration [DUR_INSULATION] += dur_incr;

}

void cast_resist_poison(int power)
{

int dur_incr = 0;

 mpr("You feel resistant to poison.");

 dur_incr = random2(power) + 10;

 if (dur_incr + you[0].duration [DUR_RESIST_POISON] > 100) you[0].duration [DUR_RESIST_POISON] = 100;
   else you[0].duration [DUR_RESIST_POISON] += dur_incr;

}

void cast_teleport_control(int power)
{

int dur_incr = 0;

 if (you[0].duration [DUR_CONTROL_TELEPORT] == 0)
 {
  you[0].attribute [ATTR_CONTROL_TELEPORT] ++;
 }

 mpr("You feel controlled.");

 dur_incr = random2(power) + 10;

 if (dur_incr + you[0].duration [DUR_CONTROL_TELEPORT] > 100) you[0].duration [DUR_CONTROL_TELEPORT] = 100;
   else you[0].duration [DUR_CONTROL_TELEPORT] += dur_incr;

}


void cast_ring_of_flames(int power)
{
 you[0].shock_shield += random2(power) / 10 + 4;
 if (you[0].shock_shield > 25) you[0].shock_shield = 25;
 mpr("The air around you leaps into flame!");
 manage_shock_shield();
}


