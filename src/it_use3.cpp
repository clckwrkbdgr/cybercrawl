
#include "linuxlib.h"

#include <algorithm>
#include <string.h>

#include "externs.h"
#include "enum.h"
#include "it_use2.h"
#include "item_use.h"
#include "spells.h"
#include "stuff.h"
#include "itemname.h"
#include "decks.h"
#include "spell.h"
#include "spells2.h"
#include "effects.h"
#include "player.h"
#include "monplace.h"
#include "skills.h"
#include "skills2.h"
#include "bang.h"
#include "view.h"
#include "spells0.h"
#include "spells1.h"
#include "monstuff.h"
#include "items.h"
#include "misc.h"

void ball_of_seeing(void);
void efreet_flask(void);
void disc_of_storms(void);
void ball_of_energy(void);
void ball_of_fixation(void);
void box_of_beasts(void);
void staff_spell(int zap_device_2);

extern int show_green; /* defined in view.cc, I think */
extern int book_thing; /* defined in spells.cc */
extern int wield_change; /* defined in output.cc */

std::string random_singing_sword_song()
{
	switch(random2(32)) {
		case 0:  return "hums a little tune.";
		case 1:  return "breaks into glorious song!";
		case 2:  return "sings.";
		case 3:  return "sings loudly.";
		case 4:  return "chimes melodiously.";
		case 5:  return "makes a horrible noise.";
		case 6:  return "sings off-key.";
		case 7:  return "sings 'tra-la-la'.";
		case 8:  return "burbles away merrily.";
		case 9:  return "gurgles.";
		case 10: return "suddenly shrieks!";
		case 11: return "cackles.";
		case 12: return "warbles.";
		case 13: return "chimes harmoniously.";
		case 14: return "makes beautiful music.";
		case 15: return "produces a loud orchestral chord.";
		case 16: return "whines plaintively.";
		case 17: return "tinkles.";
		case 18: return "rings like a bell.";
		case 19: return "wails mournfully.";
		case 20: return "practices its scales.";
		case 21: return "lilts tunefully.";
		case 22: return "hums tunelessly.";
		case 23: return "sighs.";
		case 24: return "makes a deep moaning sound.";
		case 25: return "makes a popping sound.";
		case 26: return "sings a sudden stoccato note.";
		case 27: return "says 'Hi! I'm the Singing Sword!'.";
		case 28: return "whispers something.";
		case 29: return "speaks gibberish.";
		case 30: return "raves incoherently.";
		case 31: return "yells in some weird language.";
	}
	return "";
}

void special_wielded(void)
{

int old_plus;
int old_plus2;

switch(you[0].special_wield)
{
case SPWLD_SING:
if (random2(20) == 0)
{
 msg("The Singing Sword @1") << random_singing_sword_song();
   noisy(25, you[0].x_pos, you[0].y_pos);
}
break;


case SPWLD_CURSE: // scythe of Curses
if (random2(30) == 0) curse_an_item(0, 0);
break;

case SPWLD_VARIABLE: // mace of variability
if (you[0].inv_plus [you[0].equip [EQ_WEAPON]] > 100) you[0].inv_plus [you[0].equip [EQ_WEAPON]] -= 100;
if (random2(5) != 0) break;
you[0].inv_plus [you[0].equip [EQ_WEAPON]] += random2(2);
you[0].inv_plus [you[0].equip [EQ_WEAPON]] -= random2(2);
you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] += random2(2);
you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] -= random2(2);
if (you[0].inv_plus [you[0].equip [EQ_WEAPON]] < 46) you[0].inv_plus [you[0].equip [EQ_WEAPON]] = 46;
if (you[0].inv_plus [you[0].equip [EQ_WEAPON]] > 57) you[0].inv_plus [you[0].equip [EQ_WEAPON]] = 57;
if (you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] < 46) you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] = 46;
if (you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] > 57) you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] = 57;
you[0].inv_col [you[0].equip [EQ_WEAPON]] = random2(15) + 1;
wield_change = 1;
wield_change = 1;
break;

// 5 - glaive

case SPWLD_TORMENT: // mace of torment
if (random2(200) == 0) torment();
break;

case SPWLD_ZONGULDROK:
if (random2(5) == 0) animate_dead(1 + random2(3), 1, MHITYOU, 1);
break;

case SPWLD_POWER:
old_plus = you[0].inv_plus [you[0].equip [EQ_WEAPON]];
old_plus2 = you[0].inv_plus2 [you[0].equip [EQ_WEAPON]];
you[0].inv_plus [you[0].equip [EQ_WEAPON]] = 47 + you[0].hp / 11;
you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] = 47 + you[0].hp / 11;
if (old_plus != you[0].inv_plus [you[0].equip [EQ_WEAPON]] || old_plus2 != you[0].inv_plus2 [you[0].equip [EQ_WEAPON]])
  wield_change = 1;
break;

case SPWLD_WUCAD_MU:
old_plus = you[0].inv_plus [you[0].equip [EQ_WEAPON]];
old_plus2 = you[0].inv_plus2 [you[0].equip [EQ_WEAPON]];
if (you[0].intel > 25)
{
 you[0].inv_plus [you[0].equip [EQ_WEAPON]] = 72;
 you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] = 63;
} else
{
 you[0].inv_plus [you[0].equip [EQ_WEAPON]] = 47 + you[0].intel;
 you[0].inv_plus2 [you[0].equip [EQ_WEAPON]] = 50 + you[0].intel / 2;
}
if (old_plus != you[0].inv_plus [you[0].equip [EQ_WEAPON]] || old_plus2 != you[0].inv_plus2 [you[0].equip [EQ_WEAPON]])
  wield_change = 1;
break;

case SPWLD_SHADOW: // shadow lamp
if (random2(8) <= 0 + player_spec_death()) create_monster(MONS_SHADOW, 21, 7, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
//naughty(5, 1);
show_green = DARKGREY;
break;

case 51: /* randart makes noises */
if (random2(20) == 0)
{
 msg("@1 lets out a weird humming sound.") << in_name(you[0].equip [EQ_WEAPON], 4);
 noisy(25, you[0].x_pos, you[0].y_pos);
}
break;

case 52: /* randart makes noises */
if (random2(20) == 0)
{
 msg("@1 chimes like a gong.") << in_name(you[0].equip [EQ_WEAPON], 4);
 noisy(25, you[0].x_pos, you[0].y_pos);
}
break;

case 53: /* randart makes noises */
if (random2(20) == 0)
{
 mpr("You hear a voice call your name.");
 noisy(25, you[0].x_pos, you[0].y_pos);
}
break;

case 54: /* randart makes noises */
if (random2(20) == 0)
{
 mpr("You hear a shout.");
 noisy(25, you[0].x_pos, you[0].y_pos);
}
break;

} // end switch

} // end void special_wielded





void invoke_wielded(void)
{
 int opened_gates = 0;
 int spell_casted = random2(21);
 int count_x, count_y;

if (you[0].equip [EQ_WEAPON] == -1)
{
 mpr("You aren't wielding anything!");
 return;
}

if (you[0].berserker != 0)
{
 mpr("You are in battle mode!");
 return;
}

 you[0].turnover = 1;

switch(you[0].inv_class [you[0].equip [EQ_WEAPON]])
{
 case OBJ_WEAPONS:
 switch(you[0].inv_dam [you[0].equip [EQ_WEAPON]])
 {
  case NWPN_STAFF_OF_DISPATER: // staff of Dispater
  if (you[0].deaths_door != 0 || you[0].hp <= 10 || you[0].ep <= 4) goto nothing_hap;
  mpr("You feel the ROM feeding on your energy!");
  you[0].hp -= random2(10) + random2(10) + 5;
  if (you[0].hp <= 0) you[0].hp = 1;
  you[0].ep -= random2(3) + random2(3) + 2;
  if (you[0].ep <= 0) you[0].ep = 1;
  you[0].hp_ch = 1;
  you[0].ep_ch = 1;
  your_spells(SPELL_HELLFIRE, 100, 0); // power (2nd number) is meaningless
  break;

  case NWPN_SCEPTRE_OF_ASMODEUS: // sceptre of Asmodeus
  spell_casted = random2(21);
  if (spell_casted == 0) goto nothing_hap; // nothing happens
  if (spell_casted < 2) // summon devils, maybe a Fiend
  {
   spell_casted = SPELL_SUMMON_ICE_BEAST + random2(10);
   if (random2(4) == 0)
   {
    mpr("\"Your arrogance condemns you, mortal!\"");
    spell_casted = SPELL_RING_OF_FLAMES; /* Fiend! */
   } else mpr("The Sceptre summons one of its servants.");
    create_monster(spell_casted, 25, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
    break;
  }
  spell_casted = SPELL_BOLT_OF_FIRE; // firebolt
  if (random2(3) == 0) spell_casted = SPELL_LIGHTNING_BOLT; // lightning
  if (random2(4) == 0) spell_casted = SPELL_BOLT_OF_DRAINING; // draining
  if (random2(20) == 0) spell_casted = SPELL_HELLFIRE; // hellfire
  your_spells(spell_casted, 10, 0);
  break;

  case NWPN_STAFF_OF_OLGREB: // staff of Olgreb
  if (you[0].ep <= 5 || you[0].skills [SK_SPELLCASTING] <= random2(11)) goto nothing_hap;
  you[0].ep -= 4;
  if (you[0].ep <= 0) you[0].ep = 0;
  you[0].ep_ch = 1;
  your_spells(SPELL_OLGREBS_TOXIC_RADIANCE, 100, 0); // toxic rad
  your_spells(SPELL_VENOM_BOLT, 100, 0); // venom bolt
  break;

  case NWPN_STAFF_OF_WUCAD_MU: // staff of Wucad Mu
  if (you[0].ep == you[0].ep_max || random2(4) == 0)
  {
   mpr("Nothing appears to happen.");
   break;
  }
  you[0].ep += 3 + random2(5);
  if (you[0].ep > you[0].ep_max) you[0].ep = you[0].ep_max;
  you[0].ep_ch = 1;
  mpr("Energy flows into your cyberbrain!");
  if (random2(3) == 0) miscast_effect(19, random2(9), random2(70), 100);
  break;

  default: nothing_hap : mpr("Nothing appears to happen.");
  break;
 }
 break;


 case OBJ_STAVES:
 if (you[0].inv_type [you[0].equip [EQ_WEAPON]] == STAFF_CHANNELING)
 {
  if (you[0].ep == you[0].ep_max || random2(4) == 0)
  {
   mpr("Nothing appears to happen.");
   break;
  }
  you[0].ep += 1 + random2(3);
  if (you[0].ep > you[0].ep_max) you[0].ep = you[0].ep_max;
  you[0].ep_ch = 1;
  mpr("You channel some energy.");
  break;
 }
 staff_spell(you[0].equip [EQ_WEAPON]);
 break;

 case OBJ_MISCELLANY: // misc
 switch(you[0].inv_type [you[0].equip [EQ_WEAPON]])
 {
  case MISC_BOTTLED_EFREET: efreet_flask();
  break;
  case MISC_CRYSTAL_BALL_OF_SEEING: ball_of_seeing();
  break;
  case MISC_AIR_ELEMENTAL_FAN:
  if (random2(2) == 0)
  {
   mpr("Nothing appears to happen.");
   break;
  }
  summon_elemental(100, MONS_AIR_ELEMENTAL, 4);
  break;
  case MISC_LAMP_OF_FIRE:
  if (random2(2) == 0)
  {
   mpr("Nothing appears to happen.");
   break;
  }
  summon_elemental(100, MONS_FIRE_ELEMENTAL, 4);
  break;
  case MISC_STONE_OF_EARTH_ELEMENTALS:
  if (random2(2) == 0)
  {
   mpr("Nothing appears to happen.");
   break;
  }
  summon_elemental(100, MONS_EARTH_ELEMENTAL, 4);
  break;
 case MISC_HORN_OF_GERYON: // Horn of Geryon
 if (you[0].where_are_you == 3)
 {
  mpr("You produce a weird and mournful sound.");
  for (count_x = 0; count_x < 80; count_x ++)
  {
   for (count_y = 0; count_y < 80; count_y ++)
   {
    if (grd [count_x] [count_y] == 98)
    {
     opened_gates ++;
     switch(grd [count_x + 2] [count_y])
     {
      case 67: grd [count_x] [count_y] = 92; break;
      case 61: grd [count_x] [count_y] = 93; break;
      case 1: grd [count_x] [count_y] = 95; break;
      case 62: grd [count_x] [count_y] = 94; break;
     } // Note: This assumes that the Vestibule has not been substantially changed.
    }
   }
  }
  if (opened_gates != 0)
  {
   mpr("Your way has been unbarred.");
  }
 break;
 } else
 {
   mpr("You produce a hideous howling noise!");
   create_monster(MONS_BEAST, 23, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
 }
 break;

 case MISC_BOX_OF_BEASTS: // box of beasts
 box_of_beasts();
 break;

 case MISC_DECK_OF_WONDERS: // deck of wonders
 deck_of_cards(0);
 break;

 case MISC_DECK_OF_SUMMONINGS: // deck of summonings
 deck_of_cards(1);
 break;

 case MISC_CRYSTAL_BALL_OF_ENERGY: // crystal ball of energy
 ball_of_energy();
 break;

 case 12: // crystal ball of fixation
 ball_of_fixation();
 break;

 case MISC_DISC_OF_STORMS: // disc of storms
 disc_of_storms();
 break;

 case MISC_DECK_OF_TRICKS: // deck of tricks
 deck_of_cards(2);
 break;

 case MISC_DECK_OF_POWER: // deck of power
 deck_of_cards(3);
 break;

 case MISC_PORTABLE_ALTAR_OF_NEMELEX: // card table
 if (you[0].where_are_you == 18)
 {
  mpr("Don't you think this level already has more than enough terminals?");
  return;
 }
 if (grd [you[0].x_pos] [you[0].y_pos] != 67)
 {
  mpr("You need a piece of empty floor to place this item.");
  break;
 }
 mpr("You unfold the terminal and place it on the floor.");
 grd [you[0].x_pos] [you[0].y_pos] = 190;
 unwield_item(you[0].equip [EQ_WEAPON]);
 you[0].inv_quant [you[0].equip [EQ_WEAPON]] = 0;
 you[0].equip [EQ_WEAPON] = -1;
 you[0].inv_no --;
 break;

 default: mpr("Nothing appears to happen.");
 break;
 }
 break;


 default: mpr("Nothing appears to happen.");
 break;

}

you[0].turnover = 1;

} // end invoke_wielded



void efreet_flask(void)
{

	mpr("You open the flask, and a huge walking plasma turret comes out.");

unwield_item(you[0].equip [EQ_WEAPON]);
you[0].inv_quant [you[0].equip [EQ_WEAPON]] = 0;
you[0].equip [EQ_WEAPON] = -1;
you[0].inv_no --;

if (random2(5) != 0)
{
  mpr("\"Thank you for releasing me!\"");
  create_monster(MONS_EFREET, 24, 7, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  return;
}

  mpr("It howls insanely!");
  create_monster(MONS_EFREET, 24, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);

} // end efreet_flask

void ball_of_seeing(void)
{
 int use = 0;
 mpr("You gaze into the crystal ball.");
 use = random2(you[0].intel * 6); //magic_ability(you[0].intel, you[0].intel));
 if (you[0].conf != 0) use = 0;
 if (use < 2)
 {
  mpr("Your head hurts!");
  if (you[0].intel > 3) you[0].intel --;
  you[0].intel_ch = 1;
  return;
 }
 if (use < 5)
 {
  mpr("You feel power drain from you!");
  you[0].ep = 0;
  you[0].ep_ch = 1;
  return;
 }
 if (use < 10)
 {
  mpr("You feel confused.");
  you[0].conf += 10 + random2(10);
  if (you[0].conf > 40) you[0].conf = 40;
  return;
 }
 if (use < 15 || random2(2) == 0 || you[0].level_type == 1 || you[0].level_type == 2)
 {
  mpr("You see nothing.");
  return;
 }

 mpr("You see a map of your surroundings!");
 magic_mapping(80, 95 + random2(10));

} // end ball_of_seeing()

void disc_of_storms(void)
{

you[0].turnover = 1;

if ((random2(60) > 30 + you[0].skills [SK_AIR_MAGIC] && random2(3) != 0) || you[0].attribute [ATTR_SPEC_AIR] != 0)
{
 mpr("Nothing appears to happen.");
 return;
}

if (random2(60) > 30 + you[0].skills [SK_AIR_MAGIC] && random2(3) != 0)
{
 mpr("The disc glows for a moment, then fades.");
 return;
}

if (random2(60) > 30 + you[0].skills [SK_AIR_MAGIC] && random2(3) != 0)
{
 mpr("Little bolts of electricity crackle over the disc.");
 return;
}

	mpr("The disc erupts in an explosion of electricity!");

int disc_count = 0;
int disc_count2 = 0;
int which_zap_thing = 0;

disc_count2 = 2 + random2(4);

do
{

 switch(random2(3))
 {
  case 0: which_zap_thing = 14; break; // lightning bolt
  case 1: which_zap_thing = 34; break; // shock
  case 2: which_zap_thing = 35; break; // orb of elec
 }
 struct bolt beam [1];
 beam[0].target_x = 0;
 beam[0].target_y = 0;
 beam[0].move_x = random2(13) - 6;
 beam[0].move_y = random2(13) - 6;
 beam[0].source_x = you[0].x_pos;
 beam[0].source_y = you[0].y_pos;
 zapping(which_zap_thing, 30 + random2(20), beam);
 disc_count ++;

} while (disc_count <= disc_count2);

}





void staff_spell(int zap_device_2)
{
if (you[0].inv_plus [zap_device_2] == 64 || you[0].inv_type [zap_device_2] < STAFF_SMITING || you[0].inv_type [zap_device_2] >= STAFF_AIR)
{
   mpr("That ROM has no spells in it.");
   return;
}


read_book(zap_device_2);
int specspell;
int powc = magic_ability(player_mag_abil(), you[0].intel);

int sc_read_1 = book_thing; // book_thing is got in read_book

if (sc_read_1 < 65 || (sc_read_1 > 90 && sc_read_1 < 97) || sc_read_1 > 122)
{
	mpr("What?");
	return;
}


int sc_read_2 = conv_lett(sc_read_1);


if (sc_read_2 > 7)
{
   goto whattt;
}


if (learn_a_spell(zap_device_2, sc_read_2) != 1)
{
   goto whattt;
}



specspell = which_spell_in_book(you[0].inv_type [you[0].equip [EQ_WEAPON]] + 40, sc_read_2 + 1);

if (you[0].ep < spell_value(specspell) || you[0].xl < spell_value(specspell))
{
   mpr("Your brain hurts!");
   you[0].conf += 2 + random2(4);
   if (you[0].conf > 40) you[0].conf = 40;
   you[0].turnover = 1;
   return;
}

// note that spell staves cannot have empty spaces in strungy before the last real spell.

exercise_spell(specspell, 1, 1);
your_spells(specspell, powc, 0);

you[0].ep -= spell_value(specspell);
you[0].ep_ch = 1;

//if (you[0].inv_type [you[0].equip [0]] != 10) // staff of smiting uses no food
/*
if (you[0].energy == 0)
{
 spellh = spell_hunger(spell_value(specspell), specspell);
 spellh -= magic_ability(you[0].mag_abil, you[0].intel);
 if (spellh < 10) spellh = 10;
 you[0].hunger -= spellh;
 hung_ch = 1;
 how_hungered(spellh);
}
*/
you[0].turnover = 1;

return;

whattt:
	mpr("What?");

} // end staff_spell



void tome_of_power(int sc_read_2)
{
int powc = 0;
int spell_casted = 0;
int keyin = 0;
struct bolt beam [1];


msg( "The archive opens to a screen covered in @1.") << weird_writing();
	mpr("Read it?");

question : keyin = get_ch();

switch (keyin)
{
 case 'y':
 case 'Y':
 goto read_it;

 case 'n':
 case 'N':
 you[0].turnover = 1;
 return;

 default: goto question;
}


read_it:

you[0].inv_ident [sc_read_2] = 3;
you[0].turnover = 1;

if (you[0].mutation [MUT_BLURRY_VISION] > 0 && random2(4) < you[0].mutation [MUT_BLURRY_VISION])
{
 mpr("The screen is too blurry for you to read.");
 you[0].turnover = 1;
 return;
}

powc = magic_ability(player_mag_abil(), you[0].intel);

	mpr("You find yourself reciting the program code!");

switch(random2(50))
{
case 0:
case 11:
case 12:
        if (random2(5) == 0)
        {
        mpr("The archive disappears in a mighty explosion!");
        you[0].inv_quant [sc_read_2] = 0;
        you[0].inv_no --;
        if (sc_read_2 == you[0].equip [EQ_WEAPON])
        {
           unwield_item(sc_read_2);
           you[0].equip [EQ_WEAPON] = -1;
        }
           burden_change();
        }
	beam[0].type = 43;
	beam[0].damage = 115;
	beam[0].flavour = 2; // <=- not sure about this
	beam[0].bx = you[0].x_pos;
	beam[0].by = you[0].y_pos;
	strcpy(beam[0].beam_name, "fiery explosion");
	beam[0].colour = RED;
	beam[0].thing_thrown = 1; // your explosion, (not someone else's explosion)
	explosion(1, beam);
        return;

case 1:
case 14:
	mpr("A cloud of choking fumes pours from the archive's screen!");
big_cloud(4, you[0].x_pos, you[0].y_pos, 20);
//if (random() % 3 == 0) return; else break;
return;

case 2:
case 13:
	mpr("A cloud of freezing gas pours from the archive's screen!");
big_cloud(3, you[0].x_pos, you[0].y_pos, 20);
//if (random() % 3 == 0) return; else break;
return;

case 3:
case 4:
case 5:
case 6:
case 7:
case 8:
case 9:
	mpr("A cloud of weird smoke pours from the acrhive's screen!");
big_cloud(5 + random2(3), you[0].x_pos, you[0].y_pos, 20);
return;// else break;

case 10:
	mpr("A horrible Thing appears!");
	mpr("It doesn't look too friendly.");
create_monster(MONS_LARGE_ABOMINATION, 25, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
return;

// next is 14

}

viewwindow(1);

switch(random2(23))
{
 case 0: spell_casted = SPELL_MAGIC_DART; break;
 case 16:
 case 17:
 case 1: spell_casted = SPELL_FIREBALL; break; // fireball
 case 21:
 case 18:
 case 2: spell_casted = SPELL_BOLT_OF_FIRE; break;
 case 22:
 case 19:
 case 3: spell_casted = SPELL_BOLT_OF_COLD; break;
 case 20:
 case 4: spell_casted = SPELL_LIGHTNING_BOLT; break;
 case 5: spell_casted = SPELL_POLYMORPH_OTHER; break;
 case 6: spell_casted = SPELL_THROW_FLAME; break;
 case 7: spell_casted = SPELL_THROW_FROST; break;
 case 8: spell_casted = SPELL_MEPHITIC_CLOUD; break;
 case 9: spell_casted = SPELL_VENOM_BOLT; break;
 case 10: spell_casted = SPELL_BOLT_OF_DRAINING; break;
 case 11: spell_casted = SPELL_LEHUDIBS_CRYSTAL_SPEAR; break;
 case 12: spell_casted = SPELL_BOLT_OF_INACCURACY; break;
 case 13: spell_casted = SPELL_STICKY_FLAME; break;
 case 14: spell_casted = SPELL_CIGOTUVIS_DEGENERATION; break;
 case 15: spell_casted = SPELL_TELEPORT_SELF; break;
}

// also summon demons (when they're finished)

your_spells(spell_casted, powc, 0);
// note no exercise

}

std::string string_tolower(const std::string & value)
{
	std::string result = value;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

void skill_manual(int sc_read_2)
{
int keyin;

msg("This is a manual of @1!") << skill_name(you[0].inv_plus [sc_read_2]);

	mpr("Read it?");

question : keyin = get_ch();

switch (keyin)
{
 case 'y':
 case 'Y':
 goto read_it;

 case 'n':
 case 'N':
 you[0].turnover = 1;
 return;

 default: goto question;
}


read_it:

you[0].inv_ident [sc_read_2] = 3;
you[0].turnover = 1;

msg("You read about @1.") << string_tolower(skill_name(you[0].inv_plus [sc_read_2]));

exercise(you[0].inv_plus [sc_read_2], 100);

if (random2(10) == 0)
{
        mpr("The archive destroys into dust.");
        you[0].inv_quant [sc_read_2] = 0;
        you[0].inv_no --;
        if (sc_read_2 == you[0].equip [EQ_WEAPON])
        {
         unwield_item(sc_read_2);
         you[0].equip [EQ_WEAPON] = -1;
        }
        burden_change();
} else
{
 mpr("The archive looks increasingly old and worn.");
}


} // end skill_manual



void box_of_beasts(void)
{
	you[0].turnover = 1;
	if (random2(3) == 0) {
		msg("You open the lid, but nothing happens.");
		return;
	}
	if (random2(10) == 0) {
		msg("You open the lid, but nothing happens.");
		you[0].inv_type [you[0].equip [EQ_WEAPON]] = MISC_EMPTY_EBONY_CASKET;
		return;
	}
	msg("You open the lid, and something leaps out!");
	int beasty = 0;
	switch(random2(11)) {
		case 0: beasty = MONS_GIANT_BAT; break; // bat
		case 1: beasty = MONS_HOUND; break; // hound
		case 2: beasty = MONS_JACKAL; break; // jackal
		case 3: beasty = MONS_RAT; break; // rat
		case 4: beasty = MONS_ICE_BEAST; break; // ice beast
		case 5: beasty = MONS_SNAKE; break; // snake
		case 6: beasty = MONS_YAK; break; // yak
		case 7: beasty = MONS_BUTTERFLY; break; // butterfly
		case 8: beasty = MONS_HELL_HOUND; break; // hell hound
		case 9: beasty = MONS_BROWN_SNAKE; break; // brown snake
		case 10: beasty = MONS_GIANT_LIZARD; break; // giant lizard
	}
	create_monster(beasty, 21 + random2(4), 7, you[0].x_pos, you[0].y_pos, you[0].pet_target, 250);
}

void ball_of_energy(void)
{

 int use = 0;
 int proportional = 0;

 mpr("You gaze into the crystal ball.");
 use = random2(you[0].intel * 6);
 if (you[0].conf != 0) use = 0;
 if (use < 2 || you[0].ep_max == 0)
 {
  mpr("Your head hurts!");
  if (you[0].intel > 3) you[0].intel --;
  you[0].intel_ch = 1;
  return;
 }
 if (use < 4 || you[0].ep == you[0].ep_max)
 {
  drain_away: mpr("You feel your power drain away!");
  you[0].ep = 0;
  you[0].ep_ch = 1;
  return;
 }
 if (use < 6)
 {
  mpr("You feel confused.");
  you[0].conf += 10 + random2(10);
  if (you[0].conf > 40) you[0].conf = 40;
  return;
 }

 proportional = you[0].ep * 100;
 proportional /= you[0].ep_max;

 if (random2(20) + random2(20) + random2(20) + random2(10) > proportional || random2(25) == 0)
    goto drain_away;

 mpr("You are suffused with power!");
 you[0].ep += 12 + random2(12) + random2(12);
 if (you[0].ep > you[0].ep_max) you[0].ep = you[0].ep_max;
 you[0].ep_ch = 1;
}

void ball_of_fixation(void)
{

 mpr("You gaze into the crystal ball.");
 mpr("You are mesmerised by a rainbow of scintillating colours!");

 you[0].paralysis = 100;
 you[0].slow = 100;

}


