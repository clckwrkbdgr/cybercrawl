
#include "linuxlib.h"

#include <stdlib.h>
#include <string.h>

#include "externs.h"
#include "enum.h"

#include "beam.h"
#include "fight.h"
#include "itemname.h"
#include "misc.h"
#include "monplace.h"
#include "monstuff.h"
#include "mstruct.h"
#include "mstuff2.h"
#include "ouch.h"
#include "player.h"
#include "stuff.h"
#include "view.h"

void mons_pickup(int i);
void plant_spit(int i, struct bolt beam [1]);
void monster_move(int i);
void mons_in_cloud(int i);
char mons_speaks(int i);

char mmov_x, mmov_y;

char curse_an_item(char which, char power)
{
/* use which later, if I want to curse weapon/gloves whatever
   which, for now: 0 = non-mummy, 1 = mummy (potions as well)
   don't change mitm.idam of !odecay */

/* I think this is wrong??? */

int possib [52];
int cu = power;
cu = 0;
int cu1 = 0;
int cu2 = 0;
int cu3 = 0;


for (cu = 0; cu < 52; cu++)
{
  possib [cu] = 100;
}

for (cu = 0; cu < 52; cu++)
{
  if (you[0].inv_quant [cu] == 0) continue;

  cu1 = you[0].inv_class [cu];

/*  if (cu1 == OBJ_WEAPONS || cu1 == OBJ_ARMOUR)
  {
   if (you[0].inv_dam [cu] % 30 >= 25) continue; / * no randarts * /
  }*/

  if (cu1 == OBJ_JEWELLERY)
  {
   if (you[0].inv_dam [cu] == 200) continue; /* no randarts */
  }

  if (cu1 == OBJ_WEAPONS || cu1 == OBJ_ARMOUR || cu1 == OBJ_JEWELLERY)
  {
   if (you[0].inv_plus [cu] >= 130) continue;
   possib [cu2] = cu;
   cu2++;
  }

  if (cu1 == OBJ_POTIONS && which == 1)
  {
    if (you[0].inv_type [cu] == POT_DECAY) continue;
    possib [cu2] = cu;
    cu2++;
  }

}

if (cu2 == 0) return 0;

        do
           {
              cu3 = random2(cu2);
           } while (possib [cu3] == 100);


if (you[0].inv_class [possib [cu3]] == OBJ_POTIONS)
{

  you[0].inv_type [possib [cu3]] = POT_DECAY; /* don't change you[0].inv_dam (just for fun) */
  return 1;
  /* problem: changes large piles of potions */
}

you[0].inv_plus [possib [cu3]] += 100;

return 1;

}


void monster_blink(int mn)
{

/* I can't be bothered writing an intelligent function, so I'll make it ugly: */

if (menv [mn].m_class == MONS_TUNNELING_WORM || menv [mn].m_class == MONS_WORM_TAIL) return; /* Do not let this happen! */

int passed [2];

if (random_near_space(passed) == 0) return;

mgrd [menv [mn].m_x] [menv [mn].m_y] = MNG;

menv [mn].m_x = passed [0];
menv [mn].m_y = passed [1];

mgrd [menv [mn].m_x] [menv [mn].m_y] = mn;

}

char random_near_space(int passed [2])
{

passed [0] = 0;
passed [1] = 0;

int blx = passed [0];
int bly = passed [1];
int tries = 0;

do
{

blx = you[0].x_pos - 6 + random2(14);
bly = you[0].y_pos - 6 + random2(14);

tries ++;
if (tries >= 150) return 0;
} while (see_grid(blx, bly) == 0 || grd [blx] [bly] < 65 || mgrd [blx] [bly] != MNG || (you[0].x_pos == blx && you[0].y_pos == bly));

passed [0] = blx;
passed [1] = bly;

return 1;
}



int distance(int x1, int x2, int y1, int y2)
{
// not accurate (at all), but its main purpose is to see whether you're next to something or not.
int xdisp = x1 - x2;
if (xdisp < 0) xdisp *= -1;

int ydisp = y1 - y2;
if (ydisp < 0) ydisp *= -1;

if (xdisp == 1 && ydisp == 1) ydisp = 0;
//itoa(xdisp + ydisp, st_prn, 10);
//strcpy(info, st_prn);
//mpr(info);

return xdisp + ydisp;

}



void swap_places(int swap)
{

 mpr("You swap places.");

//grd [mons_x [swap]] [mons_y [swap]] -= 50;
mgrd [menv [swap].m_x] [menv [swap].m_y] = MNG;

menv [swap].m_x = you[0].x_pos;
menv [swap].m_y = you[0].y_pos;

//grd [mons_x [swap]] [mons_y [swap]] += 50;
mgrd [menv [swap].m_x] [menv [swap].m_y] = swap;

} // end of swap_places



void print_wounds(int wounded)
{

if (menv[wounded].m_class == -1) return;

if (menv[wounded].m_class == MONS_SMALL_ZOMBIE || menv[wounded].m_class == MONS_BIG_ZOMBIE || menv[wounded].m_class == MONS_SMALL_SKELETON || menv[wounded].m_class == MONS_LARGE_SKELETON || menv[wounded].m_class == MONS_RAKSHASA || menv[wounded].m_class == MONS_FAKE_RAKSHASA)
        return;

if (menv[wounded].m_ench [2] == 6 && player_see_invis() == 0) return;

if (mons_near(wounded) == 0) return;

if (menv[wounded].m_hp == menv[wounded].m_hp_max || menv[wounded].m_hp <= 0) return;

strcpy(info, monam(menv[wounded].m_sec,menv[wounded].m_class, menv [wounded].m_ench [2], 0));
strcat(info, " is");

if (menv[wounded].m_hp <= menv[wounded].m_hp_max / 6)
{
 strcat(info, " almost");
 if (wounded_damaged(menv [wounded].m_class) == 0)
  strcat(info, " dead.");
   else  strcat(info, " destroyed.");
   mpr(info);
 return;
}

if (menv[wounded].m_hp <= menv[wounded].m_hp_max / 4)
{
 strcat(info, " horribly ");
 goto wounded_print;
}

if (menv[wounded].m_hp <= menv[wounded].m_hp_max / 3)
{
 strcat(info, " heavily ");
 goto wounded_print;
}

/*if (menv[wounded].m_hp <= menv[wounded].m_hp_max / 2)
{
 strcat(info, " somewhat ");
 goto wounded_print;
}*/

if (menv[wounded].m_hp <= (menv[wounded].m_hp_max / 4) * 3)
{
 strcat(info, " moderately ");
 goto wounded_print;
}

strcat(info, " lightly ");

wounded_print:
if (wounded_damaged(menv [wounded].m_class) == 0)
 strcat(info, "wounded.");
  else strcat(info, "damaged.");
mpr(info);
}


int wounded_damaged(int wound_class)
{
 if (mons_holiness(wound_class) == MH_UNDEAD) return 1;
 if (wound_class >= MONS_CLAY_GOLEM && wound_class <= MONS_TOENAIL_GOLEM) return 1;
 if (wound_class == MONS_FUNGUS || wound_class == MONS_SMALL_ABOMINATION || wound_class == MONS_PLANT || wound_class == MONS_FIRE_VORTEX || wound_class == MONS_LARGE_ABOMINATION || wound_class == MONS_DANCING_WEAPON || wound_class == MONS_SPATIAL_VORTEX)
   return 1; // 'damaged'

 return 0; // 'wounded'
}



void monster(void)
{
char bat = 0;
int monc = 0;
int mzap = 0;

int msecc = 0;
char brkk = 0;
int i, j, hand_used, spell_cast;

struct bolt beem [1];
int func_pass [10];

int show [19] [19];

losight(show, grd, you[0].x_pos, you[0].y_pos);

int no_mons = 0;

for (i = 0; i < MNST; i++)
{

if (menv [i].m_class != -1) no_mons ++;

if (menv [i].m_class != -1 && menv [i].m_class != MONS_WORM_TAIL)
{

monc ++;

if (menv [i].m_hp > menv [i].m_hp_max) menv [i].m_hp = menv [i].m_hp_max;

if (menv [i].m_class == MONS_GIANT_SPORE && menv [i].m_hp < 1)
{
	for (j = 0; j < 3; j++)
		{
			menv [i].m_ench [j] = 0;
		}

	menv [i].m_ench_1 = 0;
	no_mons --;
	menv [i].m_class = -1;
 mgrd [menv [i].m_x] [menv [i].m_y] = MNG;
	spore_goes_pop(i); /* has to be after the above, so that spore isn't killed twice. */
	continue;
} /* end of if giant spore */


if (you[0].haste == 0) menv [i].m_speed_inc += (menv [i].m_speed * you[0].time_taken / 10);
	else menv [i].m_speed_inc += (menv [i].m_speed * (you[0].time_taken / 2) / 10);

if (you[0].slow > 0) menv [i].m_speed_inc += (menv [i].m_speed * you[0].time_taken / 10);



while (menv [i].m_speed_inc >= 80) /* The continues & breaks are WRT this. */
{

if (menv [i].m_class != -1 && menv [i].m_hp <= 0)
{
	monster_die(i, 5, 0);
	if (menv [i].m_class != MONS_GIANT_SPORE) continue;
}

menv [i].m_speed_inc -= 10;

if (env[0].cgrid [menv [i].m_x] [menv [i].m_y] != CNG)
{
 if (menv [i].m_class >= MLAVA0 && menv [i].m_sec == 1) break;
 if (menv [i].m_class == -1) break; /* problem with vortices */
	mons_in_cloud(i);
	if (menv [i].m_class == -1)
	{
		menv [i].m_speed_inc = 1;
		break;
	}
}




if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) != 1 && menv [i].m_targ_1_x == menv [i].m_x && menv [i].m_targ_1_y == menv [i].m_y)
	menv [i].m_beh = BEH_WANDER;

if (menv [i].m_class == MONS_GLOWING_SHAPESHIFTER) menv [i].m_ench [1] = 38;
if (menv [i].m_class == MONS_SHAPESHIFTER) menv [i].m_ench [1] = 39; /* otherwise are potential problems with summonings */

switch (menv [i].m_beh)
{
case BEH_SLEEP:
menv [i].m_targ_1_x = menv [i].m_x;
menv [i].m_targ_1_y = menv [i].m_y;
break;

case BEH_CHASING_II:
case BEH_CHASING_I: // chasing YOU
if ((mons_near(i) == 1 && (you[0].invis == 0 || mons_see_invis(menv [i].m_class) != 0 || (grd [you[0].x_pos] [you[0].y_pos] == 65 && you[0].lev == 0))) || (mons_near(i) == 0 && menv [i].m_targ_1_x == menv [i].m_x && menv [i].m_targ_1_y == menv [i].m_y))
{
	menv [i].m_targ_1_x = you[0].x_pos;
	menv [i].m_targ_1_y = you[0].y_pos;
}
break;

case BEH_WANDER: if ((menv [i].m_targ_1_x == menv [i].m_x && menv [i].m_targ_1_y == menv [i].m_y) || random2(20) == 0)
{
	menv [i].m_targ_1_x = random2(80);
	menv [i].m_targ_1_y = random2(70);
}
break; // wandering

case BEH_FLEE: // fleeing
if (mons_near(i) == 1 && (you[0].invis == 0 || mons_see_invis(menv [i].m_class) > 0))
{
	menv [i].m_targ_1_x = you[0].x_pos;
	menv [i].m_targ_1_y = you[0].y_pos;
}
if (menv [i].m_hp > (menv [i].m_hp_max / 2)) menv [i].m_beh = BEH_CHASING_I;
/* change the '= 1' to whatever monster used to be doing */
break;

/* 4 is used by confusion */

case BEH_FIGHT: // attacking other monster
     if (menv [i].m_hit == MHITNOT)
     {
       menv [i].m_beh = BEH_WANDER; // ?
       break;
     }
     menv [i].m_targ_1_x = menv [menv [i].m_hit].m_x;
     menv [i].m_targ_1_y = menv [menv [i].m_hit].m_y;
break;

case BEH_ENSLAVED: // friendly
/*To be nice (although not strictly accurate) they
  should follow you even when they can't see you. */
if (you[0].pet_target != MHITNOT)
{
 if (menv [i].m_hit == MHITNOT)
 {
  menv [i].m_hit = you[0].pet_target;
 }
}

if (menv [i].m_hit == i || menv [menv [i].m_hit].m_beh == BEH_ENSLAVED) menv [i].m_hit = MHITNOT;
/* I don't know why I have to do that. */

if (menv [i].m_hit != MHITNOT && ((mons_near(i) == 1 && mons_near(menv [i].m_hit) == 1) || distance(menv [i].m_x, menv [menv [i].m_hit].m_x, menv [i].m_y, menv [menv [i].m_hit].m_y) == 1))
{
  menv [i].m_targ_1_x = menv [menv [i].m_hit].m_x;
  menv [i].m_targ_1_y = menv [menv [i].m_hit].m_y;
}
 else
    if (mons_near(i) == 1 && ((menv [i].m_targ_1_x == menv [i].m_x && menv [i].m_targ_1_y == menv [i].m_y) || random2(3) == 0))
    {
     if (random2(7) == 0)
      {
        menv [i].m_targ_1_x = random2(80);
       	menv [i].m_targ_1_y = random2(70);
      } else
             {
              menv [i].m_targ_1_x = you[0].x_pos;
              menv [i].m_targ_1_y = you[0].y_pos;
             }
    }
    if (mons_near(i) == 0)
    {
       menv [i].m_targ_1_x = you[0].x_pos;
       menv [i].m_targ_1_y = you[0].y_pos;
    }
break;

} // end of switch (mons_beh)


int p;


if (menv [i].m_ench_1 != 0)
{

for (p = 0; p < 3; p ++)
{
switch (menv [i].m_ench [p])
{
	case 1: // slow
	if (random2(250) <= menv [i].m_HD + 10)
	{
		if (menv [i].m_speed >= 100)
		{
			menv [i].m_speed = ((menv [i].m_speed - 100) * 2) + 100;
		} else menv [i].m_speed *= 2;
		menv [i].m_ench [p] = 0;
		if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
			menv [i].m_ench_1 = 0;
	}
	break;

	case 2: // haste
	if (random2(40) == 0)
	{
		if (menv [i].m_speed >= 100)
		{
			menv [i].m_speed = ((menv [i].m_speed - 100) / 2) + 100;
		} else menv [i].m_speed /= 2;
		menv [i].m_ench [p] = 0;
		if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
			menv [i].m_ench_1 = 0;
	}
	break;

	case 4: // fear
	menv [i].m_beh = BEH_FLEE;
	if (random2 (150) <= menv [i].m_HD + 10)
	{
	menv [i].m_beh = BEH_CHASING_I; // reset to monster's original behaviour
	menv [i].m_ench [p] = 0;
	if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
			menv [i].m_ench_1 = 0;
	}
	break;


	case 5: // confusion
	menv [i].m_beh = BEH_CONFUSED;
	if (random2 (80) < menv [i].m_HD + 10)
	{
  if (menv [i].m_class == MONS_BUTTERFLY || menv [i].m_class == MONS_FIRE_VORTEX || menv [i].m_class == MONS_SPATIAL_VORTEX || menv [i].m_class == MONS_VAPOUR) continue; // butterfly, fire vortex
		menv [i].m_beh = BEH_CHASING_I;
		menv [i].m_ench [p] = 0;
		if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
		menv [i].m_ench_1 = 0;
	}
	break;


	case 6: // invisibility
	if (random2 (40) == 0 || (menv [i].m_class >= MLAVA0 && menv [i].m_sec == 0) || (menv [i].m_class == 125 && random2(3) == 0))
	{
		if (mons_flag(menv [i].m_class, M_INVIS)) continue; /* never runs out for these creatures */
		if (menv [i].m_class >= MLAVA0 && menv [i].m_sec == 1) continue;
		menv [i].m_ench [p] = 0;
		if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
		menv [i].m_ench_1 = 0;
	}
	break;



   case 57: // poison
   case 58: // worse poison
   case 59: // worser poison
   case 60: // worsest poison
   case 7: // your poison
   case 8: // your worse poison
   case 9: // your worser poison
   case 10: // your worsest poison
   if (random2(2) == 0) menv [i].m_hp -= random2((menv [i].m_ench [p] % 50) - 3) + 1;
   if (mons_res_poison(menv [i].m_class) < 0) menv [i].m_hp -= random2((menv [i].m_ench [p] % 50) - 5) + random2((menv [i].m_ench [p] % 50) - 5) + 1;
   if (menv [i].m_hp <= 0)
   {
      if (menv [i].m_ench [p] < 50) monster_die(i, 1, 0); // ie you
         else monster_die(i, 5, 0);
      goto endbat;
   }
   if (random2(5) == 0) //30 - mons_HD [i]) <= 0) // LOOK AT THIS!!!
   {
      menv [i].m_ench [p] --; //= 0;
      if (menv [i].m_ench [p] % 50 == 6)
      {
	menv [i].m_ench [p] = 0;
	if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
        {
        		   menv [i].m_ench_1 = 0;
        }
      }
   }
   break;

   /* assumption: mons_res_fire has already been checked */
   case 31: // sticky flame
   case 32: // sticky flame
   case 33: // sticky flame
   case 34: // sticky flame
   // other monster's:
   case 61: // sticky flame
   case 62: // sticky flame
   case 63: // sticky flame
   case 64: // sticky flame
   menv [i].m_hp -= ((random2(4) + random2(4) + 1) * 10) / menv [i].m_speed;
   if (mons_res_fire(menv [i].m_class) == -1) menv [i].m_hp -= ((random2(5) + random2(5) + 1) * 10) / menv [i].m_speed;
   if (mons_near(i) == 1)
   {
    strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
    strcat(info, " burns!");
    mpr(info);
   }
   if (menv [i].m_hp <= 0)
   {
      if (menv [i].m_ench [p] < 60) monster_die(i, 1, 0); // ie you
         else monster_die(i, 5, 0);
      goto endbat;
   }
   if (random2(2) == 0)
   {
      menv [i].m_ench [p] --; //= 0;
      if (menv [i].m_ench [p] == 30 || menv [i].m_ench [p] == 60)
      {
      		if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
        {
        		   menv [i].m_ench_1 = 0;
        }
      }
   }
   break;

/* 19 is taken by summoning:
   If these are changed, must also change abjuration */
   case 25:
   case 24:
   case 23:
   case 22:
   case 21:
   case 20:
   if (random2(10) == 0)
   {
     menv [i].m_ench [p] --;
   }
   if (menv [i].m_ench [p] == 19)
   {
     monster_die(i, 6, 0);
     goto endbat;
   }
   break;

 case 30: // charm monster
	if (menv [i].m_beh != BEH_ENSLAVED || random2 (500) <= menv [i].m_HD + 10)
	{
	menv [i].m_beh = BEH_CHASING_I; // reset to monster's original behaviour
	menv [i].m_ench [p] = 0;
	if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
			menv [i].m_ench_1 = 0;
	}
 break;

 case 38: // monster is a glowing shapeshifter. This ench never runs out.
 if (random2(4) == 0 || menv [i].m_class == MONS_GLOWING_SHAPESHIFTER) monster_polymorph(i, 250, 0);
 break;
 case 39: // monster is a shapeshifter. This ench never runs out.
 if (random2(15) == 0 || menv [i].m_class == MONS_SHAPESHIFTER) monster_polymorph(i, 250, 0);
 break;

 case 40:
        monster_teleport(i, 1);
	menv [i].m_ench [p] = 0;
	if (menv [i].m_ench [0] == 0 && menv [i].m_ench [1] == 0 && menv [i].m_ench [2] == 0)
			menv [i].m_ench_1 = 0;
 break;

 case 41:
 case 42:
 case 43:
 menv [i].m_ench [p] --;
 break;

} // end of switch
} // end of for p
} // end of if

/* regenerate */
if (menv [i].m_hp < menv [i].m_hp_max && (random2(25) == 0 || menv [i].m_class == MONS_IMP || menv [i].m_class == MONS_TROLL || menv [i].m_class == MONS_SLIME_CREATURE || menv [i].m_class == MONS_ROCK_TROLL || menv [i].m_class == MONS_IRON_TROLL || menv [i].m_class == MONS_CACODEMON || menv [i].m_class == MONS_LEMURE || menv [i].m_class == MONS_HELLWING || menv [i].m_class == MONS_ANITA || menv [i].m_class == MONS_DEEP_TROLL)) menv [i].m_hp ++;


if (menv [i].m_speed >= 100) continue;

if (menv [i].m_class == MONS_SMALL_ZOMBIE || menv [i].m_class == MONS_BIG_ZOMBIE || menv [i].m_class == MONS_SMALL_SKELETON || menv [i].m_class == MONS_LARGE_SKELETON) menv [i].m_hp_max = menv [i].m_hp;


if (igrd [menv [i].m_x] [menv [i].m_y] != 501 && (gmon_use [menv [i].m_class] == 3 || menv [i].m_class == MONS_JELLY || menv [i].m_class == MONS_NECROPHAGE || menv [i].m_class == MONS_GHOUL))
{
	mons_pickup(i);
}

if (menv [i].m_beh == BEH_FLEE) // fleeing
{
if (menv [i].m_targ_1_x < menv [i].m_x) mmov_x = 1; else mmov_x = 0;
if (menv [i].m_targ_1_x > menv [i].m_x) mmov_x = -1;
if (menv [i].m_targ_1_y < menv [i].m_y) mmov_y = 1; else mmov_y = 0;
if (menv [i].m_targ_1_y > menv [i].m_y) mmov_y = -1;
} else
      {
	if (menv [i].m_targ_1_x < menv [i].m_x) mmov_x = -1; else mmov_x = 0;
	if (menv [i].m_targ_1_x > menv [i].m_x) mmov_x = 1;
	if (menv [i].m_targ_1_y < menv [i].m_y) mmov_y = -1; else mmov_y = 0;
	if (menv [i].m_targ_1_y > menv [i].m_y) mmov_y = 1;
      }

if (mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] != MNG && mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] != i)
{
 int mnt = mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y];
 if (menv [i].m_beh == BEH_ENSLAVED && menv [mnt].m_beh != BEH_ENSLAVED) menv [i].m_hit = mnt;
 if (menv [i].m_beh != BEH_ENSLAVED && menv [mnt].m_beh == BEH_ENSLAVED) menv [i].m_hit = mnt;
}


brkk = 0;

if (menv [i].m_beh == BEH_CONFUSED) /* confused */
{
	mmov_x = random2(3) - 1;
	mmov_y = random2(3) - 1;
	if (mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] != MNG && (mmov_x != 0 || mmov_y != 0))
	{
			if (monsters_fight(i, mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y]) == 1)
   {
 			mmov_x = 0; mmov_y = 0;
	 		brkk = 1;
		 	break;
   }
	}
} /* end of if confused */
if (brkk == 1) continue;

/* for monsters who use spec abils even when next to you */
if (mons_near(i) == 1 && menv [i].m_beh != BEH_SLEEP)
{

if (mons_flag(menv [i].m_class,M_SPEAKS) && random2(5) == 0)
{
 mons_speaks(i);
}


switch(menv [i].m_class)
{

case MONS_SPATIAL_VORTEX:
case MONS_KILLER_KLOWN: /* Killer Klown */
menv [i].m_sec = random2(15) + 1;
break;

case MONS_GIANT_EYEBALL:
if (random2(2) == 0 && (menv [i].m_ench [2] != 6 || player_see_invis() != 0))
{
  mpr("The camera stares at you.");
  if (you[0].paralysis < 10) you[0].paralysis += 2 + random2(3);
}
break;

case MONS_EYE_OF_DRAINING:
if (random2(2) == 0 && (menv [i].m_ench [2] != 6 || player_see_invis() != 0))
{
  mpr("The camera of draining stares at you.");
  you[0].ep -= random2(5) + random2(5) + random2(5) + 2;
  if (you[0].ep < 0) you[0].ep = 1;
  you[0].ep_ch = 1;
  menv [i].m_hp += 5;
  if (menv [i].m_hp >= menv [i].m_hp_max) menv [i].m_hp = menv [i].m_hp_max;
}
break;

case MLAVA0:
case MLAVA1:
case MLAVA2:
case MLAVA3:
case MLAVA4:
case MWATER0:
case MWATER1:
case MWATER2:
case MWATER3:
case MWATER4:
case MWATER5:
if (grd [menv [i].m_x] [menv [i].m_y] == 65 || grd [menv [i].m_x] [menv [i].m_y] == 200)
{
 menv [i].m_sec = 0;
 menv [i].m_ench [2] = 0;
} else
 if (menv [i].m_sec == 0 && grd [menv [i].m_x] [menv [i].m_y] != 65 && grd [menv [i].m_x] [menv [i].m_y] != 200) // visible
 {
  if (random2(5) == 0)
  {
   menv [i].m_sec = 1;
   menv [i].m_ench [2] = 6;
   menv [i].m_ench_1 = 1;
  }
 } else
{
 if (random2(10) == 0 && (menv [i].m_x != you[0].x_pos || menv [i].m_y != you[0].y_pos)) menv [i].m_sec = 0;
}
break;

case MONS_AIR_ELEMENTAL: // air elemental
 if (random2(5) == 0)
 {
  menv [i].m_ench [2] = 6;
  menv [i].m_ench_1 = 1;
 }
break;

case MONS_PANDEMONIUM_DEMON: /* random demon */
if (ghost.ghs [13] == 1) menv [i].m_sec = random2(15) + 1;
break;

} // end of switch

} // end of if mons_near



// ATTACK WAS HERE!!!








if (((menv [i].m_beh == BEH_CHASING_I || menv [i].m_beh == BEH_FLEE) && mons_near(i) == 1) || ((menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED) && menv [i].m_hit != MHITNOT))
{

if ((menv [i].m_beh == BEH_CHASING_I || menv [i].m_beh == BEH_FLEE) && mons_near(i) == 1) you[0].pet_target = i;

if ((menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED) && (menv [i].m_hit == MHITNOT || menv [i].m_hit == MHITYOU))
{
 goto end_switch;
// break;
}

if (menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED)
{
	beem[0].trac_targ = menv [i].m_hit;
	if (menv [i].m_hit == MHITNOT)
	{
	 beem[0].trac_targ_x = menv [i].m_x;
	 beem[0].trac_targ_y = menv [i].m_y;
        } else
	{
	 beem[0].trac_targ_x = menv [menv [i].m_hit].m_x;
	 beem[0].trac_targ_y = menv [menv [i].m_hit].m_y;
	}
} else
{
 beem[0].trac_targ = MHITYOU;
	beem[0].trac_targ_x = you[0].x_pos;
	beem[0].trac_targ_y = you[0].y_pos;
}

if (beem[0].trac_targ == MHITYOU && mons_near(i) == 0 && menv [i].m_class != MONS_CACODEMON) goto end_switch; //continue;


switch(menv [i].m_class)
{

case MLAVA2:
if (menv [i].m_beh == BEH_CONFUSED) break;
if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) == 0) break;
if (random2(2) == 0 || menv [i].m_sec == 1 || menv [i].m_ench [2] == 6) break;
// viewwindow was here.
if (show [menv [i].m_x - you[0].x_pos + 6] [menv [i].m_y - you[0].y_pos + 6] != 0)
{
beem[0].move_x = 0; beem[0].move_y = 0;

beem[0].move_x = beem[0].trac_targ_x - menv [i].m_x;
beem[0].move_y = beem[0].trac_targ_y - menv [i].m_y;
if (beem[0].move_x != 0 || beem[0].move_y != 0)
{
viewwindow(1);
beem[0].target_x = beem[0].trac_targ_x;
beem[0].target_y = beem[0].trac_targ_y;
beem[0].aim_down = 1;
	beem[0].range = random2(10) + 5;
	beem[0].source_x = menv [i].m_x; beem[0].source_y = menv [i].m_y;
		strcpy(beem[0].beam_name, "glob of radioactive waste");
		beem[0].damage = 108;
		beem[0].colour = RED;
		beem[0].type = '#'; /* hash # */
		beem[0].flavour = 20; /* lava */
		beem[0].hit = 20;
		strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
		strcat(info, " spits radioactive waste!");
                beem[0].beam_source = i;
		mpr(info);
	beem[0].thing_thrown = 4;
	missile(beem, 0);
}
}
break;

case MWATER2:
if (menv [i].m_beh == BEH_CONFUSED) break;
if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) == 0) break;
if (menv [i].m_sec == 1 || menv [i].m_ench [2] == 6) break;
if (show [menv [i].m_x - you[0].x_pos + 6] [menv [i].m_y - you[0].y_pos + 6] != 0)
{
beem[0].move_x = 0; beem[0].move_y = 0;
beem[0].move_x = beem[0].trac_targ_x - menv [i].m_x;
beem[0].move_y = beem[0].trac_targ_y - menv [i].m_y;
if (beem[0].move_x != 0 || beem[0].move_y != 0)
{
beem[0].target_x = beem[0].trac_targ_x;
beem[0].target_y = beem[0].trac_targ_y;
beem[0].aim_down = 1;
viewwindow(1);
	beem[0].range = random2(10) + 5;
	beem[0].source_x = menv [i].m_x; beem[0].source_y = menv [i].m_y;
		strcpy(beem[0].beam_name, "bolt of electricity");
		beem[0].damage = 105;
		beem[0].colour = LIGHTCYAN;
		beem[0].type = 35; // hash #
		beem[0].flavour = 5; // elec
		beem[0].hit = 150;
                beem[0].beam_source = i;
		strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0)); //gmon_name [mons_class [i]]);
		strcat(info, " shoots out a bolt of electricity!");
		mpr(info);
	beem[0].thing_thrown = 2;
	beam(beem);
}
}
break;

case MONS_OKLOB_PLANT:
if (menv [i].m_beh == BEH_CONFUSED) break;
if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) == 0) break;
if (random2(3) == 0) // o plant
{
  plant_spit(i, beem);
}
break;

case MONS_PIT_FIEND: // Pit Fiend
if (random2(3) == 0) break;
case MONS_FIEND:
if (menv [i].m_beh == BEH_CONFUSED) break;
if (random2(4) == 0) // fiend!
{
	switch(random2(4))
	{
		case 0:
                   spell_cast = 48;
                   mons_cast(i, beem, spell_cast); /* beem should be irrelevant here */
                   break;
		case 1:
		case 2:
		case 3:
  tracer_f(i, beem);
  if (menv [i].m_beh == BEH_ENSLAVED && (beem[0].tracer == 1 || beem[0].tracer == 2)) break;
		if (beem[0].tracer != 0) /* || (beem[0].trac_targ != MHITYOU && beem[0].trac_hit_mons != 0)) */
		{
   if (menv [i].m_ench [2] != 6)
   {
   	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   	strcat(info, " makes a gesture!");
   	mpr(info);
   }
 		spell_cast = 49;
			mons_cast(i, beem, spell_cast);
		}
		break;

	}

	mmov_x = 0; mmov_y = 0;

}
break;


case MONS_IMP: // imp
case MONS_PHANTOM: // phantom
case MONS_INSUBSTANTIAL_WISP:
case MONS_BLINK_FROG: // blink frog
if (random2(7) == 0) // phantom
{
   if (mons_near(i) == 1)
   {
    strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
    strcat(info, " jumps.");
    mpr(info);
   }
   monster_blink(i);
}
break;



//case 68:
//place_cloud(101, menv [i].m_x, menv [i].m_y, 2 + random2(4));
//break;






case MONS_MANTICORE:
if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) == 0) break;
if (menv [i].m_beh == BEH_CONFUSED) break;
if (menv [i].m_inv [1] != 501) // manticore
{
		hand_used = menv [i].m_inv [1];
		beem[0].range = 6;
		strcpy(beem[0].beam_name, item_name(mitm.iplus2 [hand_used], mitm.iclass [hand_used], mitm.itype [hand_used], mitm.idam [hand_used], mitm.iplus [hand_used], 1, mitm.iid [hand_used], 6).c_str());
		mons_throw(i, beem, hand_used);
		//mmov_x = 0;
/*		if (brek == 1)
		{
			brek = 0;
			continue;
		}*/
		continue; /* ???? */
}
break;


// dragon breath weapon:
case MONS_DRAGON:
case MONS_HELL_HOUND: // hellhound
case MONS_ICE_DRAGON:
case MONS_LINDWORM: // lindworm
case MONS_FIREDRAKE: // firedrake
case MONS_XTAHUA:// Xtahua
if (menv [i].m_beh == BEH_CONFUSED) break;
if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) == 0) break;
if ((menv [i].m_class != MONS_HELL_HOUND && random2(13) < 3) || random2(10) == 0)
{
if (menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED)
{
 beem[0].trac_targ = menv [i].m_hit;
 tracer_f(i, beem);

 if (beem[0].tracer == 0) break;
 if (menv [i].m_beh == 7 && (beem[0].tracer == 1 || beem[0].tracer == 2)) break;
 if (menv [i].m_beh == 7 && beem[0].tracer == 4) break;
 if (beem[0].tracer == 3 && beem[0].tracer_mons == 0 && menv [i].m_beh != BEH_ENSLAVED) break;

// if (beem[0].trac_hit_mons == 0) break;
}
dragon(i, beem);
mmov_x = 0;
//mmov_y = 0;
continue;
//break;
} // end of dragon breathe
break;

} // end switch??



if (menv [i].m_inv [4] != 501 && random2(3) == 0 && menv [i].m_beh != BEH_SLEEP)
{
 switch(mitm.itype [menv [i].m_inv [4]])
 {
  case 0:
  case 1:
  if (menv [i].m_hp > menv [i].m_hp_max / 2) goto out_of_potion;
  if (mons_near(i) != 0)
  {
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " drinks a vial.");
   mpr(info);
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " is healed!");
   mpr(info);
  }
  if (mitm.itype [menv [i].m_inv [4]] == 1) menv [i].m_hp += random2(10) + random2(10) + random2(10) + 10;
   menv [i].m_hp += random2(7) + 5;
  if (menv [i].m_hp > menv [i].m_hp_max) menv [i].m_hp = menv [i].m_hp_max;
  break;

  case 2:
  beem[0].colour = 1;
  case 12:
  if (mitm.itype [menv [i].m_inv [4]] == 12) beem[0].colour = 5;
  if (mons_near(i) != 0)
  {
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " drinks a vial.");
   mpr(info);
   mons_ench_f2(i, 1, func_pass, beem);
  } else goto out_of_potion;
  break;

  default: goto out_of_potion;
 }

 mitm.iquant [menv [i].m_inv [4]] --;
 if (mitm.iquant [menv [i].m_inv [4]] <= 0) destroy_item(menv [i].m_inv [4]);
 continue;

}



out_of_potion : if (menv [i].m_inv [6] != 501 && random2(3) == 0 && menv [i].m_beh != BEH_CONFUSED && menv [i].m_beh != BEH_SLEEP)
{
 switch(mitm.itype [menv [i].m_inv [6]])
 {
  case 1:
  if (menv [i].m_ench [0] == 40 || menv [i].m_ench [1] == 40 || menv [i].m_ench [2] == 40) goto out_of_scroll;
  if (menv [i].m_beh != BEH_FLEE) goto out_of_scroll;
  if (mons_near(i) != 0)
  {
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " uses a device.");
   mpr(info);
  }
  monster_teleport(i, 0);
  break;

  case 14:
  if (menv [i].m_beh != BEH_FLEE) goto out_of_scroll;
  if (mons_near(i) != 0)
  {
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " uses a device.");
   mpr(info);
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " jumps!");
   mpr(info);
   monster_blink(i);
  } else goto out_of_scroll;
  break;

  case 6:
  if (mons_near(i) != 1) goto out_of_scroll;
  if (mons_near(i) != 0)
  {
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " uses a device.");
   mpr(info);
  }
  create_monster(MONS_LARGE_ABOMINATION, 21, menv [i].m_beh, menv [i].m_x, menv [i].m_y, menv [i].m_hit, 250);
  break;

  default: goto out_of_scroll;
 }

 mitm.iquant [menv [i].m_inv [6]] --;
 if (mitm.iquant [menv [i].m_inv [6]] <= 0) destroy_item(menv [i].m_inv [6]);
 continue;

}

out_of_scroll : if (menv [i].m_inv [5] != 501 && random2(2) == 0 && menv [i].m_beh != BEH_SLEEP)
{
 if (mitm.iplus [menv [i].m_inv [5]] > 0)
 {
/*  switch(menv [i].m_beh)
  {
*/
   switch(mitm.itype [menv [i].m_inv [5]])
   {
     case WAND_HASTING: // hasting
     if (menv [i].m_ench [0] == 2 || menv [i].m_ench [1] == 2 || menv [i].m_ench [2] == 2) goto out_of_zap;
     beem[0].target_x = menv [i].m_x;
     beem[0].target_y = menv [i].m_y;
     break;

     case WAND_HEALING: // healing
     if (menv [i].m_hp > menv [i].m_hp_max / 2) goto out_of_zap;
     beem[0].target_x = menv [i].m_x;
     beem[0].target_y = menv [i].m_y;
     break;

     case WAND_INVISIBILITY: // invis
     if (menv [i].m_ench [2] == 6) goto out_of_zap;
     beem[0].target_x = menv [i].m_x;
     beem[0].target_y = menv [i].m_y;
     break;

     case WAND_FIREBALL: // fireball - too risky
     case WAND_DIGGING: // digging - I must do this sometime
     goto out_of_zap;

     case WAND_TELEPORTATION: // teleport
     if (menv [i].m_hp > menv [i].m_hp_max / 2) goto out_of_zap;
     if (menv [i].m_ench [0] == 40 || menv [i].m_ench [1] == 40 || menv [i].m_ench [2] == 40) goto out_of_zap;
     beem[0].target_x = menv [i].m_x;
     beem[0].target_y = menv [i].m_y;
     break;


     default:
     switch(menv [i].m_beh)
     {
      case BEH_CHASING_II:
      case BEH_SLEEP:
      if (mons_near(i) == 0) goto out_of_zap;
      default:
      if ((menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED) && menv [i].m_hit == MHITNOT) goto out_of_zap;
      tracer_f(i, beem);
      if (beem[0].tracer == 0) goto out_of_zap;
      if (menv [i].m_beh == BEH_ENSLAVED && (beem[0].tracer == 1 || beem[0].tracer == 2)) goto out_of_zap;
      if (menv [i].m_beh == BEH_ENSLAVED && beem[0].tracer_mons == 4) goto end_throw;
      if (beem[0].tracer == 3 && beem[0].tracer_mons == 0 && menv [i].m_beh != BEH_ENSLAVED) goto out_of_zap;

/*      if (beem[0].tracer == 0 && beem[0].tracer_mons != 3) goto out_of_zap;
      if (beem[0].tracer_mons == 1) goto out_of_zap;*/
//      if (beem[0].tracer_mons == 2 && (mitm.itype [menv [i].m_inv [5]] == 7 || mitm.itype [menv [i].m_inv [5]] == 8 || mitm.itype [menv [i].m_inv [5]] == 14 || mitm.itype [menv [i].m_inv [5]] == 17)) goto out_of_zap;
//      if (beem[0].tracer_mons == 1 && (mitm.itype [menv [i].m_inv [5]] == 7 || mitm.itype [menv [i].m_inv [5]] == 8 || mitm.itype [menv [i].m_inv [5]] == 14 || mitm.itype [menv [i].m_inv [5]] == 17)) goto out_of_zap;
      break;

/*      case 3: goto out_of_zap;

      case 6:
      case 7:
      tracer_f(i, beem);
      if (beem[0].tracer_mons == 0) goto out_of_zap; // crude, and also ineffective
      break;*/

     }
     break;
   }


   switch(mitm.itype [menv [i].m_inv [5]])
   {
    case WAND_FLAME: mzap = 1; break; // flame
    case WAND_FROST: mzap = 2; break; // frost
    case WAND_SLOWING: mzap = 4; break; // slow
    case WAND_HASTING: mzap = 5; break; // haste
    case WAND_MAGIC_DARTS: mzap = 0; break; // mag dart
    case WAND_HEALING: mzap = 13; break; // healing
    case WAND_PARALYSIS: mzap = 3; break; // paralysis
    case WAND_FIRE: mzap = 8; break; // fire
    case WAND_COLD: mzap = 9; break; // cold
    case WAND_CONFUSION: mzap = 6; break; // confusion
    case WAND_INVISIBILITY: mzap = 11; break; // invisibility
    case WAND_DIGGING: goto out_of_zap;
    case WAND_FIREBALL: goto out_of_zap;
//    case 11: mzap = 11; break; // digging
//    case 12: mzap = 11; break; // fireball
    case WAND_TELEPORTATION: mzap = 15; break; // teleportation
    case WAND_LIGHTNING: mzap = 10; break; // lightning
    case WAND_POLYMORPH_OTHER: goto out_of_zap;
    case WAND_ENSLAVEMENT: goto out_of_zap;
//    case 15: mzap = 10; break; // lightning
//    case 16: mzap = 10; break; // lightning
    case WAND_DRAINING: mzap = 19; break; // negative energy
    case WAND_RANDOM_EFFECTS: goto out_of_zap;
//    case 18: random effects

   }

        viewwindow(1);

        beem[0].move_x = beem[0].trac_targ_x - menv [i].m_x;
        beem[0].move_y = beem[0].trac_targ_y - menv [i].m_y;

        beem[0].target_x = beem[0].trac_targ_x;
        beem[0].target_y = beem[0].trac_targ_y;

        beem[0].source_x = menv [i].m_x;
        beem[0].source_y = menv [i].m_y;

        func_pass [8] = 25; // menv [i].m_HD * 7;

        int ufdg = mons_spells(mzap, func_pass, beem[0].beam_name);

        beem[0].ench_power = beem[0].damage; // !!!
        beem[0].colour = func_pass [0];
        beem[0].range = func_pass [1];
        beem[0].damage = func_pass [2];
        beem[0].hit = func_pass [3];
        beem[0].type = func_pass [4];
        beem[0].flavour = func_pass [5];
        beem[0].thing_thrown = func_pass [6];
        beem[0].ench_power = 20; // I think
	beem[0].beam_source = i;

        if (mzap == 5 || mzap == 13 || mzap == 11 || mzap == 15)
        {
        	beem[0].move_x = 0;
               	beem[0].move_y = 0;
        } else
	{
         if (beem[0].move_x == 0 && beem[0].move_y == 0) goto out_of_zap;
	}

        if (mons_near(i) != 0)
        {
	 strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
         strcat(info, " shoots a gun.");
        } else strcpy(info, "You hear a shot.");
        mpr(info);

        mitm.iplus [menv [i].m_inv [5]] --;

        switch(ufdg)
        {
        	case 0: missile(beem, 0); break;
                case 1: beam(beem); break;
        }

/* remember: the monster may well be dead here (eg if it zapped a /o lightning) */


        continue; // finishes the monster's turn

   }

// tracer = 0 = run out of range
// tracer = 1 = hits you in range
// tracer_mons = 0 = hits no monsters
//       "     = 1 = hits monster before you (possibly also after)
//       "     = 2 = hits monster after but not before
//       "     = 3 = hits tamed monster(s) but no others


// remember to use the charge!
} // end zap

/* Don't put stuff here, because of the jump to that label. */

/* SPELLCASTER MOVED HERE */
out_of_zap: if (mons_flag(menv [i].m_class,M_SPELLCASTER) && menv [i].m_beh != BEH_SLEEP)
{
spell_cast = 100;
if (menv [i].m_class == MONS_HELLION)
 switch(random2(3))
 {
 	case 0: menv [i].m_sec = RED; break;
 	case 1: menv [i].m_sec = LIGHTRED; break;
 	case 2: menv [i].m_sec = YELLOW; break;
 }

if (menv [i].m_beh == BEH_CONFUSED && menv [i].m_class != MONS_VAPOUR) goto end_switch; /* vapours cast spells even if confused */

if (you[0].invis != 0 && mons_see_invis(menv [i].m_class) == 0) goto end_switch;

if (random2(200) > 50 + menv[i].m_HD) goto end_switch;
if ((menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED) && menv [i].m_hit == MHITNOT) goto end_switch;

msecc = menv [i].m_sec;
if (menv [i].m_class == MONS_HELLION) msecc = 30; // burning devil
if (menv [i].m_class == MONS_PANDEMONIUM_DEMON)
{
 msecc = 119;
 if (ghost.ghs [9] == 0) goto end_switch; // random demon
}


//beem[0].aim_down = 1;

/*splist [0] = 8; // bolt spell
splist [1] = 6; // enchantment
splist [2] = 11; // self_ench
splist [3] = 0; // misc*/

// tracer = 0 = run out of range
// tracer = 1 = hits you in range
// tracer_mons = 0 = hits no monsters
//       "     = 1 = hits monster before you (possibly also after)
//       "     = 2 = hits monster after but not before
//       "     = 3 = hits tamed monster(s) but no others

int func_pass_2 [6];

func_pass_2 [0] = 100;
func_pass_2 [1] = 100;
func_pass_2 [2] = 100;
func_pass_2 [3] = 100;
func_pass_2 [4] = 100;
func_pass_2 [5] = 100;

mons_spell_list(msecc, func_pass_2);

/*int ifx = 0;

for (ifx = 0; ifx < 6; ifx ++)
{
 itoa(func_pass_2 [ifx], st_prn, 10);
 strcpy(info, st_prn);
 mpr(info);
}*/

if (func_pass_2 [4] == 18 && mons_near(i) == 0 && menv [i].m_beh == BEH_CHASING_I)// && trac_targ == MHITYOU)// && distance(you[0].x_pos, menv [i].m_x, you[0].y_pos, menv [i].m_y) < 40)
{
	spell_cast = 18; // This is EVIL!
	goto casted;
}

if (beem[0].trac_targ == MHITYOU && mons_near(i) == 0) goto end_switch;

/*
Used by monsters in "planning" which spell to cast. Fires off a "tracer"
which tells the monster what it'll hit if it breathes/casts etc.

   tracer = 0 = run out of range or hits a wall
   tracer = 1 = hits you in range
   tracer = 2 = hits friendly monster
   tracer = 3 = hits hostile monster

   tracer_mons = 1 = hits monster specified in trac_targ

   tracer_hit_mons now holds value of mons_see_invis of attacking monster.
   If it's 0, won't register invis monsters or you

   Note: only the missile() function is used for tracers. The tracer code
     in beam() is obsolete and useless.
*/


tracer_f(i, beem);

if (beem[0].tracer == 0) goto end_switch;
if (menv [i].m_beh == BEH_ENSLAVED && (beem[0].tracer == 1 || beem[0].tracer == 2)) goto end_switch;
//if (menv [i].m_beh == BEH_ENSLAVED && beem[0].tracer_mons == 1) goto end_switch;
if (beem[0].tracer == 3 && beem[0].tracer_mons == 0 && menv [i].m_beh != BEH_ENSLAVED) goto end_switch;


if (func_pass_2 [4] == 18) func_pass_2 [4] = 100;

//if (menv [i].m_beh == 7 && beem[0].trac_hit_tamed == 1) goto end_switch;

if (menv [i].m_beh != BEH_FLEE) /* == 3 is fear, I think */
{
// if (beem[0].tracer_mons == 0 && beem[0].tracer == 1 || ((beem[0].trac_targ != MHITYOU && beem[0].trac_hit_mons != 0)))
// if (beem[0].tracer == 1 && beem[0].tracer == 1 || ((beem[0].trac_targ != MHITYOU && beem[0].trac_hit_mons != 0)))
 if ((menv [i].m_beh != BEH_ENSLAVED && (beem[0].tracer == 1 || beem[0].tracer == 2 || beem[0].tracer_mons == 1)) || (menv [i].m_beh == BEH_ENSLAVED && beem[0].tracer == 3))
 {
 	spell_cast = func_pass_2 [random2(5)];
	if (spell_cast == 100) spell_cast = func_pass_2 [random2(5)];
	if (spell_cast == 100) spell_cast = func_pass_2 [random2(5)];
  goto casted;
 }

 if (menv [i].m_beh == BEH_ENSLAVED && beem[0].tracer_mons == 4)
 {
  spell_cast = func_pass_2 [2];
  goto casted;
 }
  if (random2(2) == 0) goto end_switch;
  spell_cast = func_pass_2 [2];
  goto casted;
} // end if
 else
 {
  if (random2(5) == 0) goto end_switch;
 	spell_cast = func_pass_2 [5];
 }

casted : if (spell_cast == 100) goto end_switch;

if (mons_near(i) == 1)
{
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
	if (menv [i].m_class != MONS_BRAIN_WORM) /* brain worm */
	{

		if (menv [i].m_class == MONS_STEAM_DRAGON || menv [i].m_class == MONS_MOTTLED_DRAGON || menv [i].m_class == MONS_STORM_DRAGON || menv [i].m_class == MONS_GOLDEN_DRAGON || menv [i].m_class == MONS_SHADOW_DRAGON || menv [i].m_class == MONS_SWAMP_DRAGON || menv [i].m_class == MONS_SWAMP_DRAKE || menv [i].m_class == MONS_HELL_HOG || menv [i].m_class == MONS_SERPENT_OF_HELL || menv [i].m_class == MONS_QUICKSILVER_DRAGON || menv [i].m_class == MONS_IRON_DRAGON) /* steam, mottled, storm and golden dragons, etc */
		{
			if (menv [i].m_ench [2] != 6)
			{
				strcat(info, " breathes.");
				mpr(info);
			}
		} else

			if (menv [i].m_class == MONS_GREAT_ORB_OF_EYES || menv [i].m_class == MONS_SHINING_EYE || menv [i].m_class == MONS_EYE_OF_DEVASTATION) // great orb of eyes etc
			{
				if (menv [i].m_ench [2] != 6)
				{
					strcat(info, " gazes.");
					mpr(info);
				}
			} else
				if (menv [i].m_class == MONS_VAPOUR) // vapour
				{
					menv [i].m_ench [2] = 6;
				}
				else
					if (menv [i].m_class == MONS_GIANT_ORANGE_BRAIN) // giant orange brain
					{
						if (menv [i].m_ench [2] != 6)
						{
							strcat(info, " pulsates.");
							mpr(info);
						}
					} else
						if (menv [i].m_class == MONS_GERYON)
						{
							strcat(info, " winds a great silver horn.");
							mpr(info);
						} else
							if (menv [i].m_class == MONS_NAGA || menv [i].m_class == MONS_NAGA_WARRIOR)
							{
								strcat(info, " spits poison.");
								mpr(info);
							} else
								if ((menv [i].m_class >= MONS_HELLION && menv [i].m_class <= 90) || menv [i].m_class == MONS_EFREET || (menv [i].m_class >= MONS_WHITE_IMP && menv [i].m_class <= MONS_CACODEMON))
								{
									strcat(info, " gestures.");
									mpr(info);
								} else
									if (menv [i].m_class == MONS_DORGI || menv [i].m_class == MONS_GUARDIAN_ROBOT)
									{
										strcat(info, " fires!");
										mpr(info);
									} else
										if (menv [i].m_class == MONS_SWORD) /* Sword */
										{
											strcat(info, " burns!");
											mpr(info);
										} else {
											if (strstr(monam (menv [i].m_sec, menv [i].m_class, 0, 0), "priest") != NULL) /* various priestly types - assumes they're called 'something priest' */
											{
												strcat(info, " utters an invocation.");
												mpr(info);
											} else
											{
												strcat(info, " executes a program.");
												mpr(info);
											}
										}
	}
/*
itoa(spell_cast, st_prn, 10);
strcpy(info, st_prn);
mpr(info);
*/

} // end of if mons_near
 else if (menv [i].m_class == MONS_GERYON)
        {
         mpr("You hear a weird and mournful sound.");
        }


if (spell_cast == 16)
{
 if (mons_near(i) == 1)
	{
		strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0)); //gmon_name [mons_class [i]]);
		strcat(info, " jumps!");
		mpr(info);
  monster_blink(i);
  continue;
 }
}


 beem[0].damage = 8 * menv [i].m_HD; // really???
 beem[0].beam_source = i;
		mons_cast(i, beem, spell_cast);

	mmov_x = 0;
 beem[0].trac_hit_tamed = 0;
	continue;
}

/* note: are 'goto end_switch's in the spellcaster bit. */

end_switch : if (gmon_use [menv [i].m_class] > 0 && menv [i].m_inv [1] != 501 && menv [i].m_class != MONS_TWO_HEADED_OGRE && (you[0].invis == 0 || mons_see_invis(menv [i].m_class) != 0) && menv [i].m_beh != BEH_CONFUSED && menv [i].m_beh != BEH_SLEEP) // 2-h ogre
{
if ((menv [i].m_beh == BEH_FIGHT || menv [i].m_beh == BEH_ENSLAVED) && menv [i].m_hit == MHITNOT) goto end_throw;

hand_used = menv [i].m_inv [1];
if (random2(10) < 8)
{
	if (mitm.iquant [hand_used] > 0)
	{
  if (beem[0].trac_targ == MHITYOU && beem[0].tracer_mons == 1) goto end_throw;
  if (beem[0].trac_targ == MHITYOU && mons_near(i) == 0) goto end_throw;
  tracer_f(i, beem);
  if (beem[0].tracer == 0) goto end_throw;
  if (menv [i].m_beh == BEH_ENSLAVED && (beem[0].tracer == 1 || beem[0].tracer == 2)) goto end_throw;
  if (beem[0].tracer == 3 && beem[0].tracer_mons == 0 && menv [i].m_beh != BEH_ENSLAVED) goto end_throw;

  if (menv [i].m_beh == BEH_ENSLAVED && beem[0].tracer == 1 && beem[0].tracer_mons != 1) goto end_throw;
  if (menv [i].m_beh == BEH_ENSLAVED && beem[0].tracer_mons == 4) goto end_throw;

		if (beem[0].tracer != 0 || (beem[0].trac_targ != MHITYOU && beem[0].trac_hit_mons != 0)) // doesn't need to worry about you[0].haste
		{
 		beem[0].range = 6;
 		strcpy(beem[0].beam_name, item_name(mitm.iplus2 [hand_used], mitm.iclass [hand_used], mitm.itype [hand_used], mitm.idam [hand_used], mitm.iplus [hand_used], 1, mitm.iid [hand_used], 6).c_str());
 		mons_throw(i, beem, hand_used);
		continue;
/* 		if (brek == 1)
 		{
 			brek = 0;
 			continue;
 		}*/
		}

	}
}

}

} // end if mons_near



end_throw : if ((menv [i].m_beh == BEH_ENSLAVED || menv [i].m_beh == BEH_FIGHT) && (mmov_x != 0 || mmov_y != 0) && mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] != MNG)
{
			if (monsters_fight(i, mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y]) == 1)
   {
    if (menv [i].m_class == MONS_GIANT_BAT || menv [i].m_class == MONS_UNSEEN_HORROR || menv [i].m_class == MONS_GIANT_BLOWFLY) menv [i].m_speed_inc -= menv [i].m_speed;
		  mmov_x = 0; mmov_y = 0;
 			brkk = 1;
	 		break;
   }
} // end of if menv [i].m_beh == 7

if (brkk == 1) continue;





if (menv [i].m_x + mmov_x == you[0].x_pos && menv [i].m_y + mmov_y == you[0].y_pos)
{

mmov_x = menv [i].m_inv [0];

if ((menv [i].m_class == MONS_GIANT_BAT || menv [i].m_class == MONS_UNSEEN_HORROR || menv [i].m_class == MONS_GIANT_BLOWFLY) && bat == 0) monster_attack(i);

if ((menv [i].m_class == MONS_GIANT_BAT || menv [i].m_class == MONS_UNSEEN_HORROR || menv [i].m_class == MONS_GIANT_BLOWFLY) && menv [i].m_beh != BEH_ENSLAVED) // giant bat
{
	menv [i].m_beh = BEH_WANDER;
	bat = 1;
}



if (menv [i].m_class != MONS_GIANT_BAT && menv [i].m_class != MONS_UNSEEN_HORROR && menv [i].m_class != MONS_GIANT_BLOWFLY) monster_attack(i);


if (menv [i].m_class == MONS_GIANT_SPORE && menv [i].m_hp < 1)
{
	for (j = 0; j < 3; j++)
		{
			menv [i].m_ench [j] = 0;
		}

	menv [i].m_ench_1 = 0;
	no_mons --;
	menv [i].m_class = -1;
 mgrd [menv [i].m_x] [menv [i].m_y] = MNG;
	spore_goes_pop(i); /* has to be after the above, so that spore isn't killed twice. */
	continue;
} // end of if giant spore



mmov_x = 0; mmov_y = 0; continue; //break;
}



if (menv [i].m_class == -1 || menv [i].m_class == MONS_OKLOB_PLANT || menv[i].m_class == MONS_CURSE_SKULL || (menv[i].m_class >= MONS_CURSE_TOE && menv[i].m_class <= MONS_POTION_MIMIC)) continue;
                         // o plant
monster_move(i);


if (menv [i].m_beh == BEH_CHASING_I)//: // chasing YOU
if (mons_near(i) == 1 && (you[0].invis == 0 || mons_see_invis(menv [i].m_class) != 0))// && random2 (6) != 0))
{
	menv [i].m_targ_1_x = you[0].x_pos;
	menv [i].m_targ_1_y = you[0].y_pos;
}

/* Tame monsters can't become afraid, because fear overwrites tameness */
if (menv [i].m_hp <= (menv [i].m_hp_max / 4 - 1) && mons_intel(menv [i].m_class) > 1 && menv [i].m_beh != BEH_ENSLAVED) menv [i].m_beh = BEH_FLEE;



}
endbat : bat = 0;

if (menv [i].m_beh == BEH_FIGHT && random2(3) != 0) menv [i].m_beh = BEH_CHASING_I; // I think?

} // end of if (mons_class != -1)

}


} // end of void monster()



void mons_pickup(int i)
{

if (menv [i].m_class == MONS_JELLY || menv [i].m_class == MONS_OOZE || menv [i].m_class == MONS_ACID_BLOB || menv [i].m_class == MONS_ROYAL_JELLY) /* Jelly! */
{

   int hps_gained;
   int quant_eated;

   quant_eated = random2(10) + 1;

   if (quant_eated > mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]]) quant_eated = mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]];

   if (mitm.iclass [igrd [menv [i].m_x] [menv [i].m_y]] == OBJ_WEAPONS && mitm.idam [igrd [menv [i].m_x] [menv [i].m_y]] > 180)
     return; /* unique items */

   if (mitm.iclass [igrd [menv [i].m_x] [menv [i].m_y]] == OBJ_MISSILES && (mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]] == MI_STONE || mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]] == MI_LARGE_ROCK))
   {
     /* shouldn't eat stone things.
      - but what about stone wands & rings? */
     return;
   }

   if (mitm.iclass [igrd [menv [i].m_x] [menv [i].m_y]] < 15)
   {
      hps_gained = quant_eated * mass(mitm.iclass [igrd [menv [i].m_x] [menv [i].m_y]], mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]]);
      hps_gained /= 20;
   } else
   {
      hps_gained = 1;
   }

   if (hps_gained < 1) hps_gained = 1;
   if (hps_gained > 50) hps_gained = 50;


   mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]] -= quant_eated;
   if (mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]] <= 0) igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];


   menv [i].m_hp += hps_gained;
   if (menv [i].m_hp > menv [i].m_hp_max) menv [i].m_hp_max = menv [i].m_hp;
   /* adjust for mass of object!!! */

   if (mons_near(i) == 1)
   {
      mpr("You hear a slurping noise.");
   } else
         {
            mpr("You hear a distant slurping noise.");
         }


if (menv [i].m_hp >= 50)
{
         if (jelly_divide(i) == 0) menv [i].m_hp = 50;
}

   return;

}




switch(mitm.iclass [igrd [menv [i].m_x] [menv [i].m_y]])
{
	case OBJ_WEAPONS:
	if (menv [i].m_inv [0] != 501)
		return;
 if (mitm.idam [igrd [menv [i].m_x] [menv [i].m_y]] > 180) return;
 if (mitm.idam [igrd [menv [i].m_x] [menv [i].m_y]] % 30 >= 25) return;
 if ((mons_charclass(menv [i].m_class) == MONS_KOBOLD || mons_charclass(menv [i].m_class) == MONS_GOBLIN) && property(mitm.iclass [igrd [menv [i].m_x] [menv [i].m_y]], mitm.itype[igrd [menv [i].m_x] [menv [i].m_y]], 1) <= 0) return;
 // wimpy monsters (Kob, gob) shouldn't pick up halberds etc
 if (mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]] == WPN_GIANT_CLUB || mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]] == WPN_GIANT_SPIKED_CLUB) return;
 // Nobody picks up giant clubs
	menv [i].m_inv [0] = igrd [menv [i].m_x] [menv [i].m_y];
	igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];
 mitm.ilink [menv [i].m_inv [0]] = 501;
 if (mitm.idam [menv [i].m_inv [0]] % 30 == SPWPN_PROTECTION) menv [i].m_AC += 3;
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
 strcat(info, " picks up ");
	strcat(info, it_name(menv [i].m_inv [0], 3).c_str());
 strcat(info, ".");
 if (mons_near(i)) mpr(info);
/*	mmov_x = 0; mmov_y = 0;*/
	break;

	case OBJ_MISSILES:
	if (menv [i].m_inv [1] != 501 && mitm.itype [menv [i].m_inv [1]] == mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]] && mitm.iplus [menv [i].m_inv [1]] == mitm.iplus [igrd [menv [i].m_x] [menv [i].m_y]] && mitm.idam [menv [i].m_inv [1]] == mitm.idam [igrd [menv [i].m_x] [menv [i].m_y]])
    /* Removed check for item_plus2 - probably irrelevant */
	{
		strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
		strcat(info, " picks up ");
		strcat(info, it_name(igrd [menv [i].m_x] [menv [i].m_y], 3).c_str());
		strcat(info, ".");
		if (mons_near(i)) mpr(info);
		mitm.iquant [menv [i].m_inv [1]] += mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]];
		mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]] = 0;
		igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];
	    mitm.ilink [menv [i].m_inv [0]] = 501;
		return;
	}
    if (mitm.itype [igrd [menv [i].m_x] [menv [i].m_y]] == MI_LARGE_ROCK) return;
    // Nobody bothers to pick up rocks if they don't already have some.
	if (menv [i].m_inv [0] != 501 || mitm.iquant [igrd [menv [i].m_x] [menv [i].m_y]] == 1)
		return;
	menv [i].m_inv [1] = igrd [menv [i].m_x] [menv [i].m_y];
	igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];
 mitm.ilink [menv [i].m_inv [0]] = 501;
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
 strcat(info, " picks up ");
	strcat(info, it_name(menv [i].m_inv [1], 3).c_str());
 strcat(info, ".");
 if (mons_near(i)) mpr(info);
/*	mmov_x = 0; mmov_y = 0;*/
	break;

	case OBJ_WANDS:
	if (menv [i].m_inv [5] != 501)
		return;
	menv [i].m_inv [5] = igrd [menv [i].m_x] [menv [i].m_y];
	igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];
 mitm.ilink [menv [i].m_inv [0]] = 501;
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
 strcat(info, " picks up ");
	strcat(info, it_name(menv [i].m_inv [5], 3).c_str());
 strcat(info, ".");
 if (mons_near(i)) mpr(info);
/*	mmov_x = 0; mmov_y = 0;*/
	break;

	case OBJ_SCROLLS:
	if (menv [i].m_inv [6] != 501)
		return;
	menv [i].m_inv [6] = igrd [menv [i].m_x] [menv [i].m_y];
	igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];
 mitm.ilink [menv [i].m_inv [0]] = 501;
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
 strcat(info, " picks up ");
	strcat(info, it_name(menv [i].m_inv [6], 3).c_str());
 strcat(info, ".");
 if (mons_near(i)) mpr(info);
/*	mmov_x = 0; mmov_y = 0;*/
	break;

	case OBJ_POTIONS:
	if (menv [i].m_inv [4] != 501)
		return;
	menv [i].m_inv [4] = igrd [menv [i].m_x] [menv [i].m_y];
	igrd [menv [i].m_x] [menv [i].m_y] = mitm.ilink [igrd [menv [i].m_x] [menv [i].m_y]];
 mitm.ilink [menv [i].m_inv [0]] = 501;
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
 strcat(info, " picks up ");
	strcat(info, it_name(menv [i].m_inv [4], 3).c_str());
 strcat(info, ".");
 if (mons_near(i)) mpr(info);
/*	mmov_x = 0; mmov_y = 0;*/
	break;



 case OBJ_CORPSES: if (menv [i].m_class != MONS_NECROPHAGE && menv [i].m_class != MONS_GHOUL) return;
 menv [i].m_hp += random2(mons_weight(mitm.iplus [igrd [menv [i].m_x] [menv [i].m_y]])) / 100 + 1;
 if (menv [i].m_hp > 77) menv [i].m_hp = 77;
 if (menv [i].m_hp > menv [i].m_hp_max) menv [i].m_hp_max = menv [i].m_hp;
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
 strcat(info, " eats ");
 strcat(info, it_name(igrd [menv [i].m_x] [menv [i].m_y], 1).c_str());
 strcat(info, ".");
 if (mons_near(i)) mpr(info);
 destroy_item(igrd [menv [i].m_x] [menv [i].m_y]);
 break;

}

if (menv [i].m_speed_inc > 25) menv [i].m_speed_inc -= menv [i].m_speed;

}




void monster_move(int i)
{
char which_first = random2(2);
char good_move [3] [3];
int count_x, count_y, vacated_x, vacated_y;
//mmov_x = 0;
//mmov_y = 0;


int okmove = 65;
if (random2(3) == 0 || menv [i].m_class == MONS_FIRE_ELEMENTAL) okmove = 66; /* effectively slows down monster movement across water. Fire elementals can't cross */
if (mons_flies(menv [i].m_class) > 0 || menv [i].m_class >= MLAVA0) okmove = MINMOVE;

for (count_x = 0; count_x < 3; count_x ++)
{
 for (count_y = 0; count_y < 3; count_y ++)
 {
  good_move [count_x] [count_y] = 1;
  if (menv [i].m_class == MONS_TUNNELING_WORM || menv [i].m_class == MONS_BORING_BEETLE)
  {
   if (grd [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1] == 1 && menv [i].m_x + count_x - 1 > 7 && menv [i].m_x + count_x - 1 < 72 && menv [i].m_y + count_y - 1 > 7 && menv [i].m_y + count_y - 1 < 62)
    goto tunnel;
  }
  if (grd [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1] < okmove)
  {
    good_move [count_x] [count_y] = 0;
    continue;
  }
  tunnel : if (mgrd [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1] != MNG)
  {
    if (!((menv [mgrd [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1]].m_beh == BEH_ENSLAVED || menv [i].m_beh == BEH_ENSLAVED) && menv [mgrd [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1]].m_beh != menv [i].m_beh))
    {
     good_move [count_x] [count_y] = 0;
     continue;
    }
  }
  if (env[0].cgrid [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1] != CNG)
  {
  if (env[0].cgrid [menv [i].m_x] [menv [i].m_y] != CNG && env[0].cloud_type [env[0].cgrid [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1]] == env[0].cloud_type [env[0].cgrid [menv [i].m_x] [menv [i].m_y]]) continue;
   if (mons_intel(menv [i].m_class) <= 1) continue;
   switch(env[0].cloud_type [env[0].cgrid [menv [i].m_x + count_x - 1] [menv [i].m_y + count_y - 1]])
   {
     case 1:
     if (mons_res_fire(menv [i].m_class) > 0 || (menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 2)) continue;
     if (menv [i].m_hp >= 15 + random2(10) + random2(10) + random2(10) + random2(10) + random2(10))
        continue;
     break;

     case 2:
     if (mons_res_poison(menv [i].m_class) > 0) continue;
     if (random2(5) + 1 < menv [i].m_HD) continue;
     if (menv [i].m_hp >= random2(10) + random2(10)) continue;
     break;

     case 3:
     if (mons_res_cold(menv [i].m_class) > 0 || (menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 3)) continue;
     if (menv [i].m_hp >= 15 + random2(10) + random2(10) + random2(10) + random2(10) + random2(10))
        continue;
     break;

     case 4:
     if (mons_res_poison(menv [i].m_class) > 0) continue;
     if (menv [i].m_hp >= random2(10) + random2(10) + random2(10) + random2(10)) continue;
     break;

     case 5:
     if (mons_res_fire(menv [i].m_class) > 0 || (menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 2)) continue;
     if (menv [i].m_hp >= random2(10) + random2(10))
        continue;
     break;

    default: continue; /* harmless clouds */
   }
   good_move [count_x] [count_y] = 0;
  }
 }
}



/* some monsters opening doors: change the gmon_use == 1 to gmon_use > 0 maybe? */
	if (grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] == 3 || grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] == 5)
   {
      if (menv [i].m_class == MONS_SMALL_ZOMBIE || menv [i].m_class == MONS_BIG_ZOMBIE || menv [i].m_class == MONS_SMALL_SKELETON || menv [i].m_class == MONS_LARGE_SKELETON || menv [i].m_class == MONS_SPECTRAL_THING)
      {
        if (gmon_use [menv [i].m_sec] > 0)
        {
        		grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] = 70;
          return;
        }
      } else
            if (gmon_use [menv [i].m_class] > 0)
            {
                grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] = 70;
                   return;
            }

      if (menv [i].m_class == MONS_JELLY)
      {
         grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] = 67;
         mpr("You hear a slurping sound.");
         menv [i].m_hp += 5;
         if (menv [i].m_hp > menv [i].m_hp_max) menv [i].m_hp_max = menv [i].m_hp;
      }


   }


/* have a variable equal to the minimum value the monster can move through! */

/*  equivalent of your move() for monsters: */
if (mmov_x != 0 && mmov_y != 0)
{
if (good_move [mmov_x + 1] [mmov_y + 1] == 0)
{

	/* some monsters opening doors */
	if (gmon_use [menv [i].m_class] > 0 && (grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] == 3 || grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] == 5))
	{
   if (menv [i].m_class == MONS_SMALL_ZOMBIE || menv [i].m_class == MONS_BIG_ZOMBIE || menv [i].m_class == MONS_SMALL_SKELETON || menv [i].m_class == MONS_LARGE_SKELETON || menv [i].m_class == MONS_SPECTRAL_THING)
   {
     if (gmon_use [menv [i].m_sec] > 0)
     {
    	 grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] = 70;
     	return;
     }
   } else
         {
          	grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] = 70;
          	return;
         }
	}

if (which_first == 1)
{

	if (good_move [mmov_x + 1] [1] == 1) //grd [menv [i].m_x + mmov_x] [menv [i].m_yly] > okmove && mgrd [menv [i].m_x + mmov_x] [menv [i].m_yly] == MNG)
	{
 		mmov_y = 0;// @@@
	} else
        if (good_move [1] [mmov_y + 1] == 1)
        {
         	mmov_x = 0;
        }

}  /* end if which_first */
 else
 {

  if (good_move [1] [mmov_y + 1] == 1)
  {
    	mmov_x = 0;
  } else
       	if (good_move [mmov_x + 1] [1] == 1)
       	{
       		mmov_y = 0;
       	}
 }  /* end if which_first */


		if (mmov_x != 0 && mmov_y != 0)
		{
		mmov_x = random2 (3) - 1;
		mmov_y = random2 (3) - 1;
		}

}
} else /* end of: if (mmov_x !=0 etc */
{

	if ((mmov_x != 0) ^ (mmov_y != 0)) /* ^ = XOR, || = OR(inc) */
	{
	if (mmov_x != 0 && (good_move [mmov_x + 1] [1] == 0))
	{

if (random2(2) == 0)
{

		if (good_move [1] [0] == 1)
		{
			mmov_y = -1;
   mmov_x = 0;
		} else
		{
			if (good_move [1] [2] == 1)
			{
				mmov_y = 1;
    mmov_x = 0;
			}
  }
} else
 {
		if (good_move [1] [2] == 1)
		{
			mmov_y = 1;
   mmov_x = 0;
		} else
		{
			if (good_move [1] [0] == 1)
			{
				mmov_y = -1;
    mmov_x = 0;
			}
		}
 } /* end if random2(2) else */







	}  else
	if (mmov_y != 0 && (good_move [1] [mmov_y + 1] == 0))
	{

if (random2(2) == 0)
 {

		if (good_move [0] [1] == 1)
			{
				mmov_x = -1;
    mmov_y = 0;
			} else
			if (good_move [2] [1] == 1)
			{
				mmov_x = 1;
    mmov_y = 0;
			}
 } else
   {


		if (good_move [2] [1] == 1)
			{
				mmov_x = 1;
    mmov_y = 0;
			} else
			if (good_move [0] [1] == 1)
			{
				mmov_x = -1;
    mmov_y = 0;
			}

    }  /* end random2(2) else */

	}









if (mmov_x != 0 && mmov_y != 0)
{                                                     // was 11.
/* This could be wrong!!!
   @@@!!! */
if (good_move [mmov_x + 1] [mmov_y + 1] == 1)
{

/*	// some monsters opening doors: change the gmon_use == 1 to gmon_use > 0 maybe? */

	{
		mmov_y = random2 (3) - 1;
		mmov_x = random2 (3) - 1;
	}
}
}

} // end of if (mmov_x != 0 ^ etc for the 2nd time
}  // end of else above


/* maybe: (makes worms stupid, though) */
if ((menv [i].m_class == MONS_TUNNELING_WORM || menv [i].m_class == MONS_BORING_BEETLE) && mmov_x != 0 && mmov_y != 0)
{
	if (random2(2) == 0) mmov_x = 0; else mmov_y = 0;
	if (grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] == 1 && menv [i].m_x + mmov_x > 6 && menv [i].m_x + mmov_x < 73 && menv [i].m_y + mmov_y > 6 && menv [i].m_y + mmov_y < 63)
        {
		grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] = 67;
                mpr("You hear a grinding noise.");
        }
}

int grik = grd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y];

 mgrd [menv [i].m_x] [menv [i].m_y] = MNG;

if (grik >= okmove && mgrd [menv [i].m_x + mmov_x] [menv [i].m_y + mmov_y] == MNG)
{

	if (menv [i].m_x + mmov_x == you[0].x_pos && menv [i].m_y + mmov_y == you[0].y_pos)
	{
		monster_attack(i);
		mmov_x = 0;
		mmov_y = 0;
	}
		vacated_x = menv [i].m_x;
		vacated_y = menv [i].m_y;

 if (menv [i].m_class >= MLAVA0 && menv [i].m_class < MWATER0 && grik != 61)
 {
  mmov_x = 0;
  mmov_y = 0;
 }
 if (menv [i].m_class >= MWATER0 && grik != 62 && grik != 65 && grik != 200)
 {
  mmov_x = 0;
  mmov_y = 0;
 }

 if (menv [i].m_class == MONS_WANDERING_MUSHROOM && see_grid(menv [i].m_x + mmov_x, menv [i].m_y + mmov_y) == 1)
 {
  mmov_x = 0;
  mmov_y = 0;
 }

if (menv [i].m_class == MONS_EFREET || menv [i].m_class == MONS_FIRE_ELEMENTAL) place_cloud(101, menv [i].m_x, menv [i].m_y, 2 + random2(4));

 /* this appears to be the real one: */
	menv [i].m_x += mmov_x;
	menv [i].m_y += mmov_y;

	if (menv [i].m_class == MONS_TUNNELING_WORM && mgrd [vacated_x] [vacated_y] == MNG && (mmov_x != 0 || mmov_y != 0)) //(mmov_x != 0 || mmov_y != 0)) // worm
	{
		char vac_x_2 = vacated_x; //menv [i].m_x;
		char vac_y_2 = vacated_y; //menv [i].m_y;

		mgrd [menv [i].m_x] [menv [i].m_y] = i;

		i++;

		if (menv [i].m_class != MONS_WORM_TAIL)
		{
			goto out_of_worm;
		}

		while(menv [i].m_class == MONS_WORM_TAIL)
		{
   mgrd [menv [i].m_x] [menv [i].m_y] = MNG;

			vacated_x = menv [i].m_x;
			vacated_y = menv [i].m_y;

			menv [i].m_x = vac_x_2;
			menv [i].m_y = vac_y_2;

			vac_x_2 = vacated_x;
			vac_y_2 = vacated_y;

			mgrd [menv [i].m_x] [menv [i].m_y] = i; //+= 50;
			i ++;
		}

		out_of_worm: i--;

		return; /* hopefully will do next monster... */

	}


} else
      {

      }

/* need to put in something so that monster picks up multiple items (eg ammunition) identical to those it's carrying. */

         mgrd [menv [i].m_x] [menv [i].m_y] = i;

/* monsters stepping on traps: */
if (grd [menv [i].m_x] [menv [i].m_y] > 74 && grd [menv [i].m_x] [menv [i].m_y] < 79 && (mmov_x != 0 || mmov_y != 0))
{
//if (mons_flies(menv [i].m_class) == 0)
mons_trap(i);
} /* end of if monster steps on trap */


} /* end of monster_move() */




void plant_spit(int i, struct bolt beam [1])
{
if (mons_near(i) == 0) return;

strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
  strcat(info, " spits at you.");
  mpr(info);

beam[0].move_x = beam[0].trac_targ_x - menv [i].m_x;
beam[0].move_y = beam[0].trac_targ_y - menv [i].m_y;
 strcpy(beam[0].beam_name, "plant spit");

	beam[0].type = '#';
	beam[0].source_x = menv [i].m_x;
	beam[0].source_y = menv [i].m_y;
 beam[0].range = 10;
	beam[0].colour = YELLOW;
 beam[0].flavour = 8;
 beam[0].beam_source = i;

	beam[0].damage = 107;

	beam[0].hit = 20 + menv [i].m_HD * 3;

	beam[0].thing_thrown = 4;

 missile(beam, 0);
}








void mons_in_cloud(int i)
{

int wc = env[0].cgrid [menv [i].m_x] [menv [i].m_y];
int hurted = 0;
struct bolt beam [1];

if (menv [i].m_class >= MONS_GOLD_MIMIC && menv [i].m_class <= MONS_POTION_MIMIC)
{
  mimic_alert(i);
  return;
}

switch(env[0].cloud_type [wc] % 100)
{
	case 99:
	cprintf("Fatal error: monster steps on nonexistant cloud!");
	exit(0);
   break;

	case 1: // fire
  if (menv [i].m_class == MONS_FIRE_VORTEX || menv [i].m_class == MONS_EFREET || menv [i].m_class == MONS_FIRE_ELEMENTAL) break; // fire vortex, efreet, and fire elemental
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0)); //gmon_name [menv [i].m_class]);
	strcat(info, " is engulfed in flame!");
	if (mons_near(i) == 1 && menv [i].m_class != MONS_EFREET) mpr(info); // efreet
   if (mons_res_fire(menv [i].m_class) > 0) break;
	hurted += ((random2(8) + random2(5) + random2(5) + 6) * 10) / menv [i].m_speed;
 if (mons_res_fire(menv [i].m_class) < 0 && !(menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 2)) hurted += (random2(15) * 10) / menv [i].m_speed;
 // remember that the above is in addition to the other you[0].damage.
 hurted -= random2(menv [i].m_AC + 1);
 if (menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 2)
 {
  hurted /= 3;
 }
 if (hurted <= 0) hurted = 0;
 menv [i].m_hp -= hurted;
	if (menv [i].m_hp < 1)
	{
		if (env[0].cloud_type [wc] >= 100) monster_die(i, 5, 0); else monster_die(i, 1, 0);
		menv [i].m_speed_inc = 1;
	}
	break;

   case 2: // stinking cloud
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " is engulfed in noxious gasses!");
   if (mons_near(i) == 1) mpr(info);

   if (mons_res_poison(menv [i].m_class) > 0) return;

   int func_pass [10];
   beam[0].colour = 4;
   if (random2(27) + 1 >= menv [i].m_HD) mons_ench_f2(i, mons_near(i), func_pass, beam);

   hurted += (random2(3) * 10) / menv [i].m_speed;
//   hurted -= random2(menv [i].m_AC + 1);
   if (hurted <= 0) hurted = 0;
   menv [i].m_hp -= hurted;
   if (menv [i].m_hp < 1)
   {
		if (env[0].cloud_type [wc] >= 100) monster_die(i, 5, 0); else monster_die(i, 1, 0);

   }
   break;




 case 3: // cold
	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0)); //gmon_name [mons_class [i]]);
	strcat(info, " is engulfed in freezing vapours!");
 if (mons_near(i) == 1) mpr(info);
   if (mons_res_cold(menv [i].m_class) > 0) break;
	hurted += ((random2(8) + random2(5) + random2(5) + 6) * 10) / menv [i].m_speed;
 if (mons_res_cold(menv [i].m_class) < 0 && !(menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 3)) hurted += (random2(15) * 10) / menv [i].m_speed;
 // remember that the above is in addition to the other damage.
 hurted -= random2(menv [i].m_AC + 1);
 if (menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 3)
 {
  hurted /= 3;
 }
 if (hurted <= 0) hurted = 0;
 menv [i].m_hp -= hurted;
	if (menv [i].m_hp < 1)
	{
		if (env[0].cloud_type [wc] >= 100) monster_die(i, 5, 0); else monster_die(i, 1, 0);
                menv [i].m_speed_inc = 1;
	}
	break;

   case 4: // you[0].poison cloud
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " is engulfed in a cloud of poison!");
   if (mons_near(i) == 1) mpr(info);
   if (mons_res_poison(menv [i].m_class) > 0) return;
   if (env[0].cloud_type [wc] >= 100) poison_monster(i, 1); // something else
								else poison_monster(i, 0);

   hurted += (random2(8) * 10) / menv [i].m_speed;
   if (mons_res_poison(menv [i].m_class) < 0) hurted += (random2(4) * 10) / menv [i].m_speed;
   if (hurted <= 0) hurted = 0;
   menv [i].m_hp -= hurted;
   if (menv [i].m_hp < 1)
   {
		if (env[0].cloud_type [wc] >= 100) monster_die(i, 5, 0); else monster_die(i, 1, 0);
   }
   break;

// 5,6,7 are harmless smoke

  case 8: // steam - I couldn't be bothered doing this for armour of res fire
  if (menv [i].m_class == MONS_STEAM_DRAGON) break;
  	strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0)); //gmon_name [mons_class [i]]);
	strcat(info, " is engulfed in steam!");
	if (mons_near(i) == 1 && menv [i].m_class != MONS_EFREET) mpr(info); // efreet
   if (mons_res_fire(menv [i].m_class) > 0 || (menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 2)) break;
	hurted += (random2(6) * 10) / menv [i].m_speed;
 if (mons_res_fire(menv [i].m_class) < 0 && !(menv [i].m_inv [2] != 501 && mitm.idam [menv [i].m_inv [2]] % 30 == 2)) hurted += (random2(6) * 10) / menv [i].m_speed;
 hurted -= random2(menv [i].m_AC + 1);
 if (hurted <= 0) hurted = 0;
 menv [i].m_hp -= hurted;
	if (menv [i].m_hp < 1)
	{
                if (env[0].cloud_type [wc] >= 100) monster_die(i, 5, 0); else monster_die(i, 1, 0);
		menv [i].m_speed_inc = 1;
	}
  break;


  case 9: // dark miasma
   strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));
   strcat(info, " is engulfed in a dark miasma.");
   if (mons_near(i) == 1) mpr(info);
   if (mons_holiness(menv [i].m_class) > 0) return;
   if (env[0].cloud_type [wc] >= 100) poison_monster(i, 1); // something else
	else poison_monster(i, 0);

   hurted += ((random2(5) + random2(5) + random2(4)) * 10) / menv [i].m_speed; // 3
   if (hurted <= 0) hurted = 0;
   menv [i].m_hp -= hurted;
   if (menv [i].m_hp_max > 4)
   {
    menv [i].m_hp_max -= random2(2);
   }
   beam[0].colour = 0;
   if (random2(3) == 0) mons_ench_f2(i, mons_near(i), func_pass, beam);

   if (menv [i].m_hp < 1)
   {
	if (env[0].cloud_type [wc] >= 100) monster_die(i, 5, 0); else monster_die(i, 1, 0);
   }
  break;

}


}


char mons_speaks(int i)
{

if (menv [i].m_beh == BEH_FLEE) return 0;

strcpy(info, monam (menv [i].m_sec, menv [i].m_class, menv [i].m_ench [2], 0));

switch(menv [i].m_class)
{
 case MONS_DORGI: // dorgi
 switch(random2(10))
 {
  case 0: mpr(" screams \"HALT!\""); break;
  case 1: mpr(" screams \"HALT! HALT RIGHT NOW!\""); break;
  case 2: mpr(" screams \"HALT, OR YOU WILL BE ELIMINATED!\""); break;
  case 3: mpr(" screams \"HALT OR I SHOOT!\""); break;
  case 4: mpr(" screams \"DROP YOUR WEAPONS! UP AGAINST THE WALL!\""); break;
  case 5: mpr(" screams \"PREPARE TO DIE!\""); break;
  case 6: mpr(" screams \"SUBMIT OR DIE!\""); break;
  case 7: mpr(" screams \"YOU ARE VIOLATING AREA SECURITY!\""); break;
  case 8: mpr(" screams \"SUBMIT TO THERAPY OR DIE!\""); break;
  case 9: mpr(" screams \"HALT, TRESPASSER!\""); break;
 }
 break;

 case MONS_KILLER_KLOWN: // Killer Klown
 switch(random2(10))
 {
  case 0: mpr(" giggles crazily."); break;
  case 1: mpr(" laughs merrily."); break;
  case 2: mpr(" beckons to you."); break;
  case 3: mpr(" does a flip."); break;
  case 4: mpr(" does a somersault."); break;
  case 5: mpr(" smiles at you."); break;
  case 6: mpr(" grins with merry abandon."); break;
  case 7: mpr(" howls with blood-lust!"); break;
  case 8: mpr(" pokes out its tongue."); break;
  case 9: mpr(" says \"Come and play with me!\""); break;
 }
 break;

/* default:
 switch(random2(10))
 {
  case 0: strcat(info, " laughs at you."); break;
  case 1: strcat(info, " salutes you derisively."); break;
  case 2: strcat(info, " beckons to you."); break;
  case 3: strcat(info, " smiles in anticipation of an easy kill."); break;
  case 4: strcat(info, " giggles maniacally."); break;
  case 5: strcat(info, " ."); break;
  case 6: strcat(info, " ."); break;
  case 7: strcat(info, " ."); break;
  case 8: strcat(info, " ."); break;
  case 9: strcat(info, " ."); break;
 }
 mpr(info);
 break;*/

}

return 1;

}





