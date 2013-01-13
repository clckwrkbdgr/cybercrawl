
#include "linuxlib.h"

#include <stdlib.h>
#include <string.h>

#include "externs.h"
#include "enum.h"

#include "bang.h"
#include "describe.h"
#include "dungeon.h"
#include "effects.h"
#include "itemname.h"
#include "it_use2.h"
#include "misc.h"
#include "monplace.h"
#include "mutation.h"
#include "ouch.h"
#include "player.h"
#include "shopping.h"
#include "spells.h"
#include "spells1.h"
#include "spells3.h"
#include "stuff.h"
#include "religion.h"

int mutate(int which_mutation);


std::string sacrifice [] =
{
" glows silver and disappears.",
" glows a brilliant golden colour and disappears.",
" rots away in an instant.",
" crumbles to dust.",
" is eaten by a bug.", /* Xom - no sacrifices */
" explodes into nothingness.",
" is consumed in a burst of flame.",
" is consumed in a roaring column of flame.",
" glows faintly for a moment, then is gone.",
" is consumed in a roaring column of flame.",
" glows with a rainbow of weird colours and disappears.",
" evaporates."
};

std::string god_name_long(int which_god);
void altar_prayer();

std::string god_attitude(int piety)
{
	if (piety <= 5) {
		return "is displeased.";
	} else if (piety <= 20) {
		return "is noncommittal.";
	} else if (piety <= 40) {
		return "is pleased with you.";
	} else if (piety <= 70) {
		return "is most pleased with you.";
	} else if (piety <= 100) {
		you[0].duration [DUR_PRAYER] *= 2;
		return "is greatly pleased with you.";
	} else if (piety <= 130) {
		you[0].duration [DUR_PRAYER] *= 2;
		return "is extremely pleased with you.";
	}
	you[0].duration [DUR_PRAYER] *= 3;
	return "is exalted by your worship!";
}

void pray(void)
{

int was_praying = you[0].duration [DUR_PRAYER];

if (you[0].religion != GOD_NO_GOD && grd [you[0].x_pos] [you[0].y_pos] == 179 + you[0].religion)
{
 altar_prayer();
} else
if (grd [you[0].x_pos] [you[0].y_pos] >= 180 && grd [you[0].x_pos] [you[0].y_pos] <= 199)
{
 if (you[0].species == SP_DEMIGOD)
 {
  mpr("Sorry, a being of your status cannot employ here.");
  return; /* demigod */
 }
 god_pitch(grd [you[0].x_pos] [you[0].y_pos] - 179);
 return;
}


if (you[0].religion == GOD_NO_GOD)
{
 if (you[0].is_undead != 0) mpr("You spend a moment contemplating the meaning of death.");
  else mpr("You spend a moment contemplating the meaning of life.");
 return;
}

if (you[0].religion == GOD_XOM)
{
mpr("The Anonimous ignores you.");
return;
}
msg("You offer a prayer to @1.") << god_name(you[0].religion);

you[0].duration [DUR_PRAYER] = 9 + random2(you[0].piety) / 20 + random2(you[0].piety) / 20;

//if (you[0].clas == 2 || you[0].clas == 6) you[0].duration [3] = 4 + random2(you[0].piety) / 20 + random2(you[0].piety) / 20 + random2(you[0].piety) / 20;

msg("@1 @2") << god_name(you[0].religion) << god_attitude(you[0].piety);

you[0].turnover = 1;

if (was_praying == 0)
{
/*
Remember to check for water/lava
*/

 if (you[0].religion == GOD_NEMELEX_XOBEH && random2(200) <= you[0].piety && (random2(3) == 0 || you[0].attribute [ATTR_CARD_TABLE] == 0) && you[0].attribute [ATTR_CARD_COUNTDOWN] == 0 && grd [you[0].x_pos] [you[0].y_pos] != 61 && grd [you[0].x_pos] [you[0].y_pos] != 62)
 {
    int Nemelex = 0;
    int gift_type = MISC_DECK_OF_TRICKS; /* deck of tricks */
    if (you[0].attribute [ATTR_CARD_TABLE] == 0) /* portable altar */
    {
      Nemelex = items(1, OBJ_MISCELLANY, MISC_PORTABLE_ALTAR_OF_NEMELEX, 1, 1, 250);
      you[0].attribute [ATTR_CARD_TABLE] = 1;
    }
       else
         {
          if (random2(200) <= you[0].piety && random2(4) == 0)
            gift_type = MISC_LANTERN_OF_SHADOWS;
          if (random2(200) <= you[0].piety && random2(2) == 0)
            gift_type = MISC_HORN_OF_GERYON;
          if (random2(200) <= you[0].piety && random2(2) == 0)
            gift_type = MISC_DECK_OF_POWER;
      	  Nemelex = items(1, OBJ_MISCELLANY, gift_type, 1, 1, 250);
         }
    if (you[0].species != SP_NAGA) mpr("Something appears at your feet!");
     else mpr("Something appears before you!");
    more();
    int what_was_theren = igrd [you[0].x_pos] [you[0].y_pos];
    mitm.ilink [Nemelex] = what_was_theren;
    igrd [you[0].x_pos] [you[0].y_pos] = Nemelex;
    you[0].attribute [ATTR_CARD_COUNTDOWN] = 10;
    lose_piety(5 + random2(5) + random2(5));
 }

 if ((you[0].religion == GOD_OKAWARU || you[0].religion == GOD_TROG) && you[0].piety > 130 && random2(you[0].piety) > 120 && random2(4) == 0 && grd [you[0].x_pos] [you[0].y_pos] != 61 && grd [you[0].x_pos] [you[0].y_pos] != 62)
 {
   msg("@1 grants you a gift!") << god_name(you[0].religion);
   more();
   if (you[0].religion == GOD_TROG || (you[0].religion == GOD_OKAWARU && random2(2) == 0)) acquirement(OBJ_WEAPONS);
     else acquirement(OBJ_ARMOUR);
   lose_piety(30 + random2(10) + random2(10));
 }

 if (you[0].religion == GOD_YREDELEMNUL && random2(you[0].piety) > 80 && random2(10) == 0)
 {
   msg("@1 grants you a cyborg servant!") << god_name(you[0].religion);
   more();
   int und_serv = MONS_WRAITH; /* wraith */
   if (random2(7) == 0) und_serv = MONS_MUMMY;
   if (random2(7) == 0) und_serv = MONS_VAMPIRE;
   if (random2(10) == 0) und_serv = MONS_FLAYED_GHOST;
   if (random2(10) == 0) und_serv = MONS_GHOUL;
   if (random2(7) == 0) und_serv = MONS_ROTTING_HULK;
   if (random2(9) == 0) und_serv = MONS_SKELETAL_WARRIOR;
   if (random2(7) == 0) und_serv = MONS_SPECTRAL_WARRIOR;
   if (random2(7) == 0) und_serv = MONS_WIGHT;
   create_monster(und_serv, 0, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
   lose_piety(4 + random2(4) + random2(4));
 }


 if ((you[0].religion == GOD_KIKUBAAQUDGHA || you[0].religion == GOD_SIF_MUNA || you[0].religion == GOD_VEHUMET) && you[0].piety > 160 && random2(you[0].piety) > 100)
 {
  int gift = BOOK_MINOR_MAGIC_I;
  if (you[0].religion == GOD_KIKUBAAQUDGHA)
  {
   if (you[0].had_item [BOOK_NECRONOMICON] == 0) gift = BOOK_NECRONOMICON; // Kiku - gives death books
   if (you[0].had_item [BOOK_UNLIFE] == 0) gift = BOOK_UNLIFE;
   if (you[0].had_item [BOOK_DEATH] == 0) gift = BOOK_DEATH;
   if (you[0].had_item [BOOK_NECROMANCY] == 0) gift = BOOK_NECROMANCY;
  }

  if (you[0].religion == GOD_SIF_MUNA) gift = 250; // Sif Muna - gives any

  if (you[0].religion == GOD_VEHUMET) // Vehumet - gives conj/summ. Gives bks first for whichever skill is higher
  {
   if (you[0].skills [SK_CONJURATIONS] < you[0].skills [SK_SUMMONINGS])
   {
    if (you[0].had_item [BOOK_ANNIHILATIONS] == 0) gift = BOOK_ANNIHILATIONS; // conj bks
    if (you[0].had_item [BOOK_CONJURATIONS_I] == 0) gift = BOOK_CONJURATIONS_I;
   }
   if (you[0].had_item [BOOK_DEMONOLOGY] == 0) gift = BOOK_DEMONOLOGY; // summoning bks
   if (you[0].had_item [BOOK_INVOCATIONS] == 0) gift = BOOK_INVOCATIONS;
   if (you[0].had_item [BOOK_SUMMONINGS] == 0) gift = BOOK_SUMMONINGS;
   if (you[0].skills [SK_CONJURATIONS] >= you[0].skills [SK_SUMMONINGS])
   {
    if (you[0].had_item [BOOK_ANNIHILATIONS] == 0) gift = BOOK_ANNIHILATIONS; // conj bks again
    if (you[0].had_item [BOOK_CONJURATIONS_I] == 0) gift = BOOK_CONJURATIONS_I;
   }
  }

  if (gift != BOOK_MINOR_MAGIC_I && (grd [you[0].x_pos] [you[0].y_pos] != 61 && grd [you[0].x_pos] [you[0].y_pos] != 62))
  { /* shouldn't give you something if it's just going to fall in a pool */
   msg("@1 grants you a gift!") << god_name(you[0].religion);
   more();
   if (gift == 250)
   {
    acquirement(OBJ_BOOKS);
   } else
   {
    int thing_created = items(1, OBJ_BOOKS, gift, 1, 1, 250);
    if (you[0].species != SP_NAGA) mpr("Something appears at your feet!");
     else mpr("Something appears before you!");
    more();
    int what_was_there = igrd [you[0].x_pos] [you[0].y_pos];
    mitm.ilink [thing_created] = what_was_there;
    igrd [you[0].x_pos] [you[0].y_pos] = thing_created;
   }
   lose_piety(40 + random2(10) + random2(10));
   if (you[0].religion == GOD_VEHUMET) lose_piety(10 + random2(10)); /* Vehumet gives books less readily */
  }
 }
}

}

std::string god_name(int which_god)
{
	switch(which_god) {
		case 0: return "You aren't employed!";
		case 1: return "Knights Templar";
		case 2: return "UNATCO";
		case 3: return "Manticore";
		case 4: return "NSF";
		case 5: return "The Anonimous";
		case 6: return "Digicorp";
		case 7: return "S.T.A.R.S.";
		case 8: return "Netchaos Order";
		case 9: return "Black Mesa";
		case 10: return "Human Liberation Front";
		case 11: return "Triads";
		case 12: return "Aperture Labs";
	}
	return "Illegal employer!";
}


std::string god_name_long(int which_god)
{
	switch(which_god) {
		case 0: return "Nobody";
		case 1: return "Mighty Knights Templar";
		case 2: return "United Nation Anti-Terrorist Coalition";
		case 3: return "Manticore Bioresearch";
		case 4: return "National Sessionist Forces";
		case 5: return "The Anonimous";
		case 6: return "Digicorp";
		case 7: return "S.T.A.R.S.";
		case 8: return "Netchaos Order";
		case 9: return "Black Mesa Research Facility";
		case 10: return "Human Liberation Front";
		case 11: return "Triads";
		case 12: return "Aperture Science Laboratories";
	}
	return "Illegal employer!";
}


void Xom_acts(int niceness, int sever, int force_sever)
{

/*
niceness = 0 - bad, 1 - nice
*/

struct bolt beam [1];

if (sever <= 1) sever = 1;

if (force_sever == 0) sever = random2(sever);

if (sever == 0) return;

okay_try_again : if (niceness == 0 || random2(3) == 0) /* bad things */
{
 if (random2(sever) <= 2) /* this should always be first - it will often be called deliberately, with a low sever value */
 {
  switch(random2(4))
  {
   case 0: mpr("Anonimous notices you."); break;
   case 1: mpr("Anonimous's attention turns to you for a moment."); break;
   case 2: mpr("Anonimous's power touches on you for a moment."); break;
   case 3: mpr("You hear Anonimous's maniacal laughter."); break;
  }
  miscast_effect(10 + random2(15), random2(10) + 5, random2(100), 0);
  return;
 }
 if (random2(sever) <= 2)
 {
  switch(random2(4))
  {
   case 0: mpr("\"Suffer!\""); break;
   case 1: mpr("Anonimous's malign attention turns to you for a moment."); break;
   case 2: mpr("Anonimous's power touches on you for a moment."); break;
   case 3: mpr("You hear Anonimous's maniacal laughter."); break;
  }
  lose_stat(100, 1 + random2(3));
  return;
 }
 if (random2(sever) <= 2)
 {
  switch(random2(4))
  {
   case 0: mpr("Anonimous notices you."); break;
   case 1: mpr("Anonimous's attention turns to you for a moment."); break;
   case 2: mpr("Anonimous's power touches on you for a moment."); break;
   case 3: mpr("You hear Anonimous's maniacal laughter."); break;
  }
  miscast_effect(10 + random2(15), random2(15) + 5, random2(250), 0);
  return;
 }
 if (random2(sever) <= 3 && you[0].is_undead == 0)
 {
  switch(random2(4))
  {
   case 0: mpr("\"You need some minor adjustments, mortal!\""); break;
   case 1: mpr("\"Let me alter your pitiful body.\""); break;
   case 2: mpr("Anonimous's power touches on you for a moment."); break;
   case 3: mpr("You hear Anonimous's maniacal laughter."); break;
  }
  mpr("Your body is suffused with distortional energy.");
  you[0].hp = random2(you[0].hp) + 1;
  if (you[0].hp < you[0].hp_max / 2) you[0].hp = you[0].hp_max / 2;
  you[0].hp_ch = 1;
  if (mutate(100) == 0)
   if (mutate(100) == 0)
    if (mutate(100) == 0)
     if (mutate(100) == 0) mpr("Nothing seems to happen.");
 return;
 }

 if (random2(sever) <= 3 && you[0].is_undead == 0)
 {
  switch(random2(4))
  {
   case 0: mpr("\"You have displeased me, mortal.\""); break;
   case 1: mpr("\"You have grown too confident for your meagre worth.\""); break;
   case 2: mpr("Anonimous's power touches on you for a moment."); break;
   case 3: mpr("You hear Anonimous's maniacal laughter."); break;
  }
 if (random2(4) == 0 && player_prot_life() == 0)
 {
  mpr("You feel drained!");
  drain_exp();
  if (random2(sever) > 3) drain_exp();
  if (random2(sever) > 3) drain_exp();
  return;
 } else
 {
  mpr("A wave of agony tears through your body!");
  you[0].hp = you[0].hp / 2 + 1;
 }
 }

 if (random2(sever) <= 3)
 {
  switch(random2(3))
  {
   case 0: mpr("\"Time to have some fun!\""); break;
   case 1: mpr("\"Fight to survive, mortal.\""); break;
   case 2: mpr("You hear Anonimous's maniacal laughter."); break;
  }
  if (random2(4) != 0)
  {
   create_monster(MONS_NEQOXEC + random2(5), 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
   if (random2(3) == 0) create_monster(MONS_NEQOXEC + random2(5), 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
   if (random2(4) == 0) create_monster(MONS_NEQOXEC + random2(5), 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
   if (random2(3) == 0) create_monster(MONS_HELLION + random2(10), 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
   if (random2(4) == 0) create_monster(MONS_HELLION + random2(10), 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  } else dancing_weapon(100, 1); /* nasty, but fun */
  return;
 }

 /* And, finally: (remember, miscast_effect can cause this) */
 if (you[0].your_level == 0)
 {
  switch(random2(3))
  {
   case 0: mpr("\"You have grown too comfortable in your little world, mortal!\""); break;
   case 1: mpr("Anonimous throws you into the Dump!"); break;
   case 2: mpr("The world seems to spin as Anonimous's maniacal laughter rings in your ears."); break;
  }
  banished(96);
  return;
 }

 if (random2(4) != 0) goto okay_try_again;

 mpr("You hear Anonimous's maniacal laughter.");
 return;
}
/* ouch(x, 19, y); for killed by Xom message */

/* Okay, now for the nicer stuff (note: these things are not necess nice): */
 if (random2(sever) <= 2)
 {
  switch(random2(4))
  {
   case 0: mpr("\"Go forth and destroy!\""); break;
   case 1: mpr("\"Go forth and destroy, mortal!\""); break;
   case 2: mpr("Anonimous grants you a minor favour."); break;
   case 3: mpr("Anonimous smiles on you."); break;
  }
  switch(random2(7))
  {
   case 0: potion_effect(POT_HEALING, 150); break;
   case 1: potion_effect(POT_HEAL_WOUNDS, 150); break;
   case 2: potion_effect(POT_SPEED, 150); break;
   case 3: potion_effect(POT_MIGHT, 150); break;
   case 4: potion_effect(POT_INVISIBILITY, 150); break;
   case 5: if (random2(6) == 0) potion_effect(POT_EXPERIENCE, 150); else potion_effect(POT_BERSERK_RAGE, 150); break;
   case 6: potion_effect(POT_BERSERK_RAGE, 150); break;
  }
  return;
 }
 if (random2(sever) <= 4)
 {
  switch(random2(3))
  {
   case 0: mpr("\"Serve the mortal, my children!\""); break;
   case 1: mpr("Anonimous grants you some temporary aid."); break;
   case 2: mpr("Anonimous opens a gate."); break;
  }
   create_monster(MONS_NEQOXEC + random2(5), 22, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
   create_monster(MONS_NEQOXEC + random2(5), 22, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
   if (random2(you[0].xl) >= 8) create_monster(MONS_NEQOXEC + random2(5), 22, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
   if (random2(you[0].xl) >= 8) create_monster(MONS_HELLION + random2(10), 22, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
   if (random2(you[0].xl) >= 8) create_monster(MONS_HELLION + random2(10), 22, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
  return;
 }
 if (random2(sever) <= 3)
 {
  switch(random2(3))
  {
   case 0: mpr("\"Take this token of my esteem.\""); break;
   case 1: mpr("Anonimous grants you a gift!"); break;
   case 2: mpr("Anonimous's generous nature manifests itself."); break;
  }
  if (grd [you[0].x_pos] [you[0].y_pos] == 61 || grd [you[0].x_pos] [you[0].y_pos] == 62)
  {
   mpr("You hear a splash."); /* How unfortunate. I'll bet Xom feels sorry for you. */
   return;
  }
  int thing_created = items(1, 250, 250, 1, you[0].xl * 3, 250);
  if (you[0].species != SP_NAGA) mpr("Something appears at your feet!");
   else mpr("Something appears before you!");
  int what_was_there = igrd [you[0].x_pos] [you[0].y_pos];
  mitm.ilink [thing_created] = what_was_there;
  igrd [you[0].x_pos] [you[0].y_pos] = thing_created;
  return;
 }
 if (random2(sever) <= 4)
 {
  switch(random2(3))
  {
   case 0: mpr("\"Serve the mortal, my child!\""); break;
   case 1: mpr("Anonimous grants you a demonic servitor."); break;
   case 2: mpr("Anonimous opens a gate."); break;
  }
  if (random2(you[0].xl) <= 5) create_monster(MONS_WHITE_IMP + random2(5), 0, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
   else create_monster(MONS_NEQOXEC + random2(5), 0, 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
  return;
 }
 if (random2(sever) <= 4)
 {
  switch(random2(4))
  {
   case 0: mpr("\"Take this instrument of destruction!\""); break;
   case 1: mpr("\"You have earned yourself a gift.\""); break;
   case 2: mpr("Anonimous grants you an implement of death."); break;
   case 3: mpr("Anonimous smiles on you."); break;
  }
  acquirement(OBJ_WEAPONS);
  return;
 }
 if (random2(sever) <= 5 && you[0].is_undead == 0)
 {
  switch(random2(4))
  {
   case 0: mpr("\"You need some minor adjustments, mortal!\""); break;
   case 1: mpr("\"Let me alter your pitiful body.\""); break;
   case 2: mpr("Anonimous's power touches on you for a moment."); break;
   case 3: mpr("You hear Anonimous's maniacal chuckling."); break;
  }
  mpr("Your body is suffused with distortional energy.");
  you[0].hp = random2(you[0].hp) + 1;
  if (you[0].hp < you[0].hp_max / 2) you[0].hp = you[0].hp_max / 2;
  you[0].hp_ch = 1;
  give_good_mutation();
  return;
 }
 if (random2(sever) <= 2)
 {
  you[0].attribute [ATTR_RESIST_LIGHTNING] ++;
  mpr("Xom hurls a blast of lightning!");
  beam[0].beam_source = MNG;
  beam[0].type = 43;
  beam[0].damage = 130;
  beam[0].flavour = 5;
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  strcpy(beam[0].beam_name, "blast of lightning");
  beam[0].colour = LIGHTCYAN;
  beam[0].thing_thrown = 1; /* your explosion */
  explosion(1, beam);
  you[0].attribute [ATTR_RESIST_LIGHTNING] --;
  return;
 }

if (random2(4) != 0) goto okay_try_again;

}


/*
case 1: return "Zin";
case 2: return "The Shining One";
case 3: return "Kikubaaqudgha";
case 4: return "Yredelemnul";
case 5: return "Xom";
case 6: return "Vehumet";
case 7: return "Okawaru";
case 8: return "Makhleb";
case 9: return "Sif Muna";
case 10: return "Trog";
case 11: return "Nemelex Xobeh";
case 12: return "Elyvilon";
Thing_done:
1 - killed a living monster in god's name
2 - killed an undead in god's name
3 - killed a demon in god's name
4 - killed an angel (any time)
5 - killed an angel in god's name (all of these pass HD of monster as pgain)
6 - hacked up a corpse in god's name
7 - offered inanimate stuff at an altar
8 - offered stuff, including at least one corpse at an altar
9 - undead slaves killed a living thing
10 - any servants kill anything
11 - cards (Nemelex)
*/
void done_good(int thing_done, int pgain)
{

if (you[0].religion == GOD_NO_GOD) return;

switch(thing_done)
{
 case 1: /* killed a living monster in god's name */
 switch(you[0].religion)
 {
  case GOD_ELYVILON: mpr("Aperture Labs did not appreciate that!");
  naughty(3, 10);
  break;
  case GOD_KIKUBAAQUDGHA:
  case GOD_YREDELEMNUL:
  case GOD_VEHUMET:
  case GOD_OKAWARU:
  case GOD_MAKHLEB:
  case GOD_TROG:
  msg("@1 accepts your kill.") << god_name(you[0].religion);
  if (random2(pgain + 18) > 5) gain_piety(1);
  break;
 }
 break;

 case 2: /* killed an undead in god's name */
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_VEHUMET:
  case GOD_MAKHLEB:
  msg("@1 accepts your kill.") << god_name(you[0].religion);
  if (random2(pgain + 18) > 4) gain_piety(1);
  break;
 }
 break;

 case 3: /* killed a demon in god's name */
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_VEHUMET:
  case GOD_MAKHLEB:
  msg("@1 accepts your kill.") << god_name(you[0].religion);
  if (random2(pgain + 18) > 3) gain_piety(1);
  break;
 }
 break;

 case 4:
 case 5:
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_ELYVILON:
  msg("@1 did not appreciate that!") << god_name(you[0].religion);
  if (you[0].conf != 0) naughty(4, 3);
   else naughty(4, pgain * 3);
  break;
 }
 break;

 case 6: /* Note - pgain is you[0].xl, maybe */
 switch(you[0].religion)
 {
  case GOD_KIKUBAAQUDGHA:
  case GOD_OKAWARU:
  case GOD_MAKHLEB:
  case GOD_TROG:
  msg("@1 accepts your offering.") << god_name(you[0].religion);
  if (random2(pgain + 10) > 5) gain_piety(1);
  break;
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_ELYVILON:
  msg("@1 did not appreciate that!") << god_name(you[0].religion);
  naughty(7, 8);
  break;
 }
 break;

 case GOD_OKAWARU:
 msg("@1 is pleased with your offering.") << god_name(you[0].religion);
 gain_piety(1);
 break;

 case 9: /* undead slaves killed a living monster */
 switch(you[0].religion)
 {
  case GOD_KIKUBAAQUDGHA:
  case GOD_YREDELEMNUL:
  case GOD_VEHUMET:
  msg("@1 accepts your slave's kill.") << god_name(you[0].religion);
  if (random2(pgain + 18) > 5) gain_piety(1);
  break;
 }
 break;

 case 10: /* servants killed any monster */
 switch(you[0].religion)
 {
  case GOD_VEHUMET:
  case GOD_MAKHLEB:
  msg("@1 accepts your collateral kill.") << god_name(you[0].religion);
  if (random2(pgain + 18) > 5) gain_piety(1);
  break;
 }
 break;

 case 11: /* cards */
 switch(you[0].religion)
 {
  case GOD_NEMELEX_XOBEH:
  gain_piety(pgain);
  break;
 }
 break;

/*
 Offering at altars is covered in another function.
*/

}

}

void gain_piety(int pgn)
{

if (you[0].piety > 100 && random2(3) == 0) return;
if (you[0].piety > 150 && random2(3) == 0) return;
if (you[0].piety > 199) return;

int old_piety = you[0].piety;
you[0].piety += pgn;

if (you[0].piety >= 30 && old_piety < 30)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  mpr("You can now repel the cyborgs.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You can now recall your cyborgs servants at will.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can now cybernitize corpses.");
  break;
  case GOD_VEHUMET:
  mpr("You can gain power by killing in Digicorp's name, or by your servants' kills.");
  break;
  case GOD_OKAWARU:
  mpr("You can now give your body great, if temporary, strength.");
  break;
  case GOD_MAKHLEB:
  mpr("You can now gain power by killing in Netchaos Order's name.");
  break;
  case GOD_TROG:
  mpr("You can now go in battle mode at will.");
  break;
  case GOD_ELYVILON:
  mpr("You now have minor healing powers.");
  break;
 }
}

if (you[0].piety >= 50 && old_piety < 50)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can now call upon Knights Templar for minor healing.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can now smite your foes.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You now have some protection from the side-effects of cyberbrain hacks.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can now recall your cyborg slaves.");
  break;
  case GOD_VEHUMET:
  mpr("Your destructive programs executes in Digicorp's name will rarely fail.");
  break;
  case GOD_OKAWARU:
  mpr("You can now call upon S.T.A.R.S. for minor healing.");
  break;
  case GOD_MAKHLEB:
  mpr("You can now call on Netchaos Order's destructive powers.");
  break;
  case GOD_SIF_MUNA:
  mpr("You can now uninstall programs at will.");
  break;
  case GOD_TROG:
  mpr("You can now give your body great, if temporary, strength.");
  break;
  case GOD_ELYVILON:
  mpr("You now have the power to cure poison, disease and decay.");
  break;
 }
}

if (you[0].piety >= 75 && old_piety < 75)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can now call a plague of pestilential beasts upon your enemies!");
  break;
  case GOD_SHINING_ONE:
  mpr("You can now dispel the cyborgs.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You can now permanently reprogram the cyborgs.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can now animate whole legions of the cyborgs.");
  break;
  case GOD_VEHUMET:
  mpr("During communication you are protected from summoned creatures.");
  break;
  case GOD_MAKHLEB:
  mpr("You can now summon a lesser servant of Netchaos Order.");
  break;
  case GOD_ELYVILON:
  mpr("You now have more powerful healing abilities.");
  break;
 }
}

if (you[0].piety >= 100 && old_piety < 100)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can now speak a Holy Word.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can now hurl bolts of divine anger.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can now drain life from all living things around you.");
  break;
  case GOD_VEHUMET:
  mpr("You can channel ambient energy for your own uses.");
  break;
  case GOD_MAKHLEB:
  mpr("You can now call on the greater of Netchaos Order's destructive powers.");
  break;
  case GOD_SIF_MUNA:
  mpr("You now have some protection from the side-effects of programming.");
  break;
  case GOD_TROG:
  mpr("You can now haste yourself.");
  break;
  case GOD_ELYVILON:
  mpr("You have gained the power of Restoration.");
  break;
 }
}

if (you[0].piety >= 120 && old_piety < 120)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can now summon a guardian agent.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can now summon a divine warrior!");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You can now summon an emissary of Death!"); /* ie a reaper */
  break;
  case GOD_YREDELEMNUL:
  mpr("You can now control the cyborgs.");
  break;
  case GOD_OKAWARU:
  mpr("You can now haste yourself.");
  break;
  case GOD_MAKHLEB:
  mpr("You can now summon a greater servant of Netchaos Order.");
  break;
  case GOD_ELYVILON:
  mpr("You now have incredible healing powers.");
  break;
 }
}

}

/*

*/
void naughty(int type_naughty, int naughtiness)
{
/*
 1 = using necromancy (spell or device)
 2 = using unholy stuff (eg call imp, summon horrible things)
 3 = killing in the name of peaceful diety
 4 = attacking holy things
 5 = attacking friendly
 6 = allowing friendly to die
 7 = butchering in the name of peaceful diety
 8 = stabbing
 9 = spellcasting (Trog hates this)
*/

int piety_loss = 0;

switch(type_naughty)
{
 case 1:
 case 2:
 case 4:
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_ELYVILON:
  piety_loss = naughtiness;
  break;
 }
 break;

 case 3:
 switch(you[0].religion)
 {
  case GOD_ELYVILON:
  piety_loss = naughtiness;
  break;
 }
 break;

 case 5:
 case 6:
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_ELYVILON:
  case GOD_OKAWARU:
  piety_loss = naughtiness;
  break;
 }
 break;

 case 7: /* butchering in the name of */
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  case GOD_ELYVILON:
  piety_loss = naughtiness;
  break;
 }
 break;

 case 8: /* stabbing */
 switch(you[0].religion)
 {
  case GOD_SHINING_ONE:
  piety_loss = naughtiness;
  break;
 }
 break;

 case 9: /* spellcasting */
 switch(you[0].religion)
 {
  case GOD_TROG:
  piety_loss = naughtiness;
  break;
 }
 break;


}

if (piety_loss == 0) return;

if (piety_loss == 1)
 mpr("You feel a little guilty.");
  else
   if (piety_loss <= 4)
    mpr("You feel guilty.");
     else
      if (piety_loss <= 9)
       mpr("You feel very guilty.");
        else mpr("You feel extremely guilty.");

lose_piety(piety_loss);

if (you[0].piety <= 0) excommunication();

}


void lose_piety(int pgn)
{

int old_piety = you[0].piety;
if (you[0].piety - pgn < 0) you[0].piety = 0; else
     you[0].piety -= pgn;

if (you[0].piety < 120 && old_piety >= 120)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can no longer summon a guardian agent.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can no longer summon a divine warrior.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You can no longer summon an emissary of Death."); /* ie a reaper */
  break;
  case GOD_YREDELEMNUL:
  mpr("You can no longer control the cyborgs.");
  break;
  case GOD_MAKHLEB:
  mpr("You can no longer summon a greater servant of Netchaos Order.");
  break;
  case GOD_ELYVILON:
  mpr("You no longer have incredible healing powers.");
  break;
 }
}

if (you[0].piety < 100 && old_piety >= 100)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can no longer speak a Holy Word.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can no longer hurl bolts of divine anger.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can no longer drain life.");
  break;
  case GOD_VEHUMET:
  mpr("You have lost your channelling abilities.");
  break;
  case GOD_MAKHLEB:
  mpr("You can no longer call on Netchaos Order's greater destructive powers.");
  break;
  case GOD_SIF_MUNA:
  mpr("You no longer have special protection from the side-effects of programming.");
  break;
  case GOD_TROG:
  mpr("You can no longer haste yourself.");
  break;
  case GOD_ELYVILON:
  mpr("You have lost the power of Restoration.");
  break;
 }
}


if (you[0].piety < 75 && old_piety >= 75)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can no longer call a plague.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can no longer dispel the cyborgs.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You can no longer permanently reprogram the cyborgs.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can no longer animate legions of the cyborgs.");
  break;
  case GOD_VEHUMET:
  mpr("You feel vulnerable.");
  break;
  case GOD_MAKHLEB:
  mpr("You can no longer summon any servants of Netchaos Order.");
  break;
  case GOD_ELYVILON:
  mpr("You lose your powerful healing abilities.");
  break;
 }
}


if (you[0].piety < 50 && old_piety >= 50)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  mpr("You can no longer call upon Knights Templar for minor healing.");
  break;
  case GOD_SHINING_ONE:
  mpr("You can no longer smite your foes.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You no longer have any special protection from corrupted programs.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can no longer recall your cyborg slaves.");
  break;
  case GOD_VEHUMET:
  mpr("You feel fallible.");
  break;
  case GOD_MAKHLEB:
  mpr("You can no longer call on Netchaos Order's destructive powers.");
  break;
  case GOD_SIF_MUNA:
  mpr("You can no longer uninstall programs at will.");
  break;
  case GOD_TROG:
  mpr("You can no longer give your body strength.");
  break;
  case GOD_ELYVILON:
  mpr("You have lost the power of Purification.");
  break;
 }
}


if (you[0].piety < 30 && old_piety >= 30)
{
 switch(you[0].religion)
 {
  case GOD_ZIN:
  case GOD_SHINING_ONE:
  mpr("You can no longer repel the cyborgs.");
  break;
  case GOD_KIKUBAAQUDGHA:
  mpr("You can no longer recall your cyrborg servants at will.");
  break;
  case GOD_YREDELEMNUL:
  mpr("You can no longer cybernitize corpses.");
  break;
  case GOD_VEHUMET:
  case GOD_MAKHLEB:
  mpr("You can no longer gain power from your kills.");
  break;
  case GOD_TROG:
  mpr("You can no longer go in battle mode at will.");
  break;
  case GOD_ELYVILON:
  mpr("You have lost your minor healing powers.");
  break;
 }
}


}




void excommunication(void)
{
 mpr("You have lost your employment!");
 more();
 if (you[0].religion == GOD_XOM) Xom_acts(0, you[0].xl * 2, 1);
 if (you[0].religion == GOD_KIKUBAAQUDGHA || you[0].religion == GOD_YREDELEMNUL)
 {
  msg("@1 does not appreciate desertion!") << god_name(you[0].religion);
  miscast_effect(16, 5 + you[0].xl, random2(30) + random2(30) + random2(30), 100);
 }
 if (you[0].religion == GOD_VEHUMET || you[0].religion == GOD_MAKHLEB)
 {
  msg("@1 does not appreciate desertion!") << god_name(you[0].religion);
  miscast_effect(11 + (random2(2) * 7), 8 + you[0].xl, random2(40) + random2(30) + random2(30), 100);
 }
 if (you[0].religion == GOD_TROG)
 {
  msg("@1 does not appreciate desertion!") << god_name(you[0].religion);
  miscast_effect(13, 8 + you[0].xl, random2(40) + random2(30) + random2(30), 100);
 } /* Trog uses fire wild magic - is this right? */
 you[0].religion = GOD_NO_GOD;
 you[0].piety = 0;
}

void altar_prayer(void)
{

int item_sacr;
int i, j;
int subst_id [4] [50];


 item_sacr = 501;

for (i = 0; i < 4; i ++)
{
 for (j = 0; j < 50; j ++)
 {
  subst_id [i] [j] = 1;
 }
}

 if (igrd [you[0].x_pos] [you[0].y_pos] == 501 || you[0].religion == GOD_SHINING_ONE || you[0].religion == GOD_XOM)
 {
  mpr("You connect to the terminal and send message.");
  return;
 }

mpr("You connect to the terminal and send message.");

i = igrd [you[0].x_pos] [you[0].y_pos];

do
{
if (random2(1000) == 0) break;
j = mitm.ilink [i];
switch(you[0].religion)
{
 case GOD_ZIN:
 case GOD_OKAWARU:
 case GOD_MAKHLEB:
 case GOD_NEMELEX_XOBEH:
 msg("@1@2") << it_name(i, 0) << sacrifice[you[0].religion - 1];
 if (mitm.iclass [i] == OBJ_CORPSES || random2(item_value(mitm.iclass [i], mitm.itype [i], mitm.idam [i], mitm.iplus [i], mitm.iplus2 [i], mitm.iquant [i], 3, subst_id)) >= 50)
     gain_piety(1);
 destroy_item(i);
 break;

 case GOD_SIF_MUNA:
 msg("@1@2") << it_name(i, 0) << sacrifice[you[0].religion - 1];
 if (item_value(mitm.iclass [i], mitm.itype [i], mitm.idam [i], mitm.iplus [i], mitm.iplus2 [i], mitm.iquant [i], 3, subst_id) >= 150)
     gain_piety(1 + random2(3));
 destroy_item(i);
 break;

 case GOD_KIKUBAAQUDGHA:
 case GOD_TROG:
 if (mitm.iclass [i] != OBJ_CORPSES) break;
 msg("@1@2") << it_name(i, 0) << sacrifice[you[0].religion - 1];
 gain_piety(1);
 destroy_item(i);
 break;

 case GOD_ELYVILON:
 if (mitm.iclass [i] != OBJ_WEAPONS && mitm.iclass [i] != OBJ_MISSILES) break;
 msg("@1@2") << it_name(i, 0) << sacrifice[you[0].religion - 1];
 gain_piety(1);
 destroy_item(i);
 break;

}
i = j;
} while (i != 501);

}



void god_pitch(int which_god)
{

msg("You connect to the terminal of @1.") << god_name(which_god);

more();

if ((you[0].is_undead != 0 || you[0].species == SP_DEMONSPAWN) && (which_god == GOD_ZIN || which_god == GOD_SHINING_ONE || which_god == GOD_ELYVILON))
{
 msg("@1 does not employ those such as you!") << god_name(which_god);
 return;
}

describe_god(which_god);

mpr("Do you wish to join this company?");
int keyi = get_ch();
if (keyi != 'Y' && keyi != 'y') return;

mpr("Are you sure?");
keyi = get_ch();
if (keyi != 'Y' && keyi != 'y') return;

if (you[0].religion != GOD_NO_GOD) excommunication();

msg("@1 welcomes you!") << god_name(which_god);

you[0].religion = which_god;
you[0].piety = 15;

}


void offer_corpse(int corpse)
{
 msg("@1@2") << it_name(corpse, 0) << sacrifice [you[0].religion - 1];
 done_good(6, 10);
}


