#include <string.h>
#include "linuxlib.h"


#include <strings.h>

#include "externs.h"
#include "enum.h"
#include "beam.h"
#include "direct.h"
#include "files.h"
#include "fight.h"
#include "player.h"
#include "ouch.h"
#include "mstruct.h"
#include "misc.h"
#include "mutation.h"
#include "player.h"
#include "stuff.h"
#include "dungeon.h"
#include "itemname.h"
#include "skills2.h"
#include "view.h"
#include "spells0.h"
#include "spells2.h"
#include "spells3.h"

void torment(void)
{
 int dmi = 0;

 if (you[0].is_undead != 0 || you[0].mutation [MUT_TORMENT_RESISTANCE] != 0)
 {
  mpr("You feel a surge of unholy energy.");
 } else
       {
        mpr("Your body is wracked with pain!");
        //you[0].hp -= (you[0].hp / 2) - 1;
        int tormdam = (you[0].hp / 2) - 1;
        if (tormdam <= 0) tormdam = 0;
        ouch(tormdam, 0, 0);
        you[0].hp_ch = 1;
       }

        for (dmi = 0; dmi < MNST; dmi++)
        {
                if (menv [dmi].m_class == -1) continue;

                if (mons_near(dmi) != 1) continue;

//              int mres = random2(100);
//              int mres2 = mon_resist_mag(mons_class [dmi], mons_HD [dmi]);

//              if (mres < mres2) continue;

  if (mons_holiness(menv [dmi].m_class) > 0) continue;

                // maybe an exemption for undead?
                // maybe do something fun for magic circles?

                menv [dmi].m_hp -= (menv [dmi].m_hp / 2) - 1;

                if (menv [dmi].m_ench [2] == 6) continue; // can't see it

                msg("@1 convulses!") << monam (menv [dmi].m_sec, menv [dmi].m_class, menv [dmi].m_ench [2], 0);
        }

}


void banished(int gate_type)
{
 you_teleport2(0); // this is to ensure that you're standing on a suitable space (67)
 grd [you[0].x_pos] [you[0].y_pos] = gate_type;
 down_stairs(1, you[0].your_level); // heh heh
}


int forget_spell(void)
{
int spc2;
if (you[0].spell_no <= 0) return 0;

do
{
 spc2 = random2(20);
 if (random2(1000) == 0) return 0; // safety check
} while (you[0].spells [spc2] == 210);


 you[0].spell_levels += spell_value(you[0].spells [spc2]);
 you[0].spell_no --;
 you[0].spells [spc2] = 210;
 return 1;
}


int lose_stat(int force_stat, int stat_loss)
{

if (force_stat == 100) force_stat = random2(3);

  if (player_sust_abil() != 0)
  {
   mpr("You feel slightly ill for a moment.");
   return 0;
  }
     	switch(force_stat)
	{
		case STAT_STRENGTH: mpr("You feel weakened.");
		you[0].strength -= stat_loss;
		if (you[0].strength < 3) you[0].strength = 3;
		you[0].strength_ch = 1;
		burden_change();
		break;

		case STAT_DEXTERITY: mpr("You feel clumsy.");
		you[0].dex -= stat_loss;
		if (you[0].dex < 3) you[0].dex = 3;
		you[0].dex_ch = 1;
		break;

		case STAT_INTELLIGENCE: mpr("You feel dopey.");
		you[0].intel -= stat_loss;
		if (you[0].intel < 3) you[0].intel = 3;
		you[0].intel_ch = 1;
		break;

        }

return 1;

}





void direct_effect(struct bolt beam [1])
{
int hurted = 0;

switch(beam[0].type)
{
 case DMNBM_HELLFIRE: // hellfire
 mpr("You are engulfed in a burst of hellfire!");
 hurted = random2(10) + random2(10) + 5;
 beam[0].flavour = 20; // lava, but it's hellfire anyway
 strcpy(beam[0].beam_name, "hellfire"); // for ouch
 hurted = check_your_resists(hurted, beam[0].flavour);
 scrolls_burn(4, 6);
 ouch(hurted, beam[0].beam_source, 3);
 you[0].hp_ch = 1;
 break;

 case DMNBM_SMITING: // smiting
 mpr("Something smites you!");
 hurted = random2(6) + random2(6) + 7;
 strcpy(beam[0].beam_name, "smiting"); // for ouch
 ouch(hurted, beam[0].beam_source, 3);
 you[0].hp_ch = 1;
 break;

 case DMNBM_BRAIN_FEED: // brain feed
      if (random2(3) == 0 && you[0].intel > 3 && player_sust_abil() == 0)
      {
        mpr("Something feeds on your intelligence!");
        mpr("You feel stupid.");
        you[0].intel --;
        you[0].intel_ch = 1;
      } else {
                mpr("Something tries to feed on your intelligence!");
     	     }
 break;

 case DMNBM_MUTATION: // mutation
 if (mutate(100) == 0) mpr("You feel very strange for a moment.");
 break;



}

return;

}


void mons_direct_effect(struct bolt beam [1], int i)
{

int o = menv [i].m_hit;
int hurted = 0;

switch(beam[0].type)
{


  case DMNBM_HELLFIRE:
  if (mons_near(menv [i].m_hit) == 1)
  {
 		msg("@1 is engulfed in hellfire.") << monam (menv [o].m_sec, menv [o].m_class, menv [o].m_ench [2], 0);
  }
  strcpy(beam[0].beam_name, "hellfire");
  beam[0].flavour = 20;
  hurted = 0;
  hurted += random2(10) + random2(5) + 5;
  check_mons_resists(beam, o, hurted);
  menv [o].m_hp -= hurted;
  if (menv [o].m_beh == 0) menv [o].m_beh = 1;
  if (menv [o].m_hp <= 0)
  {
 		monster_die(o, 4, i);
  }
  break;

  case DMNBM_SMITING:
  if (mons_near(o) == 1)
  {
 		msg("@1 is smitten.") << monam (menv [o].m_sec, menv [o].m_class, menv [o].m_ench [2], 0);
  }
  strcpy(beam[0].beam_name, "smiting");
  beam[0].flavour = 0;
  hurted = 0;
  hurted += random2(6) + random2(6) + 7;
  menv [o].m_hp -= hurted;
  if (menv [o].m_hp <= 0)
  {
    monster_die(o, 4, i);
  }
  break;


  case DMNBM_BRAIN_FEED: // brain feed - not implemented here (nor, probably, can be)
		break;

} // end switch

} // end monster_direct_effect()


std::string random_roaring_monster()
{
	switch(random2(8)) {
		case 0: return "frog";
		case 1: return "pill bug";
		case 2: return "millipede";
		case 3: return "eggplant";
		case 4: return "albino dragon";
		case 5: return "dragon";
		case 6: return "human";
		case 7: return "slug";
	}
	return "";
}

std::string random_smell()
{
	switch(random2(8)) {
		case 0: return "coffee";
		case 1: return "salt";
		case 2: return "burning hair";
		case 3: return "baking bread";
		case 4: return "something weird";
		case 5: return "wet wool";
		case 6: return "sulphur";
		case 7: return "fire and brimstone";
	}
	return "";
}

std::string random_itching()
{
	switch(random2(3)) {
		case 0: return "ears itch";
		case 1: return "brain hurts";
		case 2: return "nose twitches suddenly";
	}
	return "";
}

std::string random_sound()
{
	switch(random2(9)) {
		case 0: return "snatches of song";
		case 1: return "a voice call someone else's name";
		case 2: return "a very strange noise";
		case 3: return "roaring flame";
		case 4: return "a very strange noise indeed";
		case 5: return "the chiming of a distant gong";
		case 6: return "the bellowing of a yak";
		case 7: return "a crunching sound";
		case 8: return "the tinkle of an enormous bell";
	}
	return "";
}

void random_uselessness(int ru, int sc_read_2)
{
	switch(ru) {
		case 0:
			msg("The dust glows a @1 colour!") << weird_colours(random2(256));
			break;
		case 1:
			mpr("The device reassembles itself in your hand!");
			you[0].inv_quant [sc_read_2]++;
			burden_change();
			break;
		case 2:
			if (you[0].equip [EQ_WEAPON] != -1) {
				in_name(you[0].equip [EQ_WEAPON], 4, str_pass);
				msg("@1 glows @2 for a moment.") << str_pass << weird_colours(random2(256));
				return;
			} else {
				msg("Nothing appears to happen.");
				return;
			}
		case 3:
			msg("You hear the distant roaring of an enraged @1!") << random_roaring_monster();
			return;
		case 4:
			msg("You smell @1.") << random_smell();
			return;
		case 5:
			mpr("You experience a momentary feeling of inescapable doom!");
			return;
		case 6:
			msg("Your @1!") << random_itching();
			return;
		case 7:
			mpr("You hear the tinkle of a tiny bell.");
			summon_butter();
			return;
		case 8:
			msg("You hear @1.") << random_sound();
			return;
	}
}


void acquirement(int force_class)
{
int thing_created = 0;
// Remember lava!
int class_wanted = 250;
int type_wanted = 250;

int force_plus = 0;

int acqc = 0;
int already_has [50];

int glof = 99;
int keyin;
int func_pass [10];

for (acqc = 0; acqc < 50; acqc ++)
{
 already_has [acqc] = 0;
}

if (force_class == 250)
{

 mpr("This is a device of 3D printing!");
 query :
 mpr("a - Weapon, b - Armour, c - Implant,");
 mpr("d - Archive, e - ROM, f - Miscellaneous");
 mpr("What kind of item would you like to print? ");
 keyin = get_ch();

 switch(keyin)
 {
  case 'a':
  case 'A':
  class_wanted = OBJ_WEAPONS;
  break;

  case 'b':
  case 'B':
  class_wanted = OBJ_ARMOUR;
  break;

  case 'c':
  case 'C':
  class_wanted = OBJ_JEWELLERY;
  break;

  case 'd':
  case 'D':
  class_wanted = OBJ_BOOKS;
  break;

  case 'e':
  case 'E':
  class_wanted = OBJ_STAVES;
  break;

  case 'f':
  case 'F':
  class_wanted = OBJ_MISCELLANY;
  break;
 }
} else class_wanted = force_class;

if (class_wanted > OBJ_ARMOUR)
{
for (acqc = 0; acqc < 52; acqc ++)
{
 if (you[0].inv_quant [acqc] > 0 && you[0].inv_class [acqc] == class_wanted)
 {
  already_has [you[0].inv_type [acqc]] = 1;

/*  if (class_wanted == 10)
  {
   if (you[0].inv_type [acqc] == 0)
   {
    already_has [1] = 1;
    already_has [2] = 1;
   }
   if (you[0].inv_type [acqc] == 1)
   {
    already_has [0] = 1;
    already_has [2] = 1;
   }
   if (you[0].inv_type [acqc] == 3) already_has [4] = 1;
   if (you[0].inv_type [acqc] == 4) already_has [3] = 1;
  }*/
 }
}

/*if (class_wanted == 10) // these are books which that class wouldn't want (or shouldn't get, anyway)
{
/ * if (you[0].clas == JOB_NECROMANCER)
 {
  already_has [20] = 1; // holy books
  already_has [17] = 1;
  already_has [18] = 1;
 }
 if (you[0].clas == JOB_FIRE_ELEMENTALIST)
 {
  already_has [6] = 1;
  already_has [9] = 1;
 }
 if (you[0].clas == JOB_ICE_ELEMENTALIST)
 {
  already_has [5] = 1;
  already_has [8] = 1;
 }* /
 if (you[0].clas == JOB_PRIEST || you[0].clas == JOB_PALADIN)
 {
  already_has [15] = 1; // necromancy
  already_has [23] = 1;
  already_has [24] = 1;
  already_has [34] = 1;
 }
}*/

do
{
 switch(class_wanted)
 {

  case OBJ_JEWELLERY:
  type_wanted = 250;
  break;

  case OBJ_BOOKS:

// remember, put rarer books higher in the list

 type_wanted = 99;

glof = best_skill(26, 50, glof);

which_book :
 switch(glof)
 {
  default:
  case SK_SPELLCASTING: // spellcasting
  if (you[0].had_item [43] == 0) type_wanted = 43;
  if (you[0].had_item [42] == 0) type_wanted = 42;
  break;

  case SK_POISON_MAGIC: // Poison Magic
  if (you[0].had_item [32] == 0) type_wanted = 32;
  if (you[0].had_item [13] == 0) type_wanted = 13;
  break;

  case SK_EARTH_MAGIC: // Earth
  if (you[0].had_item [40] == 0) type_wanted = 40;
  if (you[0].had_item [39] == 0) type_wanted = 39;
  break;

  case SK_AIR_MAGIC: // Air
  if (you[0].had_item [29] == 0) type_wanted = 29;
  if (you[0].had_item [28] == 0) type_wanted = 28;
  break;

  case SK_ICE_MAGIC: // Ice
  if (you[0].had_item [9] == 0) type_wanted = 9;
  if (you[0].had_item [6] == 0) type_wanted = 6;
  break;

  case SK_FIRE_MAGIC: // fire
  if (you[0].had_item [8] == 0) type_wanted = 8;
  if (you[0].had_item [5] == 0) type_wanted = 5;
  break;

  case SK_SUMMONINGS: // summ
  if (you[0].had_item [27] == 0) type_wanted = 27;
  if (you[0].had_item [7] == 0) type_wanted = 7;
  if (you[0].had_item [25] == 0) type_wanted = 25;
  break;

  case SK_ENCHANTMENTS: // ench
  if (you[0].had_item [12] == 0) type_wanted = 12;
  if (you[0].had_item [20] == 0) type_wanted = 20;
  if (you[0].had_item [36] == 0) type_wanted = 36;
  if (you[0].had_item [16] == 0) type_wanted = 16;
  if (you[0].had_item [26] == 0) type_wanted = 26;
  break;

  case SK_CONJURATIONS: // conj
  if (you[0].had_item [33] == 0) type_wanted = 33;
  if (you[0].had_item [3] == 0) type_wanted = 3;
  break;

  case SK_NECROMANCY: // necro
  if (you[0].had_item [24] == 0) type_wanted = 24;
  if (you[0].had_item [34] == 0) type_wanted = 34;
  if (you[0].had_item [15] == 0) type_wanted = 15;
  if (you[0].had_item [23] == 0) type_wanted = 23;
  break;

  case SK_TRANSLOCATIONS: // translocations
  if (you[0].had_item [11] == 0) type_wanted = 11;
  if (you[0].had_item [31] == 0) type_wanted = 31;
  break;

  case SK_TRANSMIGRATION: // transmutation
  if (you[0].had_item [17] == 0) type_wanted = 17;
  if (you[0].had_item [18] == 0) type_wanted = 18;
  break;


 }

if (type_wanted == 99 && glof == best_skill(26, 50, 99)) // && best_skill(25, 50, 99) != best_skill(26, 50, 99))
{
 glof = best_skill(26, 50, best_skill(26, 50, 99));
 goto which_book;
}


if (type_wanted == 99)
  do
  {
   type_wanted = random2(27);
   if (random2(500) == 0) break;
  } while (you[0].had_item [type_wanted] == 1);


  force_plus = 127;

  // should guarantee a full spellbook, so:
  spellbook_template(type_wanted, func_pass);
  if (func_pass [6] == 210) force_plus = 126;
  if (func_pass [5] == 210) force_plus = 124;
  if (func_pass [4] == 210) force_plus = 120;
  if (func_pass [3] == 210) force_plus = 112;
  if (func_pass [2] == 210) force_plus = 96;

  break;

  case OBJ_STAVES:
  type_wanted = random2(18);
  if (class_wanted == OBJ_STAVES && type_wanted > 9 && random2(5) != 0) type_wanted = random2(10);
  break;

  case OBJ_MISCELLANY:
  type_wanted = random2(6);
  break;

  default: mesclr();
  goto query;
 }
} while(already_has [type_wanted] == 1 && random2(200) != 0);

} else
{
 if (class_wanted == OBJ_WEAPONS) type_wanted = 250; // 1 + random2(18); // weapons - no clubs
  else type_wanted = 250; // always get random armour
}

if (grd [you[0].x_pos] [you[0].y_pos] == 61 || grd [you[0].x_pos] [you[0].y_pos] == 62)
{
 mpr("You hear a splash."); // how sad (and stupid)
 return;
}

thing_created = /*items(you[0].unique_items, 1, you[0].item_description,
       grd, class_wanted, type_wanted, 1, 351, 250);*/
		items(1, class_wanted, type_wanted, 1, 351, 250);


if (you[0].species != SP_NAGA) mpr("Something appears at your feet!");
 else mpr("Something appears before you!");

if (force_plus != 0) mitm.iplus [thing_created] = force_plus;

int what_was_there = igrd [you[0].x_pos] [you[0].y_pos];
mitm.ilink [thing_created] = what_was_there;
igrd [you[0].x_pos] [you[0].y_pos] = thing_created;

return;

}



int recharge_wand(void)
{

// note that the scroll of recharging also recharges weapons of electrocution; see the scroll function

if (you[0].equip [EQ_WEAPON] == -1)
{
 return 0;
}

if (you[0].inv_class [you[0].equip [EQ_WEAPON]] != OBJ_WANDS)
{
 return 0; // not a wand
}

int charge_gain = 8;

if (you[0].inv_type [you[0].equip [EQ_WEAPON]] == WAND_FIRE || you[0].inv_type [you[0].equip [EQ_WEAPON]] == WAND_COLD)
 charge_gain = 5;

if (you[0].inv_type [you[0].equip [EQ_WEAPON]] == WAND_FIREBALL || you[0].inv_type [you[0].equip [EQ_WEAPON]] == WAND_LIGHTNING || you[0].inv_type [you[0].equip [EQ_WEAPON]] == WAND_DRAINING)
 charge_gain = 4;

item_name(you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_quant [you[0].equip [EQ_WEAPON]], you[0].inv_ident [you[0].equip [EQ_WEAPON]], 4, str_pass);
msg("@1 glows for a moment.") << str_pass;

you[0].inv_plus [you[0].equip [EQ_WEAPON]] += random2(charge_gain) + random2(charge_gain) + random2(charge_gain) + 1;

if (you[0].inv_plus [you[0].equip [EQ_WEAPON]] > charge_gain * 3)
 you[0].inv_plus [you[0].equip [EQ_WEAPON]] = charge_gain * 3;

return 1;

}

void yell(void)
{

int targ_prev = 0;
int mons_targd = 0;
struct dist beam [1];

mpr("What do you say?");
mpr(" ! - Yell");
mpr(" a - Order allies to attack a monster");
	if (you[0].prev_targ != MHITNOT && you[0].prev_targ < MNST)
	 if (mons_near(you[0].prev_targ) == 1 && (menv [you[0].prev_targ].m_ench [2] != 6 || player_see_invis() != 0))
	 {
		mpr(" p - Order allies to attack your previous target");
		targ_prev = 1;
	 }
msg(" Anything else - Stay silent ");

int keyn = get_ch();

switch(keyn)
{

  case '!':
  mpr("You yell for attention!");
  you[0].turnover = 1;
  noisy(12, you[0].x_pos, you[0].y_pos);
  return;

  case 'a':
  mpr("Gang up on whom?");
  direction(100, beam);

  if (beam[0].nothing == -1 || mgrd [beam[0].target_x] [beam[0].target_y] == MNG)
  {
	mpr("Yeah, whatever.");
	return;
  }
  mons_targd = mgrd [beam[0].target_x] [beam[0].target_y];
  break;


  case 'p':
  if (targ_prev == 1)
  {
   mons_targd = you[0].prev_targ;
   break;
  } /* fall through... */
  default:
  mpr("Okely-dokely.");
  return;

}

int i = 0;

for (i = 0; i < MNST; i ++)
{
 if (menv [i].m_class == -1) continue;
 if (menv [i].m_beh != BEH_ENSLAVED) continue;
 if (mons_near(i) == 0) continue;
 menv [i].m_hit = mons_targd;
 menv [i].m_targ_1_x = menv [mons_targd].m_x;
 menv [i].m_targ_1_y = menv [mons_targd].m_y;
}

noisy(10, you[0].x_pos, you[0].y_pos);
mpr("Attack!");

}



