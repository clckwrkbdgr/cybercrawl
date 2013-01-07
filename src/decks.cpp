
#include "linuxlib.h"

#include <string.h>

#include "externs.h"
#include "enum.h"

#include "effects.h"
#include "food.h"
#include "it_use2.h"
#include "misc.h"
#include "monplace.h"
#include "ouch.h"
#include "player.h"
#include "spells.h"
#include "spells1.h"
#include "spells3.h"
#include "stuff.h"

#include "mutation.h"
#include "religion.h"

/* Number of cards. This number includes the first (number zero) card */
#define DECK_WONDERS 27
#define DECK_SUMMONING 11
#define DECK_TRICKS 11
#define DECK_POWER 17

#define TOTAL_CARDS 55

char mutate(int which_mutation);

unsigned char deck_of_wonders [] =
{
0, /* Blank */
1, /* Butterfly */
2, /* Wraith */
3, /* Xp */
4, /* Wealth */
5, /* Brain */
6, /* Strength */
7, /* Quicksilver */
8, /* Stupid */
9, /* Weak */
10, /* Slug */
11, /* Shuffle */
12, /* Freak */
13, /* Death */
14, /* Normalisation */
15, /* Shadow */
16, /* Gate */
17, /* Statue */
18, /* Acquisition */
19, /* Haste */
29, /* Lich */
37, /* Xom */
39, /* Decay */
44, /* Altar */
43, /* Fountain */
51, /* Maze */
52, /* Pandemonium */
0, /* Sanity buffer... */
0 /* */
};

unsigned char deck_of_summoning [] =
{
17, /* Statue */
20, /* Little demon */
21, /* Bigger demon */
22, /* Very big dem */
23, /* Demon swarm */
24, /* Yak */
25, /* Fiend */
26, /* Dragon */
27, /* golem */
28, /* Very ugly */
30, /* Unseen horror */
0, /* Sanity buffer... */
0 /* */

};

unsigned char deck_of_tricks [] =
{
0, /* Blank */
1, /* Butterfly */
31, /* Blink */
32, /* Teleport */
33, /* Inst teleport */
34, /* Rage */
35, /* Levity */
//38, /* Slowness */
40, /* Healing */
47, /* Wild Magic */
20, /* Little demon */
19, /* Haste */
0, /* Sanity buffer... */
0 /* */

};

unsigned char deck_of_power [] =
{
0, /* Blank */
21, /* Demon */
22, /* Big demon */
33, /* Inst teleport */
36, /* Venom */
37, /* Xom */
41, /* Heal wounds */
45, /* Famine */
46, /* Feast */
47, /* Wild Magic */
48, /* Violence */
49, /* Protection */
50, /* Knowledge */
19, /* Haste */
42, /* Torment */
23, /* Demon Swarm (bad) */
38, /* Slowness */
0, /* Sanity buffer... */
0 /* */
};

void cards(unsigned char which_card);


void deck_of_cards(unsigned char which_deck)
{
int card [50];
unsigned char max_card = 0;
int i = 0;

mpr("You execute a nanite program...");

switch(which_deck)
{
 case 0:
      for (i = 0; i < DECK_WONDERS; i ++)
      {
       card [i] = deck_of_wonders [i];
      }
 	  max_card = DECK_WONDERS;
 	break;
 case 1:
      for (i = 0; i < DECK_SUMMONING; i ++)
      {
       card [i] = deck_of_summoning [i];
      }
 	  max_card = DECK_SUMMONING;
 	break;
 case 2:
      for (i = 0; i < DECK_TRICKS; i ++)
      {
       card [i] = deck_of_tricks [i];
      }
 	  max_card = DECK_TRICKS;
 	break;
 case 3:
      for (i = 0; i < DECK_POWER; i ++)
      {
       card [i] = deck_of_power [i];
      }
 	  max_card = DECK_POWER;
 	break;
}

i = card [random2(max_card)];

if (random2(250) == 0)
{
 mpr("This program doesn't seem to belong here.");
 i = random2(TOTAL_CARDS);
}

cards(i);

you[0].inv_plus [you[0].equip [EQ_WEAPON]] --;
if (you[0].inv_plus [you[0].equip [EQ_WEAPON]] == 0)
{
 mpr("You throw away the empty kit of nanite programs.");
 unwield_item(you[0].equip [EQ_WEAPON]);
 you[0].inv_quant [you[0].equip [EQ_WEAPON]] --;
 if (you[0].inv_quant [you[0].equip [EQ_WEAPON]] == 0) // can this be false?
 {
		you[0].inv_no --;
		you[0].equip [EQ_WEAPON] = -1;
		mpr("You are now empty handed.");
 }
 done_good(11, 1 + random2(2));
 if (which_deck == 0) done_good(11, 2);
 if (which_deck == 3) done_good(11, 1);
}
burden_change();

if (random2(3) == 0 || which_deck == 0) done_good(11, 1);
/* Using cards pleases Nemelex */

}

void cards(unsigned char which_card)
{

int dvar [5];
int dvar1 = 0;

switch(which_card)
{
default:
case 0:
 mpr("It is blank.");
break;
case 1:
mpr("You have execute the Butterfly.");
mpr("A brightly coloured insect flies from the nanites!");
create_monster(MONS_BUTTERFLY, 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 2:
mpr("You have execute the Wraith.");
lose_level();
drain_exp();
break;
case 3:
mpr("You have execute Experience.");
if (you[0].xp < 27)
 you[0].xp = exp_needed(you[0].xl + 2, you[0].species) + 1;
level_change();
break;
case 4:
mpr("You have execute Wealth.");
you[0].gp += 800 + random2(500) + random2(500);
you[0].gp_ch = 1;
break;
case 5:
mpr("You have execute the Brain!");
you[0].max_intel += 1 + random2(2) + random2(2);
you[0].intel_ch = 1; // note that only the maximum is increased
break;
case 6:
mpr("You have execute Strength!");
you[0].max_strength += 1 + random2(2) + random2(2);
you[0].strength_ch = 1;
break;
case 7:
mpr("You have execute the Quicksilver card.");
you[0].max_dex += 1 + random2(2) + random2(2);
you[0].dex_ch = 1;
break;
case 8:
mpr("You have execute Stupidity!");
you[0].intel -= 2 + random2(2) + random2(2);
if (you[0].intel <= 3) you[0].intel = 0;
you[0].intel_ch = 1;
break;
case 9:
mpr("You have execute Weakness.");
you[0].strength -= 2 + random2(2) + random2(2);
if (you[0].strength <= 3) you[0].strength = 0;
you[0].strength_ch = 1;
break;
case 10:
mpr("You have execute the Slug.");
you[0].dex -= 2 + random2(2) + random2(2);
if (you[0].dex <= 3) you[0].dex = 0;
you[0].dex_ch = 1;
break;
case 11: // shuffle stats
mpr("You have execute the Shuffle card!");
//you[0].strength = you[0].max_strength;
//you[0].intel = you[0].max_intel;
//you[0].dex = you[0].max_dex;
dvar [0] = you[0].strength;
dvar [1] = you[0].intel;
dvar [2] = you[0].dex;
you[0].strength = 101;
you[0].intel = 101;
you[0].dex = 101;
do
{
 dvar1 = random2(3);
 if (dvar [dvar1] == 101)
 {
  continue;
 }

 if (you[0].strength == 101)
 {
  you[0].strength = dvar [dvar1];
  you[0].max_strength = dvar [dvar1];
 } else
 if (you[0].intel == 101)
 {
  you[0].intel = dvar [dvar1];
  you[0].max_intel = dvar [dvar1];
 } else
 if (you[0].dex == 101)
 {
  you[0].dex = dvar [dvar1];
  you[0].max_dex = dvar [dvar1];
 }
 dvar [dvar1] = 101;
} while (dvar [0] != 101 || dvar [1] != 101 || dvar [2] != 101);
you[0].strength_ch = 1;
you[0].intel_ch = 1;
you[0].dex_ch = 1;
burden_change();
break;
case 12: // mutation
mpr("You have execute the Freak!");
mutate(100);
mutate(100);
mutate(100);
mutate(100);
mutate(100);
mutate(100);
break;
case 13: // reaper
strcpy(info, "Oh no! You have execute the Death program!");
if (you[0].duration [DUR_TELEPORT] != 0) you_teleport(); /* heh heh heh */
create_monster(MONS_REAPER, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_REAPER, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_REAPER, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_REAPER, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_REAPER, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 14: // lose mutations
mpr("You have execute Normalisation.");
delete_mutation(100);
delete_mutation(100);
delete_mutation(100);
delete_mutation(100);
delete_mutation(100);
delete_mutation(100);
break;
case 15: // soul eater
mpr("You have execute the Shadow.");
create_monster(MONS_SOUL_EATER, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 16: // gate to abyss
mpr("You have execute the Gate!");
more();
if (you[0].level_type == 2)
{
  banished(97);
  break;
}
if (you[0].level_type == 1)
{
  mpr("Nothing appears to happen.");
  break;
}
banished(96);
break;
case 17: // pet
mpr("You have execute the Crystal Statue.");
create_monster(MONS_CRYSTAL_GOLEM, 0, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
break;
case 18: // acquirement
mpr("You have execute Acquisition!");
mpr("The nanites assembles to form a single-use device.");
acquirement(250);
break;
case 19: // Haste
mpr("You have execute Haste.");
potion_effect(POT_SPEED, 150);
break;
case 20:
	mpr("On the program is a picture of a little demon.");
	mpr("The picture comes to life!");
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 21:
	mpr("On the program is a picture of a demon.");
	mpr("The picture comes to life!");
create_monster(MONS_NEQOXEC + random2(5), 21 + random2(3), 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 22:
	mpr("On the program is a picture of a huge demon.");
	mpr("The picture comes to life!");
create_monster(MONS_NEQOXEC + random2(5), 20 + random2(3), 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 23:
	mpr("On the program is a picture of a swarm of little demons.");
	mpr("The picture comes to life!");
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
create_monster(MONS_WHITE_IMP + random2(5), 21 + random2(4), 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 24:
	mpr("On the program is a picture of a huge shaggy yak.");
	mpr("The picture comes to life!");
create_monster(MONS_DEATH_YAK, 25, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 25:
	mpr("On the program is a picture of a huge scaly devil.");
	mpr("The picture comes to life!");
create_monster(MONS_FIEND, 21, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 26:
	mpr("On the program is a picture of a huge scaly dragon.");
	mpr("The picture comes to life!");
if (random2(2) == 0)
 create_monster(MONS_DRAGON, 22, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
  else create_monster(MONS_ICE_DRAGON, 22, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 27:
	mpr("On the program is a picture of a statue.");
	mpr("The picture comes to life!");
create_monster(MONS_CLAY_GOLEM + random2(6), 21, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 28:
	mpr("On the program is a picture of a very ugly thing.");
	mpr("The picture comes to life!");
create_monster(MONS_VERY_UGLY_THING, 21, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 29:
	mpr("On the program is a picture of a very irritated-looking skeletal thing.");
	mpr("The picture comes to life!");
create_monster(MONS_LICH, 0, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 30: // unseen horror
if (player_see_invis() == 0)
{
 mpr("It is blank");
} else
{
 mpr("On the program is a picture of a hideous abomination.");
 mpr("The picture comes to life!");
}
create_monster(MONS_UNSEEN_HORROR, 21, 7, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
break;
case 31: // Blink
mpr("You have execute Blink.");
random_blink();
break;
case 32:
mpr("You have execute the Portal of Delayed Transposition.");
you_teleport();
break;
case 33:
mpr("You have execute the Portal of Instantaneous Transposition.");
you_teleport2(1);
break;
case 34:
mpr("You have execute Rage.");
if (go_berserk() == 0) mpr("Nothing appears to happen.");
break;
case 35:
mpr("You have execute Levity.");
potion_effect(POT_LEVITATION, 100);
break;
case 36:
mpr("You have execute Venom.");
if (player_res_poison() == 0)
{
  you[0].poison += 2 + random2(3);
  mpr("You feel poison flood through your system.");
}
break;
case 37:
mpr("You have execute the program of Anonimous!");
Xom_acts(1, 5 + random2(12), 1);
break;
case 38:
mpr("You have execute Slowness.");
potion_effect(POT_SLOWING, 100);
break;
case 39:
mpr("You have execute Decay.");
if (you[0].is_undead != 0)
{
   mpr("You feel terrible.");
   break;
}
	mpr("You feel your flesh start to rot away!");
you[0].rotting += random2(5) + 4;
break;
case 40:
mpr("You have execute the Elixir of Health.");
potion_effect(POT_HEALING, 100);
break;
case 41:
mpr("You have execute the Symbol of Immediate Regeneration.");
potion_effect(POT_HEALING, 100);
break;
case 42:
mpr("You have execute the Symbol of Torment.");
torment();
break;
case 43:
mpr("You have execute the Fountain.");
if (grd [you[0].x_pos] [you[0].y_pos] == 67)
{
  mpr("A water conduit of clear blue water grows from the floor at your feet.");
  grd [you[0].x_pos] [you[0].y_pos] = 200;
} else mpr("Nothing appears to happen.");
break;
case 44:
mpr("You have execute the Terminal.");
if (you[0].religion == GOD_NO_GOD)
{
 mpr("Nothing appears to happen.");
 break;
}
dvar1 = 179 + you[0].religion;
if (grd [you[0].x_pos] [you[0].y_pos] == 67)
{
  mpr("A terminal grows from the floor at your feet!");
  grd [you[0].x_pos] [you[0].y_pos] = dvar1;
} else
{
 do
 {
  dvar [0] = 10 + random2(70);
  dvar [1] = 10 + random2(60);
 } while (grd [dvar [0]] [dvar [1]] != 67);
 grd [dvar [0]] [dvar [1]] = dvar1;
 mpr("A terminal waits for you somewhere on this level!");
}
break;
case 45:
mpr("You have execute Famine.");
if (you[0].is_undead == 2)
{
 mpr("You feel rather smug.");
 break;
}
you[0].hunger = 500;
food_change();
break;
case 46:
mpr("You have execute the Feast.");
if (you[0].is_undead == 2)
{
 mpr("You feel a horrible emptiness.");
 break;
}
you[0].hunger = 12000;
food_change();
break;
case 47:
mpr("You have execute Corrupted Program.");
miscast_effect(10 + random2(15), random2(15) + 5, random2(250), 0);
break;
case 48:
mpr("You have execute Violence.");
acquirement(0);
break;
case 49:
mpr("You have execute Protection.");
acquirement(2);
break;
case 50:
mpr("You have execute Knowledge.");
acquirement(10);
break;
case 51:
mpr("You have execute the Maze!");
more();
if (you[0].level_type != 0) break;
banished(81);
break;
case 52:
mpr("You have execute the Bioengineering program!");
more();
if (you[0].level_type == 3)
{
  banished(100);
  break;
}
if (you[0].level_type == 1)
{
  mpr("Nothing appears to happen.");
  break;
}
banished(99);
break;
case 53:
mpr("You have execute the rules for contract bridge.");
mpr("How intriguing!");
break;
case 54:
mpr("You have execute the Prison!");
entomb();
break;

// Fear, immolation, Holy Word

}

}

