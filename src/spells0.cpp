#include "spells0.h"
#include <string.h>
#include "linuxlib.h"


#include <stdlib.h>

#include "externs.h"
#include "enum.h"

#include "itemname.h"
#include "macro.h"
#include "player.h"
#include "dungeon.h"

int spell_hunger(int spell_value, int spell);
int which_spell_in_book(int sbook_type, int spl);
char spellbook_contents(int plus, int type);
char spell_value(int spell);
int spell_type(int spell, int typy);
int spell_spec(int spell, int power);
char spell_fail(int spell);
void print_slash(char already);
char spell_list(void);


char spell_list(void)
{
char st_pass [60];

   int j;
   char lines = 0;
   int anything = 0;
   char strng [5] = "";
   char ft;
   char ki;
   char already = 0;

   strcpy(st_pass, "");

   clrscr();

cprintf("  Your Programs                     Type                  Success   Level");
lines++;

	if (lines > 22)
	{
		gotoxy(1,25);
		cprintf("-more-");
		ki = getch();
		if (ki == 27)
		{
			return 27;
		}
		if (ki >= 65 && ki < 123)
		{
			return ki;
		}
		if (ki == 0) ki = getch();
		lines = 0;
		clrscr();
		gotoxy(1,1);
		anything = 0;

	}
	if (lines > 1) cprintf(EOL" ");

	for (j = 0; j < 25; j++)
	{

		if (lines > 23)
		{
			gotoxy(1,25);
			cprintf("-more-");
			ki = getch();
			if (ki == 27)
			{
				return 27;
			}
			if (ki >= 65 && ki < 123)
			{
				return ki;
			}
			if (ki == 0) ki = getch();
			lines = 0;
			clrscr();
			gotoxy(1,1);
			anything = 0;
		}

		if (you[0].spells [j] != 210)
		{

			anything ++;
			if (lines > 0) cprintf(EOL);
			lines++;
				cprintf(" ");
				if (j < 26) ft = (char) j + 97;
					else ft = (char) j  + 65;
				strng [0] = ft;
				cprintf(strng);
			cprintf(" - ");

			cprintf(spell_name(you[0].spells [j]).c_str());

   gotoxy(35, wherey());
   already = 0;
   if (spell_type(you[0].spells [j], 17) == 1)
   {
    cprintf("Holy");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 21) == 1)
   {
    print_slash(already);
    cprintf("Poison");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 13) == 1)
   {
    print_slash(already);
    cprintf("Fire");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 14) == 1)
   {
    print_slash(already);
    cprintf("Ice");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 22) == 1)
   {
    print_slash(already);
    cprintf("Earth");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 23) == 1)
   {
    print_slash(already);
    cprintf("Air");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 11) == 1)
   {
    print_slash(already);
    cprintf("Conjuration");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 12) == 1)
   {
    print_slash(already);
    cprintf("Plugins");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 19) == 1)
   {
    print_slash(already);
    cprintf("Statistics");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 20) == 1)
   {
    print_slash(already);
    cprintf("Maintenance use");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 18) == 1)
   {
    print_slash(already);
    cprintf("Summoning");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 15) == 1)
   {
    print_slash(already);
    cprintf("Transmigration");
    already = 1;
   }
   if (spell_type(you[0].spells [j], 16) == 1)
   {
    print_slash(already);
    cprintf("Cyborgization");
    already = 1;
   }



/*
types of spells:
11 = conjuration
12 = enchantment
13 = fire
14 = ice
15 = transmigration
16 = necromancy
17 = holy
18 = summoning
19 = divination
20 = translocation
21 = poison
*/

gotoxy(58, wherey());

int spell_f = spell_fail(you[0].spells [j]);

if (spell_f == 100)
{
 cprintf("Useless");
} else
if (spell_f > 90)
{
 cprintf("Terrible");
} else
if (spell_f > 80)
{
 cprintf("Cruddy");
} else
if (spell_f > 70)
{
 cprintf("Bad");
} else
if (spell_f > 60)
{
 cprintf("Very Poor");
} else
if (spell_f > 50)
{
 cprintf("Poor");
} else
if (spell_f > 40)
{
 cprintf("Fair");
} else
if (spell_f > 30)
{
 cprintf("Good");
} else
if (spell_f > 20)
{
 cprintf("Very Good");
} else
if (spell_f > 10)
{
 cprintf("Great");
} else
if (spell_f > 0)
{
 cprintf("Excellent");
} else
 cprintf("Perfect");

			gotoxy(70, wherey());


         cprintf(to_string(spell_value(you[0].spells [j])).c_str());

		}
	} // end of j loop
   if (anything > 0)
   {
	   ki = getch();
	   if (ki >= 65 && ki < 123)
	   {
		   return ki;
	   }
	   if (ki == 0) ki = getch();
	   return anything;
   }

       // was 35
   ki = getch();

   return ki;


}


void print_slash(char already)
{

 if (already == 1) cprintf("/");

}


char spell_fail(int spell)
{

int chance = 60;
int chance2 = 0;


chance -= spell_spec(spell, 0) * 6;

chance -= you[0].intel * 2;

//chance -= (you[0].intel - 10) * abs(you[0].intel - 10);

//chance += spell_value(spell) * spell_value(spell) * 3; //spell_value(spell);

if (you[0].equip [EQ_BODY_ARMOUR] != -1)
 chance += (abs(property(2, you[0].inv_type [you[0].equip [EQ_BODY_ARMOUR]], 1)) * 3) - 2;

if (you[0].equip [EQ_SHIELD] != -1)
{
  if (you[0].inv_type [you[0].equip [EQ_SHIELD]] == 8) chance += 5;
  if (you[0].inv_type [you[0].equip [EQ_SHIELD]] == 14) chance += 15;
}

switch(spell_value(spell))
{
 case 1: chance += 3; break;
 case 2: chance += 15; break;
 case 3: chance += 35; break;
 case 4: chance += 70; break;
 case 5: chance += 100; break;
 case 6: chance += 150; break;
 case 7: chance += 200; break;
 case 8: chance += 260; break;
 case 9: chance += 330; break;
 case 10: chance += 420; break;
 case 11: chance += 500; break;
 case 12: chance += 600; break;
}

//if (chance <= 0) chance = 0;
if (chance > 100) chance = 100;

chance2 = chance;

if (chance < 45) chance2 = 45;
if (chance < 42) chance2 = 43;
if (chance < 38) chance2 = 41;
if (chance < 35) chance2 = 40;
if (chance < 32) chance2 = 38;
if (chance < 28) chance2 = 36;
if (chance < 22) chance2 = 34;
if (chance < 16) chance2 = 32;
if (chance < 10) chance2 = 30;
if (chance < 2) chance2 = 28;
if (chance < -7) chance2 = 26;
if (chance < -12) chance2 = 24;
if (chance < -18) chance2 = 22;
if (chance < -24) chance2 = 20;
if (chance < -30) chance2 = 18;
if (chance < -38) chance2 = 16;
if (chance < -46) chance2 = 14;
if (chance < -60) chance2 = 12;
if (chance < -80) chance2 = 10;
if (chance < -100) chance2 = 8;
if (chance < -120) chance2 = 6;
if (chance < -140) chance2 = 4;
if (chance < -160) chance2 = 2;
if (chance < -180) chance2 = 0;

if (you[0].religion == GOD_VEHUMET && you[0].duration [DUR_PRAYER] != 0 && you[0].piety >= 50 && (spell_type(spell, 11) != 0 || spell_type(spell, 18) != 0))
{
 chance2 /= 2;
}

if (you[0].duration [DUR_TRANSFORMATION] > 0 && you[0].attribute [ATTR_TRANSFORMATION] == TRAN_BLADE_HANDS)
{
 chance2 += 20;
}

return chance2;

}


int spell_spec(int spell, int power)
{

int enhanced = 0;


power = 0;

int s = 0;
int spellsy = 0;

for (s = 11; s < 24; s ++)
{
// if (s == 13 || s == 14 || s == 17) continue;
 if (s == 17) continue;
 if (spell_type(spell, s) == 1) spellsy ++;
}

if (spellsy != 0)
{
if (spell_type(spell, 11) == 1)
{
 power += (you[0].skills [SK_CONJURATIONS] * 2) / spellsy;
}
if (spell_type(spell, 12) == 1)
{
 power += (you[0].skills [SK_ENCHANTMENTS] * 2) / spellsy;
}
if (spell_type(spell, 15) == 1)
{
 power += (you[0].skills [SK_TRANSMIGRATION] * 2) / spellsy;
}
if (spell_type(spell, 16) == 1)
{
 power += (you[0].skills [SK_NECROMANCY] * 2) / spellsy;
}
if (spell_type(spell, 18) == 1)
{
 power += (you[0].skills [SK_SUMMONINGS] * 2) / spellsy;
}
if (spell_type(spell, 19) == 1)
{
 power += (you[0].skills [SK_DIVINATIONS] * 2) / spellsy;
}
if (spell_type(spell, 20) == 1)
{
 power += (you[0].skills [SK_TRANSLOCATIONS] * 2) / spellsy;
}
if (spell_type(spell, 21) == 1)
{
 power += (you[0].skills [SK_POISON_MAGIC] * 2) / spellsy;
}
if (spell_type(spell, 22) == 1)
{
 power += (you[0].skills [SK_EARTH_MAGIC] * 2) / spellsy;
}
if (spell_type(spell, 23) == 1)
{
 power += (you[0].skills [SK_AIR_MAGIC] * 2) / spellsy;
}
if (spell_type(spell, 13) == 1)
{
 power += (you[0].skills [SK_FIRE_MAGIC] * 2) / spellsy;
}
if (spell_type(spell, 14) == 1)
{
 power += (you[0].skills [SK_ICE_MAGIC] * 2) / spellsy;
}
} // end if spellsy != 0

power += (you[0].skills [SK_SPELLCASTING] * 5) / 10;

power += player_mag_abil(); // currently only affected by staves/rings of wizardry


/*
types of spells:
11 = conjuration
12 = enchantment
13 = fire
14 = ice
15 = transmigration
16 = necromancy
17 = holy
18 = summoning
19 = divination
20 = translocation
21 = poison
*/

if (spell_type(spell, 16) == 1)
{
  enhanced += player_spec_death();
  enhanced -= player_spec_holy();
//  naughty(spell_value(spell) * 2, 1);
} // necromancy
if (spell_type(spell, 13) == 1)
{
  enhanced += player_spec_fire();
  enhanced -= player_spec_cold();
} // fire
if (spell_type(spell, 14) == 1)
{
  enhanced += player_spec_cold();
  enhanced -= player_spec_fire();
} // ice
if (spell_type(spell, 17) == 1)
{
  enhanced -= player_spec_death();
  enhanced += player_spec_holy();
  if (you[0].special_wield == 50) enhanced -= 2;
} // holy - signs switched!
if (spell_type(spell, 11) == 1)
{
 enhanced += player_spec_conj();
}
if (spell_type(spell, 12) == 1)
{
 enhanced += player_spec_ench();
}
if (spell_type(spell, 18) == 1)
{
 enhanced += player_spec_summ();
}
if (spell_type(spell, 21) == 1)
{
 enhanced += player_spec_poison();
}

if (spell_type(spell, 22) == 1)
{
  enhanced += player_spec_earth(); // spec_earth
  enhanced -= player_spec_air(); // spec_air
} // earth

if (spell_type(spell, 23) == 1)
{
  enhanced -= player_spec_earth(); // spec_earth
  enhanced += player_spec_air(); // spec_air
} // air

/* If the bit just above changes, remember to also change it in crawl99.cc */

/*
types of you[0].spells:
11 = conjuration
12 = enchantment
13 = fire
14 = ice
15 = transmigration
16 = necromancy
17 = holy
18 = summoning
19 = divination
20 = translocation
21 = poison
*/

/*
if (enhanced == 1)
{
 strcpy(info, "You feel a surge of power.");
 incrl();
}
if (enhanced == 2)
{
 strcpy(info, "You feel a strong surge of power.");
 incrl();
}
if (enhanced >= 3) // how would you get this???
{
 strcpy(info, "You feel a huge surge of power!");
 incrl();
}
if (enhanced == -1)
{
 strcpy(info, "You feel a numb sensation.");
 incrl();
}
if (enhanced == -2)
{
 strcpy(info, "You feel an extremely numb sensation.");
 incrl();
}
if (enhanced <= -3)
{
 strcpy(info, "You feel an extraordinarily numb sensation.");
 incrl();
}
*/

if (enhanced > 0)
{
 for (s = 0; s < enhanced; s++)
 {
  power *= 15;
  power /= 10;
 }
}

if (enhanced < 0)
{
 for (s = enhanced; s < 0; s++)
 {
  power /= 2;
//  power /= 10;
 }
}

return power;

} // end int spell_spec






int spell_type(int spell, int typy)
{
int stype = 0;

switch(spell)
{
	case 0: stype = 19; break; //6; // identify
	case 1: stype = 20; break; //5; // teleportation
	case 2: stype = 12; break; //4; // cause fear
	case 3: stype = 12; break; //1; // noise
	case 4: stype = 12; break; //5; // remove curse
	case 5: stype = 11; break; //1; // magic missile
	case 6: stype = 1311; break; //5; // fireball
	case 13: stype = 1311; break; //3; // conjure flame
   case 14: stype = 1522; break; //4; // dig
   case 15: stype = 1311; break; //5; // firebolt
   case 16: stype = 1411; break; //5; // bolt of freezing cold
   case 17: stype = 2311; break; //6; // bolt of lightning
   case 20: stype = 1512; break; //6; // polymorph other
   case 21: stype = 12; break; //3;
   case 22: stype = 12; break; //4;
   case 23: stype = 12; break; //4;
   case 24: stype = 12; break; //3;
   case 25: stype = 12; break; //4; // invisibility
   case 26: stype = 1311; break; //2; // throw flame
   case 27: stype = 1411; break; //2; // throw frost
   case 28: stype = 20; break; //4; // blink
   case 29: stype = 1411; break; //7; // freezing cloud
   case 30: stype = 232111; break; //3; // stinking cloud
   case 31: stype = 1312; break; //8; // ring of flames
   case 32: stype = 17; break; //2; //  restore strength:
   case 33: stype = 17; break; //2; //          int
   case 34: stype = 17; break; //2; //          dex
   case 35: stype = 2111; break; //5; // venom bolt
   case 36: stype = 21; break; //4; // toxic radiance - uses lots of food?
   case 37: stype = 20; break; //5; // teleport other
   case 38: stype = 17; break; //2; // lesser healing
   case 39: stype = 17; break; //6; // greater healing
   case 40: stype = 17; break; //3; // cure poison
   case 41: stype = 17; break; //5; // purification
   case 42: stype = 1612; break; //8; // death's door
   case 43: stype = 12; break; //3; // selective amnesia
   case 44: stype = 12; break; //7; // mass confusion
   case 45: stype = 17; break; //4; // smiting
   case 46: stype = 17; break; //3; // repel undead
   case 47: stype = 17; break; //7; // holy word
   case 48: stype = 19; break; //3; // detect curse
   case 49: stype = 18; break; //1; // summon small mammal
   case 50: stype = 18; break; //4; // Abjuration
   case 51: stype = 2118; break; //4; // summon scorpions
   case 52: stype = 1223; break; //3; // levitation
   case 53: stype = 1611; break; //6; // bolt of draining
   case 54: stype = 1122; break; //8; // splinters
   case 55: stype = 11; break; //4; // innacuracy
   case 56: stype = 2111; break; //6; // poisonous cloud
   case 57: stype = 1311; break; //8; // fire storm
   case 58: stype = 19; break; //2; // detect traps
   case 59: stype = 20; break; //2; // random blink
   case 60: stype = 11; break; //5; // Isk's blast
   case 61: stype = 18; break; //5; // swarm
   case 62: stype = 18; break; //8; // Summon Thing
   case 63: stype = 12; break; //4; // Enslavement
   case 64: stype = 1922; break; //4; // Magic Mapping
   case 65: stype = 17; break; //3; // heal other
   case 66: stype = 16; break; //7; // Animate dead
   case 67: stype = 16; break; //1; // Pain
   case 68: stype = 12; break; //4; // Extension
   case 69: stype = 1612; break; //6; // Control Undead
   case 70: stype = 16; break; //3; // animate skeleton
   case 71: stype = 16; break; //3; // vampiric draining
   case 72: stype = 1618; break; // summon greater undead
   case 73: stype = 19; break; // detect items
   case 74: stype = 16; break;

   case 75: stype = 13; break; // burn
   case 76: stype = 14; break; // freeze
   case 77: stype = 18; break; // Summon elemental
   case 78: stype = 14; break; // refrigeration
   case 79: stype = 1311; break; // Sticky flame
   case 80: stype = 1418; break; // ice beast
   case 81: stype = 1412; break; // ice armour
   case 82: stype = 18; break; // imp
   case 83: stype = 1223; break; // repel missiles
   case 84: stype = 12; break; // berserk
   case 85: stype = 16; break; // dispel undead
   case 86: stype = 17; break; // Guardian
   case 87: stype = 17; break; // ???
   case 88: stype = 1723; break; // Thunderbolt
   case 89: stype = 17; break; // Flame of Cleansing
   case 90: stype = 17; break; // Shining Light
   case 91: stype = 17; break; // Summon Deva
   case 92: stype = 17; break; // Abjuration

   case 110: stype = 16; break; // twisted res
   case 111: stype = 1612; break; // regen
   case 112: stype = 16; break; // bone shards
   case 113: stype = 20; break; // Banishment
   case 114: stype = 1516; break; // degeneration
   case 115: stype = 1121; break; // sting
   case 116: stype = 16; break; // blood
   case 117: stype = 12; break; // dance
   case 118: stype = 1113; break; // hellfire
   case 119: stype = 18; break; // summon demon
   case 120: stype = 18; break; // summon demon
   case 121: stype = 18; break; // summon demon
   case 122: stype = 16; break; // rot corpse
   case 123: stype = 12; break; // Tukima's blade
   case 124: stype = 1213; break; // Fire brand
   case 125: stype = 1214; break; // Ice brand
   case 126: stype = 1216; break; // weapon of draining

   case 127: stype = 1122; break; // throw pebble
   case 128: stype = 1122; break; // bolt of iron
   case 129: stype = 1122; break; // stone arrow
   case 130: stype = 1122; break; // tomb of doro
   case 131: stype = 1222; break; // stonemail

   case 132: stype = 1123; break; // shock
   case 133: stype = 1223; break; // swiftness
   case 134: stype = 1223; break; // flight
   case 135: stype = 1223; break; // insulation
   case 136: stype = 1123; break; // orb of elec
   case 137: stype = 19; break; // detect creat
   case 138: stype = 21; break; // cure poison (poison)
   case 139: stype = 1220; break; // control teleport
   case 140: stype = 21; break; // poison ammunition
   case 141: stype = 21; break; // poison weapon
   case 142: stype = 2112; break; // resist poison
   case 143: stype = 12; break; // noise 2
   case 144: stype = 15; break; // alter self
   case 145: stype = 11; break; // debug ray
   case 146: stype = 1820; break; // recall
   case 147: stype = 20; break; // Portal
   case 148: stype = 16; break; // Agony

   case 149: stype = 1521; break; // Spider form
   case 150: stype = 15; break; // Disrupt
   case 151: stype = 15; break; // Disintegrate
   case 152: stype = 15; break; // Blade Hands
   case 153: stype = 2215; break; // Statue
   case 154: stype = 1415; break; // Ice Form
   case 155: stype = 1315; break; // Dragon Form
   case 156: stype = 1615; break; // Lich Form
   case 157: stype = 16; break; // Death channel
   case 158: stype = 16; break; // Symbol of Torment
   case 159: stype = 1223; break; // deflect missiles
   case SPELL_ORB_OF_FRAGMENTATION: stype = 1122; break;
   case SPELL_ICE_BOLT: stype = 1114; break;
   case SPELL_ICE_STORM: stype = 1114; break;
   case SPELL_ARC: stype = 23; break;
   case SPELL_AIRSTRIKE: stype = 23; break;
   case SPELL_SHADOW_CREATURES: stype = 18; break;


/*
types of spells:
11 = conjuration
12 = enchantment
13 = fire
14 = ice
15 = transmigration
16 = necromancy
17 = holy
18 = summoning
19 = divination
20 = translocation
21 = poison
22 = Earth
23 = Air
-see guidelines below
*/



   // Make restore abilities necromancy?

   default: stype = 0; break;
//default: return 2;

}

//if (stype == typy || (stype / 10) % 10 == typy || (stype / 100) % 10 == typy) return 1;

//if (stype == typy || (stype / 10) % 10 == typy || (stype / 100) % 10 == typy || (stype / 1000) % 10 == typy || (stype / 10000) % 10 == typy) return 1;

if (stype % 100 == typy || (stype / 100) % 100 == typy || (stype / 10000) % 100 == typy) return 1;

//232111

//if (stype % 100 == typy || stype / 100 == typy) return 1;

return 0;

} // end spell_type

/*
In case anyone ever wants to add new spells, or just understand my reasons
 for putting a particular spell into a particular type, read on:

Guidelines for typing spells

Conjuration
This type has a near monopoly on effective and relatively risk-free combat
spells. All other types of combat spells are either indirect (enchantments),
risky/detrimental/not versatile (necromancy) or just plain crappy (burn and
freeze), although smiting (holy) is not too bad.
Conjuration spells all involve the magical creation of matter and/or energy
(which are the same thing anyway, right?). They are distinguished from
summoning spells in that they do not involve the summoning of an entire
creature from another place.

Enchantment
These spells mostly cause some kind of durational effect, which lasts only
until the magic wears off. Enchantments are distinguished from transmigrations
in that the latter cause a permanent alteration in something which persists
even after the magic has faded, while the effects of the former last only
so long as the magic does. Sometimes enchantments may take advantage of the
more powerful aspects of transmigration to induce some kind of radical change
(eg polymorph).
Some enchantments would also fall under the description of 'meta-magic'
spells, like Selective Amnesia and Remove Curse (and if I ever implement
Dispel Magic, it will be an enchantment).
It is possible that some divinations could be retyped as
divination/enchantment, as they appear to be primarily concerned with
detecting enchantments. Detect Curse and Identify are what I'm thinking
of here.

Fire and Ice
These are quite obvious. I'm trying to keep these two balanced with each
other, but it can be difficult. I have to weigh up some useful fire spells,
like Sticky Flame, Fireball, Ring of Flames and Firestorm, and the fact that
Fire wizards have an advantage when summoning fire elementals by either
spell or device, with the also quite useful Refrigeration, Ice Armour and
Freezing Cloud. Ice wizards don't have a corresponding advantage with
water elementals, because water and ice are two different things (ice is not
necessarily water ice, for example).
Generally, Fire spells tend towards chaos, disorder and entropy, while
Ice spells tend towards order and stasis. But these trends are rather
underdeveloped at the moment.
Note that just about the only reason one would ever choose an ice or fire
wizard over a conjurer would be the resistance gained at level 12.
Especially because having a fire specialisation basically removes any chance
of ever using ice spells effectively, and vice versa.

Transmigration
See enchantments.

Necromancy
This is the fun stuff. Necromancy is a mixed bag of many and various
different kinds of spells, with a few common themes:
-Differentiation of living, dead and undead. Some necromancy affects only the
living (pain, vampiric draining etc), some affects only the dead (animate
dead, twisted resurrection etc), and some affects only undead (dispel and
control undead).
-Actual or potential harm: eg risk in Death's Door, hp loss with Pain,
disease with summon greater undead, etc. Also loss of potential xp gain
with bolt of draining and degeneration.
-Material components are central to many of the spells.
-Some spells duplicate effects of other types, but do so in a different
(neither superior or inferior) way. Eg bone shards is a very powerful spell
for only 3 magic points, but requires preparation. Also, necromantic
healing spells are different and more idiosyncratic than holy healing.
Although regeneration is usually less useful than lesser healing and is
level 3 instead of 2, it can be cast before combat (when 1 turn spent
casting is less important), and is affected by extension.
-Generally unholy theme of spells (I mean, Twisted Resurrection?).

Holy
The Holy type is also fairly various, but is rather less interesting than
necromancy (after all, priests are better at fighting than necromancers).
Holy spells do things like driving off undead and healing. Note that I
consider item stickycursing to be more of an issue for enchantments rather
than holy magic, which is why remove curse is enchantment.

Summoning
These spells involve bringing a creature from somewhere else (possibly on
another plane of existence) to this world to do battle for the caster. Some
future summonings could potentially be combination conjuration/summoning
spells, eg the ball lightning spell I keep planning to implement.
Also, potential exists for some risky high-level spells, maybe demon
summoning?

Divination
These spells provide information to the caster. A diviner class would be
possible (and having detect curse and identify would be very handy), but
would be extremely difficult to play - there is no potential in this type
for combat spells.

Translocation
Translocation spells deal with teleportation etc, also interplanar travel
(eg Banishment, and the planned Gate spell).
It is possible that I may give summoners some special access to translocations
due to the obvious similarities.

Poison
These spells all involve poison. Most are also conjurations.
I don't plan to implement a 'Poisoner' class, as it would become unplayable
deep in the dungeon where most monsters are poison resistant.



Many spells use magic from two types. These spells are equally
available to either type; a conjurer is no worse at a fire/conjuration than
at a pure conjuration. I guess a spell could be of three types, but they
would have to be types with short names (limited space in the spell windows).
 - Note : this is no longer true, with the implementation of magic skills.
    Your skill for a spell is effectively the average of all types used in it.
    Poison has no skills, but still has a staff


*/




/*

char spell_value(char spell)
{

switch(spell)
{
	case 0: return 10; // identify
	case 1: return 8; // teleportation
	case 2: return 6; // cause fear
	case 3: return 1; // noise
	case 4: return 7; // remove curse
	case 5: return 1; // magic missile
	case 6: return 7; // fireball
	case 13: return 5; // conjure flame
   case 14: return 6; // dig
   case 15: return 6; // firebolt
   case 16: return 6; // bolt of freezing cold
   case 17: return 8; // bolt of lightning
   case 20: return 7; // polymorph other
   case 21: return 3;
   case 22: return 4;
   case 23: return 4;
   case 24: return 3;
   case 25: return 6; // invisibility
   case 26: return 2; // throw flame
   case 27: return 2; // throw frost
   case 28: return 4; // controlled blink
   case 29: return 11; // freezing cloud
   case 30: return 3; // stinking cloud
   case 31: return 12; // ring of flames
   case 32: return 2; //  restore strength:
   case 33: return 2; //          int
   case 34: return 2; //          dex
   case 35: return 6; // venom bolt
   case 36: return 4; // toxic radiance - uses lots of food?
   case 37: return 9; // teleport other
   case 38: return 3; // lesser healing
   case 39: return 8; // greater healing
   case 40: return 3; // cure poison
   case 41: return 6; // purification
   case 42: return 13; // death's door
   case 43: return 4; // selective amnesia
   case 44: return 10; // mass confusion
   case 45: return 6; // smiting
   case 46: return 3; // repel undead
   case 47: return 10; // holy word
   case 48: return 3; // detect curse
   case 49: return 1; // summon small mammal
   case 50: return 6; // Abjuration
   case 51: return 6; // summon scorpions
   case 52: return 3; // levitation
   case 53: return 7; // bolt of draining
   case 54: return 12; // splinters
   case 55: return 8; // innacuracy
   case 56: return 6; // poisonous cloud
   case 57: return 15; // fire storm
   case 58: return 2; // detect traps
   case 59: return 2; // random blink
   case 60: return 6; // Isk's blast
   case 61: return 6; // swarm
   case 62: return 12; // Summon Thing
   case 63: return 6; // Enslavement
   case 64: return 5; // Magic Mapping
   case 65: return 3; // heal other
   case 66: return 11; // Animate dead
   case 67: return 1; // Pain
   case 68: return 5; // Extension
   case 69: return 10; // Control Undead
   case 70: return 3; // animate skeleton
   case 71: return 5; // vampiric draining
   case 72: return 16; // summon greater undead
   case 73: return 3; // detect items
   case 74: return 9; // reviv
default: return 2;

}

return 1;

}
*/

char spell_value(int spell)
{

switch(spell)
{
	case 0: return 6; // identify
	case 1: return 5; // teleportation
	case 2: return 5; // cause fear
	case 3: return 1; // noise
	case 4: return 5; // remove curse
	case 5: return 1; // magic missile
	case 6: return 6; // fireball
	case 13: return 3; // conjure flame
   case 14: return 4; // dig
   case 15: return 5; // firebolt
   case 16: return 5; // bolt of freezing cold
   case 17: return 6; // bolt of lightning
   case 20: return 5; // polymorph other
   case 21: return 3; // slow
   case 22: return 8; // haste
   case 23: return 4; // paralyse
   case 24: return 3; // confuse
   case 25: return 6; // invisibility
   case 26: return 2; // throw flame
   case 27: return 2; // throw frost
   case 28: return 4; // blink
   case 29: return 7; // freezing cloud
   case 30: return 3; // stinking cloud
   case 31: return 8; // ring of flames
   case 32: return 2; //  restore strength:
   case 33: return 2; //          int
   case 34: return 2; //          dex
   case 35: return 5; // venom bolt
   case 36: return 4; // toxic radiance
   case 37: return 5; // teleport other
   case 38: return 2; // lesser healing
   case 39: return 6; // greater healing
   case 40: return 3; // cure poison
    case 41: return 5; // purification
   case 42: return 8; // death's door
   case 43: return 3; // selective amnesia
   case 44: return 6; // mass confusion
  case 45: return 4; // smiting??
    case 46: return 3; // repel undead
    case 47: return 7; // holy word
   case 48: return 3; // detect curse
   case 49: return 1; // summon small mammal
   case 50: return 3; // Abjuration
   case 51: return 4; // summon scorpions
   case 52: return 2; // levitation
   case 53: return 6; // bolt of draining
   case 54: return 8; // crystal spear
   case 55: return 4; // innacuracy
   case 56: return 6; // poisonous cloud
   case 57: return 9; // fire storm
   case 58: return 2; // detect traps
   case 59: return 2; // random blink
   case 60: return 4; // Isk's blast
   case 61: return 6; // swarm
   case 62: return 8; // Summon Thing
   case 63: return 4; // Enslavement
   case 64: return 4; // Magic Mapping
    case 65: return 3; // heal other
   case 66: return 4; // Animate dead
   case 67: return 1; // Pain
   case 68: return 5; // Extension
   case 69: return 6; // Control Undead
   case 70: return 1; // animate skeleton
   case 71: return 3; // vampiric draining
   case 72: return 7; // summon wraiths
   case 73: return 2; // detect items
   case 74: return 6; // reviv
   case 75: return 1; // burn
   case 76: return 1; // freeze
   case 77: return 4; // Summon elemental
   case 78: return 5; // refrigeration
   case 79: return 4; // Sticky flame
   case 80: return 5; // Ice beast
   case 81: return 3; // Ozocubu's Armour
   case 82: return 2; // imp
   case 83: return 2; // repel missiles
   case 84: return 3; // berserker
   case 85: return 4; // dispel undead
    case 86: return 7; // Guardian
    case 87: return 4; // ???
    case 88: return 6; // Thunderbolt
    case 89: return 8; // Flame of Cleansing
    case 90: return 7; // Shining Light
    case 91: return 8; // Summon Deva
    case 92: return 4; // Abjuration

   case 110: return 5; // twisted res
   case 111: return 3; // regen
   case 112: return 3; // bone shards
   case 113: return 5; // Banishment
   case 114: return 5; // Degeneration
   case 115: return 1; // sting
   case 116: return 2; // blood
   case 117: return 3; // dance
   case 118: return 9; // hellfire
   case 119: return 5; // summon demon
   case 120: return 6; // demonic horde
   case 121: return 7; // summon greater demon

   case 122: return 2; // corpse rot
   case 123: return 2; // Tukima's v bl
   case 124: return 2; // flaming weapon
   case 125: return 2; // freezing weapon
   case 126: return 2; // draining weapon

   case 127: return 1; // throw pebble
   case 128: return 6; // bolt of iron
   case 129: return 3; // stone arrow
   case 130: return 7; // tomb of doro
   case 131: return 6; // stonemail
   case 132: return 3; // shock
   case 133: return 2; // swiftness
   case 134: return 4; // fly
   case 135: return 4; // insulation
   case 136: return 7; // orb of elec
   case 137: return 2; // detect creat
   case 138: return 2; // cure poison (poison)
   case 139: return 4; // control teleport
   case 140: return 4; // poison ammunition
   case 141: return 2; // poison weapon
   case 142: return 4; // resist poison
   case 143: return 2; // create noise 2
   case 144: return 7; // alter self
   case 145: return 7; // debug ray
   case 146: return 3; // recall
   case 147: return 8; // portal
   case 148: return 5; // agony
   case 149: return 3; // Spider form
   case 150: return 1; // Disrupt
   case 151: return 6; // Disintegrate
   case 152: return 4; // Blade Hands
   case 153: return 5; // Statue form
   case 154: return 5; // Ice beast form
   case 155: return 8; // Dragon Form
   case 156: return 8; // Lich Form
   case 157: return 9; // Death Channel
   case 158: return 6; // Symbol of Torment
   case 159: return 6; // deflect missiles
   case SPELL_ORB_OF_FRAGMENTATION: return 7;
   case SPELL_ICE_BOLT: return 4;
   case SPELL_ICE_STORM: return 9;
   case SPELL_ARC: return 1;
   case SPELL_AIRSTRIKE: return 4;
   case SPELL_SHADOW_CREATURES: return 5;

default: return 2;

}

return 1;

}


std::string spell_name(int spell)
{
	switch(spell) {
		case 0: return "Identify";
		case 1: return "Use Ventilation";
		case 2: return "Cause Fear";
		case 3: return "Create Noise";
		case 4: return "Remove Virus";
		case 5: return "Energy Dart";
		case 6: return "Fireball";
		case 13: return "Conjure Flame";
		case 14: return "Dig";
		case 15: return "Bolt of Fire";
		case 16: return "Bolt of Cold";
		case 17: return "Lightning Bolt";
		case 20: return "Polymorph Other";
		case 21: return "Slow";
		case 22: return "Haste";
		case 23: return "Paralyze";
		case 24: return "Confuse";
		case 25: return "Invisibility";
		case 26: return "Throw Flame";
		case 27: return "Throw Frost";
		case 28: return "Controlled Jump";
		case 29: return "Freezing Cloud";
		case 30: return "Mephitic Cloud";
		case 31: return "Ring of Flames";
		case 32: return "Restore Strength";
		case 33: return "Restore Intelligence";
		case 34: return "Restore Dexterity";
		case 35: return "Venom Bolt";
		case 36: return "Olgreb's Toxic Radiance";
		case 37: return "Use Maintenace On Other";
		case 38: return "Lesser Healing";
		case 39: return "Greater Healing";
		case 40: return "Cure Poison";
		case 41: return "Purification";
		case 42: return "Death's Door";
		case 43: return "Selective Amnesia";
		case 44: return "Mass Confusion";
		case 45: return "Smiting";
		case 46: return "Repel Cyborgs";
		case 47: return "Holy Word";
		case 48: return "Detect Virus";
		case 49: return "Summon Small Mammal";
		case 50: return "Abjuration";
		case 51: return "Summon Scorpions";
		case 52: return "Levitation";
		case 53: return "Bolt of Draining";
		case 54: return "Lehudib's Crystal Spear";
		case 55: return "Bolt of Inaccuracy"; // spelling?
		case 56: return "Poisonous Cloud";
		case 57: return "Fire Storm";
		case 58: return "Detect Traps";
		case 59: return "Blink";
		case 60: return "Iskenderun's Mystic Blast"; // this name was found in the hack.exe file of an early version of PCHACK. Credit goes to its creator (whoever that may be).
		case 61: return "Swarm";
		case 62: return "Summon Horrible Things";
		case 63: return "Enslavement";
		case 64: return "Magic Mapping";
		case 65: return "Heal Other";
		case 66: return "Cybernitize Dead";
		case 67: return "Pain";
		case 68: return "Extension";
		case 69: return "Control Cyborg";
		case 70: return "Cybernitize Skeleton";
		case 71: return "Chemical Draining";
		case 72: return "Summon Robots";
		case 73: return "Detect Items";
		case 74: return "Borgnjor's Revivification";
		case 75: return "Burn";
		case 76: return "Freeze";
		case 77: return "Summon Nanorg";
		case 78: return "Ozocubu's Refrigeration";
		case 79: return "Sticky Flame";
		case 80: return "Summon Refrigerator";
		case 81: return "Ozocubu's Armour";
		case 82: return "Call Imp";
		case 83: return "Repel Missiles";
		case 84: return "Berserker Rage";
		case 85: return "Dispel Cyborg";
		case 86: return "Guardian";
		case 87: return "Pestilence";
		case 88: return "Thunderbolt";
		case 89: return "Flame of Cleansing";
		case 90: return "Shining Light";
		case 91: return "Summon Daeva";
		case 92: return "Abjuration";
		case 110: return "Twisted Resurrection";
		case 111: return "Regeneration";
		case 112: return "Bone Shards";
		case 113: return "Banishment";
		case 114: return "Cigotuvi's Degeneration";
		case 115: return "Sting";
		case 116: return "Sublimation of Blood";
		case 117: return "Tukima's Dance";
		case 118: return "Hellfire"; // Staff of Dispater
		case 119: return "Summon Biomutant";
		case 120: return "Mutant Horde";
		case 121: return "Summon Greater Mutant";
		case 122: return "Corpse Rot";
		case 123: return "Tukima's Vorpal Blade";
		case 124: return "Fire Brand";
		case 125: return "Freezing Aura";
		case 126: return "Lethal Infusion";
		case 127: return "Crush";
		case 128: return "Bolt of Iron";
		case 129: return "Stone Arrow";
		case 130: return "Tomb of Doroklohe";
		case 131: return "Stonemail";
		case 132: return "Shock";
		case 133: return "Swiftness";
		case 134: return "Fly";
		case 135: return "Insulation";
		case 136: return "Orb of Electrocution";
		case 137: return "Detect creatures";
		case 138: return "Cure Poison";
		case 139: return "Control Maintenance";
		case 140: return "Poison Ammunition";
		case 141: return "Poison Weapon";
		case 142: return "Resist Poison";
		case 143: return "Projected Noise";
		case 144: return "Alter Self";
		case 145: return "Debugging ray";
		case 146: return "Recall";
		case 147: return "Portal";
		case 148: return "Agony";
		case 149: return "Spider Form";
		case 150: return "Disrupt";
		case 151: return "Disintegrate";
		case 152: return "Blade Hands";
		case 153: return "Statue Form";
		case 154: return "Ice Form";
		case 155: return "Dragon Form";
		case 156: return "Cybermutation";
		case 157: return "Death Channel";
		case 158: return "Symbol of Torment";
		case 159: return "Deflect Missiles";
		case 160: return "Orb of Fragmentation";
		case 161: return "Ice Bolt";
		case 162: return "Ice Storm";
		case 163: return "Arc";
		case 164: return "Airstrike";
		case 165: return "Shadow Creatures";
		default: return "another program";
	}
	/* When adding enchantments, must add them to extension as well */

	/*
	   spells to do:
	   Contingency?
	   Trigger contingency
	   Preserve Corpses
	   Permanency
	   Ball Lightning
	   Explosive rune?
	   Fennel wands
	   More summonings!
	   */
}

#include <bitset>

char spellbook_contents(int plus, int type)
{

char st_pass [60];

int numby = plus;
int j;
int spelcount = 0;
char strng [2] = "";
char ft = 0;

//char st_pass [60];
char already = 0;


   strcpy(st_pass, "");

  strcpy(st_pass, "");

  clrscr();


   int spell_types [10];

   spellbook_template(type, spell_types);

//   numby -= 64;

   std::string stringy = std::bitset<16>(numby).to_string();

   textcolor(LIGHTGREY);
cprintf(" Programs                           Type                      Level"EOL);


   for (j = 1; j < 7; j ++)
   {

      if (stringy [j] != 49) continue;

				cprintf(" ");
				if (spelcount < 26) ft = (char) spelcount + 97;
					else ft = (char) spelcount + 65;

               strcpy(strng, "");

				strng [0] = ft;
            strng [1] = 0;

				cprintf(strng);

			cprintf(" - ");

//			spell_name(???, st_pass);

//			cprintf(st_pass);


			cprintf(spell_name(spell_types [j]).c_str());

         //cprintf("A spell");

   gotoxy(35, wherey());
   already = 0;
   if (spell_type(spell_types [j], 17) == 1)
   {
    cprintf("Holy");
    already = 1;
   }
   if (spell_type(spell_types [j], 21) == 1)
   {
    print_slash(already);
    cprintf("Poison");
    already = 1;
   }
   if (spell_type(spell_types [j], 13) == 1)
   {
    print_slash(already);
    cprintf("Fire");
    already = 1;
   }
   if (spell_type(spell_types [j], 14) == 1)
   {
    print_slash(already);
    cprintf("Ice");
    already = 1;
   }
   if (spell_type(spell_types [j], 22) == 1)
   {
    print_slash(already);
    cprintf("Earth");
    already = 1;
   }
   if (spell_type(spell_types [j], 23) == 1)
   {
    print_slash(already);
    cprintf("Air");
    already = 1;
   }
   if (spell_type(spell_types [j], 11) == 1)
   {
    print_slash(already);
    cprintf("Conjuration");
    already = 1;
   }
   if (spell_type(spell_types [j], 12) == 1)
   {
    print_slash(already);
    cprintf("Plugins");
    already = 1;
   }
   if (spell_type(spell_types [j], 19) == 1)
   {
    print_slash(already);
    cprintf("Statistics");
    already = 1;
   }
   if (spell_type(spell_types [j], 20) == 1)
   {
    print_slash(already);
    cprintf("Maintenance Use");
    already = 1;
   }
   if (spell_type(spell_types [j], 18) == 1)
   {
    print_slash(already);
    cprintf("Summoning");
    already = 1;
   }
   if (spell_type(spell_types [j], 15) == 1)
   {
    print_slash(already);
    cprintf("Transmigration");
    already = 1;
   }
   if (spell_type(spell_types [j], 16) == 1)
   {
    print_slash(already);
    cprintf("Cyborgization");
    already = 1;
   }



			gotoxy(65, wherey());

         cprintf(to_string(spell_value(spell_types [j])).c_str());


         cprintf(EOL);

   spelcount ++;


   } // end of for j




   /*
   for (i = 0; i < 9; i ++)
   {
      stringy [i] += 48;

   }*/


   //cprintf(stringy);

   //getch();


//   goto inpy;


//if (getch() == 0) getch();

char keyn = getch();
if (keyn == 0) getch();

return keyn;

}




int which_spell_in_book(int sbook_type, int spl)
{
int func_pass [10];

spellbook_template(sbook_type, func_pass);

return func_pass [spl];



}

int spell_hunger(int spell_value, int spell)
{

switch(spell_value)
{
 case 1: return 50;
 case 2: return 95;
 case 3: return 160;
 case 4: return 250;
 case 5: return 350;
 case 6: return 550;
 case 7: return 700;
 case 8: return 850;
 case 9: return 1000;
 case 10: return 1000;
 case 11: return 1100;
 case 12: return 1250;
 case 13: return 1380;
 case 14: return 1500;
 case 15: return 1600;
 default: return 1600 + spell_value * 20;
}

return spell;

} // end of spell_hunger


char undead_can_memorise(int spell)
{

switch(spell)
{
   case 42: return 2; // death's door
   case 62: return 1; // Summon Horrible things
   case 74: return 2; // reviv
   case 84: return 2; // berserker
//   case 111: return 2; // regen
   case 138: return 2; // cure poison (poison)
   case 142: return 2; // resist poison
   case 144: return 1; // alter self
   case 149: return 1; // Spider form
   case 152: return 1; // Blade Hands
   case 153: return 1; // Statue form
   case 154: return 1; // Ice beast form
   case 155: return 1; // Dragon Form
   case 156: return 2; // Lich Form
   case 158: return 2; // Symbol of Torment

default: return 0;

}

}

