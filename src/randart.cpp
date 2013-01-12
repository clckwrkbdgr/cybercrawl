
#include "linuxlib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "externs.h"
#include "enum.h"

#include "randart.h"
#include "itemname.h"
#include "stuff.h"

#define RA_PROPERTIES 30

/*
The initial generation of a randart is very simple - it occurs
in dungeon.cc and consists of giving it a few random things - plus & plus2
mainly.
*/

std::string rand_wpn_names[] = {
" of Blood",
" of Death",
" of Bloody Death",
" of Pain",
" of Painful Death",
" of Pain & Death",
" of Infinite Pain",
" of Eternal Torment",
" of Power",
" of Wrath",
/* 10: */
" of Doom",
" of Tender Mercy",
" of the Apocalypse",
" of the Jester",
" of the Ring",
" of the Fool",
" of the Gods",
" of the Imperium",
" of Destruction",
" of Armageddon",
/* 20: */
" of Cruel Justice",
" of Righteous Anger",
" of Might",
" of the Disc",
" of Netchaos Order",
" of Human Liberation Front",
" of The Anonimous",
" of the Ancients",
" of Data",
" of Triads",
/* 30: */
" of the Hackers",
" of the Superhackers",
" of the King",
" of the Queen",
" of the Spheres",
" of Circularity",
" of Linearity",
" of Conflict",
" of Battle",
" of Honour",
/* 40: */
" of the Butterfly",
" of the Wasp",
" of the Frog",
" of the Weasel",
" of the Troglodytes",
" of the Pill-Bug",
" of Sin",
" of Vengeance",
" of Execution",
" of Arbitration",
/* 50: */
" of the Seeker",
" of Truth",
" of Lies",
" of the Eggplant",
" of the Turnip",
" of Chance",
" of Curses",
" of Hell's Wrath",
" of the Cyborg",
" of Chaos",
/* 60: */
" of Law",
" of Life",
" of the Old World",
" of the New World",
" of the Middle World",
" of Crawl",
" of Unpleasantness",
" of Discomfort",
" of Brutal Revenge",
" of Triumph",
/* 70: */
" of Evisceration",
" of Dismemberment",
" of Terror",
" of Fear",
" of Pride",
" of the Volcano",
" of Blood-Lust",
" of Division",
" of Eternal Harmony",
" of Peace",
/* 80: */
" of Quick Death",
" of Instant Death",
" of Misery",
" of the Whale",
" of the Lobster",
" of the Whelk",
" of the Penguin",
" of the Puffin",
" of the Mushroom",
" of the Toadstool",
/* 90: */
" of the Little People",
" of the Puffball",
" of Spores",
" of Optimality",
" of Pareto-Optimality",
" of Greatest Utility",
" of Anarcho-Capitalism",
" of Ancient Evil",
" of the Revolution",
" of the People",
/* 100: */
" of the Ninjas",
" of the Mechanics",
" of the Terrorists",
" of the Humans",
" of Sludge",
" of the Assassins",
" of the Builders",
" of the Mutants",
" of Equitable Redistribution",
" of Wealth",
/* 110: */
" of Poverty",
" of Reapportionment",
" of Fragile Peace",
" of Reinforcement",
" of Beauty",
" of the Slug",
" of the Snail",
" of the Gastropod",
" of Corporal Punishment",
" of Capital Punishment",
/* 120: */
" of the Beast",
" of Light",
" of Darkness",
" of Day",
" of the Day",
" of Night",
" of the Night",
" of Twilight",
" of the Twilight",
" of Dawn",
/* 130: */
" of the Dawn",
" of the Sun",
" of the Moon",
" of Distant Worlds",
" of the Unseen Realm",
" of Pandemonium",
" of the Abyss",
" of the Nexus",
" of the Gulag",
" of the Crusades",
/* 140: */
" of Proximity",
" of Wounding",
" of Peril",
" of the Eternal Warrior",
" of the Eternal War",
" of Evil",
" of Pounding",
" of Oozing Pus",
" of Pestilence",
" of Plague",
/* 150: */
" of Negation",
" of the Saviour",
" of Infection",
" of Defence",
" of Protection",
" of Defence by Offence",
" of Expedience",
" of Reason",
" of Unreason",
" of the Heart",
/* 160: */
" of Offence",
" of the Leaf",
" of Leaves",
" of Winter",
" of Summer",
" of Autumn",
" of Spring",
" of Midsummer",
" of Midwinter",
" of Eternal Night",
/* 170: */
" of Shrieking Terror",
" of the Lurker",
" of the Crawling Thing",
" of the Thing",
"\"Thing\"",
" of the Sea",
" of the Forest",
" of the Trees",
" of Earth",
" of the World",
/* 180: */
" of Bread",
" of Yeast",
" of the Amoeba",
" of Deformation",
" of Guilt",
" of Innocence",
" of Ascent",
" of Descent",
" of Music",
" of Brilliance",
/* 190: */
" of Disgust",
" of Feasting",
" of Sunlight",
" of Starshine",
" of the Stars",
" of Dust",
" of the Clouds",
" of the Sky",
" of Ash",
" of Slime",
/* 200: */
" of Clarity",
" of Eternal Vigilance",
" of Purpose",
" of the Moth",
" of the Goat",
" of Fortitude",
" of Equivalence",
" of Balance",
" of Unbalance",
" of Harmony",
/* 210: */
" of Disharmony",
" of the Inferno",
" of the Omega Point",
" of Inflation",
" of Deflation",
" of Supply",
" of Demand",
" of Gross Domestic Product",
" of Unjust Enrichment",
" of Detinue",
/* 220: */
" of Conversion",
" of Anton Piller",
" of Mandamus",
" of Frustration",
" of Breach",
" of Fundamental Breach",
" of Termination",
" of Extermination",
" of Satisfaction",
" of Res Nullius",
/* 230: */
" of Fee Simple",
" of Terra Nullius",
" of Context",
" of Prescription",
" of Freehold",
" of Tortfeasance",
" of Omission",
" of Negligence",
" of Pains",
" of Attainder",
/* 240: */
" of Action",
" of Inaction",
" of Truncation",
" of Defenestration",
" of Desertification",
" of the Wilderness",
" of Psychosis",
" of Neurosis",
" of Fixation",
" of the Open Hand",
/* 250: */
" of the Tooth",
" of Honesty",
" of Dishonesty",
" of Divine Compulsion",
" of the Invisible Hand",
" of Freedom",
" of Liberty",
" of Servitude",
" of Domination",
" of Tension",
/* 260: */
" of Monotheism",
" of Atheism",
" of Agnosticism",
" of Existentialism",
" of the Good",
" of Relativism",
" of Absolutism",
" of Absolution",
" of Abstinence",
" of Abomination",
/* 270: */
" of Mutilation",
" of Stasis",
" of Wonder",
" of Dullness",
" of Dim Light",
" of the Shining Light",
" of Immorality",
" of Amorality",
" of Precise Incision",
" of Orthodoxy",
/* 280: */
" of Faith",
" of Untruth",
" of the Augerer",
" of the Water Diviner",
" of the Soothsayer",
" of Punishment",
" of Amelioration",
" of Sulphur",
" of the Egg",
" of the Globe",
/* 290: */
" of the Candle",
" of the Candelabrum",
" of the Chemists",
" of the Terrorists",
" of the Fields",
" of World's End",
" of Blue Skies",
" of Red Skies",
" of Orange Skies",
" of Purple Skies",
/* 300: */
" of Articulation",
" of the Mind",
" of the Spider",
" of the Lamprey",
" of the Beginning",
" of the End",
" of Severance",
" of Sequestration",
" of Mourning",
" of Death's Door",
/* 310: */
" of the Key",
" of Earthquakes",
" of Failure",
" of Success",
" of Intimidation",
" of the Mosquito",
" of the Gnat",
" of the Blowfly",
" of the Turtle",
" of the Tortoise",
/* 320: */
" of the Pit",
" of the Grave",
" of Submission",
" of Dominance",
" of the Messenger",
" of Crystal",
" of Gravity",
" of Levity",
" of the Slorg",
" of Surprise",
/* 330: */
" of the Maze",
" of the Labyrinth",
" of Divine Intervention",
" of Rotation",
" of the Spinneret",
" of the Scorpion",
" of Demonkind",
" of the Genius",
" of Bloodstone",
" of Grontol",
/* 340: */
" \"Grim Tooth\"",
" \"Widowmaker\"",
" \"Widowermaker\"",
" \"Lifebane\"",
" \"Conservator\"",
" \"Banisher\"",
" \"Tormentor\"",
" \"Secret Weapon\"",
" \"String\"",
" \"Stringbean\"",
/* 350: */
" \"Blob\"",
" \"Globulus\"",
" \"Hulk\"",
" \"Raisin\"",
" \"Starlight\"",
" \"Giant's Toothpick\"",
" \"Pendulum\"",
" \"Backscratcher\"",
" \"Brush\"",
" \"Murmer\"",
/* 360: */
" \"Sarcophage\"",
" \"Concordance\"",
" \"Dragon's Tongue\"",
" \"Arbiter\"",
" \"Gram\"",
" \"Grom\"",
" \"Grim\"",
" \"Grum\"",
" \"Rummage\"",
" \"Omelette\"",
/* 370: */
" \"Egg\"",
" \"Aubergine\"",
" \"Z\"",
" \"X\"",
" \"O\"",
" \"Ox\"",
" \"Death Rattle\"",
" \"Tattletale\"",
" \"Fish\"",
" \"Bung\"",
/* 380: */
" \"Arcanum\"",
" \"Mud Pie of Death\"",
" \"Transmigrator\"",
" \"Ultimatum\"",
" \"Earthworm\"",
" \"Worm\"",
" \"Worm's Wrath\"",
" \"Xom's Favour\"",
" \"Bingo\"",
" \"Leviticus\"",
// Not yet possible...
/* 390: */
" of Joyful Slaughter",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",



/* 390: */
"\"\"",
"\"\"",
"\"\"",
"\"\"",
"\"\"",
"\"\"",
"\"\"",
"\"\"",
"\"\"",
"\"\"",






/* 340: */
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",












/* 200: */
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
};


std::string rand_armour_names[] = {
/* 0: */
" of Shielding",
" of Grace",
" of Impermeability",
" of the Onion",
" of Life",
" of Defence",
" of Nonsense",
" of Eternal Vigilance",
" of Fun",
" of Joy",
/* 10: */
" of Death's Door",
" of the Gate",
" of Watchfulness",
" of Integrity",
" of Bodily Harmony",
" of Harmony",
" of the Untouchables",
" of Grot",
" of Grottiness",
" of Filth",
/* 20: */
" of Wonder",
" of Wondrous Power",
" of Power",
" of Vlad",
" of the Eternal Fruit",
" of Invincibility",
" of Hide-and-Seek",
" of the Mouse",
" of the Saviour",
" of Plasticity",
/* 30: */
" of Baldness",
" of Terror",
" of the Arcane",
" of Resist Death",
" of Anaesthesia",
" of the Guardian",
" of Inviolability",
" of the Tortoise",
" of the Turtle",
" of the Armadillo",
/* 40: */
" of the Echidna",
" of the Armoured One",
" of Weirdness",
" of Pathos",
" of Serendipity",
" of Loss",
" of Hedging",
" of Indemnity",
" of Limitation",
" of Exclusion",
/* 50: */
" of Repulsion",
" of Untold Secrets",
" of the Earth",
" of the Turtledove",
" of Limited Liability",
" of Responsibility",
" of Hadjma",
" of Glory",
" of Preservation",
" of Conservation",
/* 60: */
" of Protective Custody",
" of the Clam",
" of the Barnacle",
" of the Lobster",
" of Hairiness",
" of Supple Strength",
" of Space",
" of the Vacuum",
" of Compression",
" of Decompression",

/* 70: */
" of the Loofah",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
" of ",
// Sarcophagus
};

/* Remember: disallow unrandart creation in abyss/pan */


/* The following unrandart bits were taken from $pellbinder's mstruct code
(see mstruct.h & mstruct.cc) and modified (LH). They're in randart.cc and
not randart.h because they're only used in this code module. */

//extern int unranddatasize;
//extern struct unrandart_entry {
int unranddatasize;

struct unrandart_entry {

	std::string name/*[32]*/; // true name of unrandart (max 31 chars)
	std::string unid_name/*[32]*/; // un-id'd name of unrandart (max 31 chars)
	int ura_cl ; // class of ura
	int ura_ty ; // type of ura
	int ura_pl ; // plus of ura
	int ura_pl2 ; // plus2 of ura
	int ura_col ; // colour of ura
	int prpty [RA_PROPERTIES] ;
	std::string spec_descrip1/*[32]*/; // A special description which is added to the 'V' command output (max 31 chars)
	std::string spec_descrip2/*[32]*/; // A special description which is added to the 'V' command output (max 31 chars)
	std::string spec_descrip3/*[32]*/; // A special description which is added to the 'V' command output (max 31 chars)
};// unranddata[];



struct unrandart_entry unranddata[]={
#include "unrand.h"
};

#define sura seekunrandart(aclass, atype, aplus, aplus2)

struct unrandart_entry *seekunrandart(int aclass, int atype, int aplus, int aplus2);

int unrandart_exist [NO_UNRANDARTS];
void set_unrandart_exist(int whun, int is_exist);

int random4(int randmax);



void set_unrandart_exist(int whun, int is_exist)
{
 unrandart_exist [whun] = is_exist;
}

int does_unrandart_exist(int whun)
{
 return unrandart_exist [whun];
}


int randart_wpn_properties(int aclass, int atype, int adam, int aplus, int aplus2, int acol, int prop)
{

if ((aclass == OBJ_JEWELLERY && adam == 201) || (aclass != OBJ_JEWELLERY && adam == 25))
{
 struct unrandart_entry *search_unrandarts = sura;
 return search_unrandarts->prpty [int(prop)];
}

int globby = aclass * adam + acol + atype * (aplus % 100) + aplus2 * 100;
int randstore = random();
int proprt [RA_PROPERTIES];
int i = 0;
int power_level = ((aplus % 100) - 50) / 3 + (aplus2 - 50) / 3;

srandom(globby);

if (aclass == OBJ_ARMOUR) power_level = ((aplus % 100) - 50) / 2 + 2;

if (aclass == OBJ_JEWELLERY) power_level = 1 + random4(3) + random4(2);

if (power_level < 0) power_level = 0;

for (i = 0; i < RA_PROPERTIES; i ++)
{
 proprt [i] = 0;
}

if (aclass == OBJ_WEAPONS) /* Only weapons get brands, of course */
{
 proprt [RAP_BRAND] = SPWPN_FLAMING + random4(15); /* brand */
 if (random4(6) == 0) proprt [RAP_BRAND] = SPWPN_FLAMING + random4(2);
 if (random4(6) == 0) proprt [RAP_BRAND] = SPWPN_ORC_SLAYING + random4(4);
 if (random4(6) == 0) proprt [RAP_BRAND] = SPWPN_VORPAL;
 if (proprt [RAP_BRAND] == SPWPN_FLAME || proprt [RAP_BRAND] == SPWPN_FROST) proprt [RAP_BRAND] = 0; /* missile wpns */
 if (proprt [RAP_BRAND] == SPWPN_PROTECTION) proprt [RAP_BRAND] = 0; /* no protection */
 if (proprt [RAP_BRAND] == SPWPN_DISRUPTION && atype != WPN_MACE) proprt [RAP_BRAND] = SPWPN_NORMAL; /* Only maces get disruption */

 if (atype >= WPN_SLING && atype <= WPN_HAND_CROSSBOW)
 {
  if (random4(3) == 0) proprt [RAP_BRAND] = SPWPN_FLAME + random4(2); else proprt [RAP_BRAND] = SPWPN_NORMAL;
 }

 if (atype == WPN_DEMON_BLADE || atype == WPN_DEMON_WHIP)
 {
  switch(random4(9))
  {
   case 0: proprt [RAP_BRAND] = SPWPN_DRAINING; break;
   case 1: proprt [RAP_BRAND] = SPWPN_FLAMING; break;
   case 2: proprt [RAP_BRAND] = SPWPN_FREEZING; break;
   case 3: proprt [RAP_BRAND] = SPWPN_ELECTROCUTION; break;
   case 4: proprt [RAP_BRAND] = SPWPN_VAMPIRICISM; break;
   case 5: proprt [RAP_BRAND] = SPWPN_PAIN; break;
   case 6: proprt [RAP_BRAND] = SPWPN_VENOM; break;
   default: power_level -= 2;
  }
  power_level += 2;
 } else if (random4(3) == 0) proprt [RAP_BRAND] = SPWPN_NORMAL; else power_level ++;
}

if (random4(5) == 0) goto skip_mods;

if (random4(4 + power_level) == 0 && aclass != OBJ_ARMOUR) /* AC mod - not for armours */
{
 proprt [RAP_AC] = 1 + random4(3) + random4(3) + random4(3);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_AC] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

if (random4(4 + power_level) == 0) /* ev mod */
{
 proprt [RAP_EVASION] = 1 + random4(3) + random4(3) + random4(3);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_EVASION] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

if (random4(4 + power_level) == 0) /* str mod */
{
 proprt [RAP_STRENGTH] = 1 + random4(3) + random4(2);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_STRENGTH] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

if (random4(4 + power_level) == 0) /* int mod */
{
 proprt [RAP_INTELLIGENCE] = 1 + random4(3) + random4(2);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_INTELLIGENCE] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

if (random4(4 + power_level) == 0) /* dex mod */
{
 proprt [RAP_DEXTERITY] = 1 + random4(3) + random4(2);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_DEXTERITY] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

skip_mods : if (random4(15) < power_level || aclass == OBJ_WEAPONS) goto skip_combat;
/* Weapons can't get these */

if (random4(4 + power_level) == 0) /* to-hit */
{
 proprt [RAP_ACCURACY] = 1 + random4(3) + random4(2);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_ACCURACY] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

if (random4(4 + power_level) == 0) /* to-dam */
{
 proprt [RAP_DAMAGE] = 1 + random4(3) + random4(2);
 power_level ++;
 if (random4(4) == 0)
 {
  proprt [RAP_DAMAGE] -= 1 + random4(3) + random4(3) + random4(3);
  power_level --;
 }
}

skip_combat : if (random4(12) < power_level) goto finished_powers;

/* res_fire */
if (random4(4 + power_level) == 0 && (aclass != OBJ_JEWELLERY || (atype != RING_PROTECTION_FROM_FIRE && atype != RING_FIRE && atype != RING_ICE)) && (aclass != OBJ_ARMOUR || (atype != ARM_DRAGON_ARMOUR && atype != ARM_ICE_DRAGON_ARMOUR && atype != ARM_GOLD_DRAGON_ARMOUR)))
{
 proprt [RAP_FIRE] = 1;
 if (random4(5) == 0) proprt [RAP_FIRE] += 1;
 power_level ++;
}

/* res_cold */
if (random4(4 + power_level) == 0 && (aclass != OBJ_JEWELLERY || (atype != RING_PROTECTION_FROM_COLD && atype != RING_FIRE && atype != RING_ICE)) && (aclass != OBJ_ARMOUR || (atype != ARM_DRAGON_ARMOUR && atype != ARM_ICE_DRAGON_ARMOUR && atype != ARM_GOLD_DRAGON_ARMOUR)))
{
 proprt [RAP_COLD] = 1;
 if (random4(5) == 0) proprt [RAP_COLD] += 1;
 power_level ++;
}

if (random4(12) < power_level || power_level > 7) goto finished_powers;

/* res_elec */
if (random4(4 + power_level) == 0 && (aclass != OBJ_ARMOUR || atype != ARM_STORM_DRAGON_ARMOUR))
{
 proprt [RAP_ELECTRICITY] = 1;
 power_level ++;
}
/* res_poison */
if (random4(5 + power_level) == 0 && (aclass != OBJ_JEWELLERY || atype != RING_POISON_RESISTANCE) && (aclass != OBJ_ARMOUR || atype != ARM_GOLD_DRAGON_ARMOUR || atype != ARM_SWAMP_DRAGON_ARMOUR))
{
 proprt [RAP_POISON] = 1;
 power_level ++;
}

/* prot_life - no necromantic brands on weapons allowed */
if (random4(4 + power_level) == 0  && (aclass != OBJ_JEWELLERY || atype != RING_TELEPORTATION) && proprt [RAP_BRAND] != SPWPN_DRAINING && proprt [RAP_BRAND] != SPWPN_VAMPIRICISM && proprt [RAP_BRAND] != SPWPN_PAIN)
{
 proprt [RAP_NEGATIVE_ENERGY] = 1;
 power_level ++;
}

if (random4(4 + power_level) == 0 && (aclass != OBJ_JEWELLERY || atype != RING_PROTECTION_FROM_MAGIC)) /* res magic */
{
 proprt [RAP_MAGIC] = 20 + random4(100);
 power_level ++;
}

/* see_invis */
if (random4(4 + power_level) == 0 && (aclass != OBJ_JEWELLERY || atype != RING_INVISIBILITY))
{
 proprt [RAP_EYESIGHT] = 1;
 power_level ++;
}

if (random4(12) < power_level || power_level > 10) goto finished_powers;

/* turn invis */
if (random4(10) == 0 && (aclass != OBJ_JEWELLERY || atype != RING_INVISIBILITY))
{
 proprt [RAP_INVISIBLE] = 1;
 power_level ++;
}

if (random4(10) == 0 && (aclass != OBJ_JEWELLERY || atype != RING_LEVITATION)) /* levitate */
{
 proprt [RAP_LEVITATE] = 1;
 power_level ++;
}

if (random4(10) == 0) /* blink */
{
 proprt [RAP_BLINK] = 1;
 power_level ++;
}

if (random4(10) == 0 && (aclass != OBJ_JEWELLERY || atype != RING_TELEPORTATION)) /* teleport */
{
 proprt [RAP_CAN_TELEPORT] = 1;
 power_level ++;
}

if (random4(10) == 0 && (aclass != OBJ_JEWELLERY || atype != AMU_RAGE)) /* go berserk */
{
 proprt [RAP_BERSERK] = 1;
 power_level ++;
}

if (random4(10) == 0) /* sense surr */
{
 proprt [RAP_MAPPING] = 1;
 power_level ++;
}


finished_powers :

if (aclass == OBJ_ARMOUR) power_level -= 4; /* Armours get less powers, and are also less likely to be cursed that wpns */

if (random4(17) >= power_level || power_level < 2) goto finished_curses;

switch(random4(9))
{
case 0: /* makes noise */
if (aclass != 0) break;
proprt [RAP_NOISES] = 1 + random4(4);
break;
case 1: /* no magic */
proprt [RAP_PREVENT_SPELLCASTING] = 1;
break;
case 2: /* random teleport */
if (aclass != OBJ_WEAPONS) break;
proprt [RAP_CAUSE_TELEPORTATION] = 5 + random4(15);
break;
case 3: /* no teleport - doesn't affect some instantaneous teleports */
if (aclass == OBJ_JEWELLERY && atype == RING_TELEPORTATION) break; /* already is a ring of tport */
if (aclass == OBJ_JEWELLERY && atype == RING_TELEPORT_CONTROL) break; /* already is a ring of tport ctrl */
proprt [RAP_BLINK] = 0;
proprt [RAP_CAN_TELEPORT] = 0;
proprt [RAP_PREVENT_TELEPORTATION] = 1;
break;
case 4: /* berserk on attack */
if (aclass != OBJ_WEAPONS) break;
proprt [RAP_ANGRY] = 1 + random4(8);
break;
case 5: /* susceptible to fire */
if (aclass == OBJ_JEWELLERY && (atype == RING_PROTECTION_FROM_FIRE || atype == RING_FIRE || atype == RING_ICE)) break; /* already does this or something */
if (aclass == OBJ_ARMOUR && (atype == ARM_DRAGON_ARMOUR || atype == ARM_ICE_DRAGON_ARMOUR || atype == ARM_GOLD_DRAGON_ARMOUR)) break;
proprt [RAP_FIRE] = -1;
break;
case 6: /* susceptible to cold */
if (aclass == OBJ_JEWELLERY && (atype == RING_PROTECTION_FROM_COLD || atype == RING_FIRE || atype == RING_ICE)) break; /* already does this or something */
if (aclass == OBJ_ARMOUR && (atype == ARM_DRAGON_ARMOUR || atype == ARM_ICE_DRAGON_ARMOUR || atype == ARM_GOLD_DRAGON_ARMOUR)) break;
proprt [RAP_COLD] = -1;
break;
case 7: /* speed metabolism */
if (aclass == OBJ_JEWELLERY && atype == RING_HUNGER) break; /* already is a ring of hunger */
if (aclass == OBJ_JEWELLERY && atype == RING_SUSTENANCE) break; /* already is a ring of sustenance */
proprt [RAP_METABOLISM] = 1 + random4(3);
break;
case 8: /* emits mutagenic radiation - increases mpower */
proprt [RAP_MUTAGENIC] = 2 + random4(4); /* property is chance (1 in ...) of increasing mpower */
break;
}
/*
26 - +to-hit (no wpns)
27 - +to-dam (no wpns)
*/

finished_curses :

srandom(randstore);

return proprt [int(prop)];

}


std::string random_randart_characteristic()
{
	switch(random4(21)) {
		case 0: return "brightly glowing ";
		case 1: return "runed ";
		case 2: return "smoking ";
		case 3: return "bloodstained ";
		case 4: return "twisted ";
		case 5: return "shimmering ";
		case 6: return "warped ";
		case 7: return "crystal ";
		case 8: return "jewelled ";
		case 9: return "transparent ";
		case 10: return "encrusted ";
		case 11: return "pitted ";
		case 12: return "slimy ";
		case 13: return "polished ";
		case 14: return "fine ";
		case 15: return "crude ";
		case 16: return "ancient ";
		case 17: return "ichor-stained ";
		case 18: return "faintly glowing ";
		case 19: return "steaming ";
		case 20: return "shiny ";
	}
	return "";
}

std::string randart_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev)
{
	if (adam == 25) {
		struct unrandart_entry *search_unrandarts = sura;
		if (ident_lev == 0) return search_unrandarts->unid_name;
		else return search_unrandarts->name;
	}

	int globby = aclass + adam * (aplus % 100) + atype * aplus2;
	int randstore = random();

	srandom(globby);

	if (ident_lev == 0) {
		std::string result = random_randart_characteristic() + standard_name_weap(atype);
		srandom(randstore);
		return result;
	}

	std::string result;
	if (random4(2) == 0) {
		result = standard_name_weap(atype) + rand_wpn_names [random4(390)];
	} else {
		result = standard_name_weap(atype);
		if (random4(3) == 0) {
			result += " of " + make_name(random4(250), random4(250), random4(250), 3);
		} else {
			result += " \"" + make_name(random4(250), random4(250), random4(250), 3) + "\"";
		}
	}
	srandom(randstore);
	return result;
}



std::string randart_armour_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev)
{
	if (adam == 25) {
		struct unrandart_entry *search_unrandarts = sura;
		if (ident_lev == 0) return search_unrandarts->unid_name;
		else return search_unrandarts->name;
	}

	int globby = aclass + adam * (aplus % 100) + atype * aplus2;
	int randstore = random();
	srandom(globby);
	std::string result;
	if (ident_lev == 0) {
		result = random_randart_characteristic() + standard_name_armour(atype, aplus2);
		srandom(randstore);
		return result;
	}

	if (random4(2) == 0) {
		result = standard_name_armour(atype, aplus2) + rand_armour_names [random4(71)];
	} else {
		result = standard_name_armour(atype, aplus2);
		if (random4(3) == 0) {
			result += " of " + make_name(random4(250), random4(250), random4(250), 3);
		} else {
			result += " \"" + make_name(random4(250), random4(250), random4(250), 3) + "\"";
		}
	}
	srandom(randstore);
	return result;
}


std::string randart_ring_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev)
{
	if (adam == 201) {
		struct unrandart_entry *search_unrandarts = sura;
		if (ident_lev == 0) return search_unrandarts->unid_name;
		else return search_unrandarts->name;
	}
	int globby = aclass + adam * (aplus % 100) + atype * aplus2;
	int randstore = random();
	srandom(globby);
	std::string result;
	std::string type = (atype < AMU_RAGE) ? "ring" : "amulet";
	if (ident_lev == 0) {
		result = random_randart_characteristic() + type;
		srandom(randstore);
		return result;
	}
	if (random4(5) == 0) {
		result = type + rand_armour_names [random4(71)];
	} else {
		result = type;
		if (random4(3) == 0) {
			result += " of " + make_name(random4(250), random4(250), random4(250), 3);
		} else {
			result += " \"" + make_name(random4(250), random4(250), random4(250), 3) + "\"";
		}
	}
	srandom(randstore);
	return result;
}


struct unrandart_entry *seekunrandart(int aclass, int atype, int aplus, int aplus2)
{

int x = 0;

	while(x < NO_UNRANDARTS)
    {
		if (unranddata [x].ura_cl == aclass && unranddata [x].ura_ty == atype && unranddata [x].ura_pl % 100 == aplus % 100 && unranddata [x].ura_pl2 == aplus2)
        {
          return &unranddata [x];
        }
		x++;
	}	
	return seekunrandart(250, 250, 250, 250); /* Should *never* happen */

}

int find_okay_unrandart(int aclass)
{
 int x = 0;
 int ura_index [NO_UNRANDARTS];
 for (x = 0; x < NO_UNRANDARTS; x ++)
 {
   ura_index [x] = 0;
 }
 int counter = 0;

 x = 0;

	while(x < NO_UNRANDARTS)
    {
		if (unranddata [x].ura_cl == aclass && does_unrandart_exist(x) == 0)
        {
         ura_index [counter] = x;
         counter ++;
        }
		x++;
	}

 if (counter == 0) return -1;

 int y = ura_index [random4(counter)];

/* output_value("Found randart no: ", y); */

 return y;
}

void make_item_unrandart(int x, int ura_item)
{

 mitm.iclass [ura_item] = unranddata [x].ura_cl;
 mitm.itype [ura_item] = unranddata [x].ura_ty;
 mitm.iplus [ura_item] = unranddata [x].ura_pl;
 mitm.iplus2 [ura_item] = unranddata [x].ura_pl2;
 mitm.icol [ura_item] = unranddata [x].ura_col;
 mitm.idam [ura_item] = 25;
 if (mitm.iclass [ura_item] == OBJ_JEWELLERY) mitm.idam [ura_item] = 201;
 set_unrandart_exist(x, 1);

}

std::string unrandart_descrip(int which_descrip, int aclass, int atype, int aplus, int aplus2)
{
 switch(which_descrip)
 {
  case 0: return sura->spec_descrip1;
  case 1: return sura->spec_descrip2;
  case 2: return sura->spec_descrip3;
 }

 return "Unknown.";

/* Eventually it would be great to have randomly generated descriptions for
randarts. */

}


std::string standard_name_weap(int item_typ)
{
	switch(item_typ) {
		case WPN_CLUB: return "club";
		case WPN_MACE: return "mace";
		case WPN_FLAIL: return "flail";
		case WPN_DAGGER: return "dagger";
		case WPN_MORNINGSTAR: return "morningstar";
		case WPN_SHORT_SWORD: return "short sword";
		case WPN_LONG_SWORD: return "long sword";
		case WPN_GREAT_SWORD: return "great sword";
		case WPN_SCIMITAR: return "scimitar";
		case WPN_HAND_AXE: return "hand axe";
		case WPN_BATTLEAXE: return "battleaxe";
		case WPN_SPEAR: return "spear";
		case WPN_HALBERD: return "halberd";
		case WPN_SLING: return "sling";
		case WPN_BOW: return "bow";
		case WPN_CROSSBOW: return "crossbow";
		case WPN_HAND_CROSSBOW: return "hand crossbow";
		case WPN_GLAIVE: return "glaive";
		case WPN_QUARTERSTAFF: return "quarterstaff";
		case WPN_SCYTHE: return "scythe";
		case WPN_GIANT_CLUB: return "giant club";
		case WPN_GIANT_SPIKED_CLUB: return "giant spiked club";
		case WPN_EVENINGSTAR: return "eveningstar";
		case WPN_QUICK_BLADE: return "quick blade";
		case WPN_KATANA: return "katana";
		case WPN_EXECUTIONERS_AXE: return "executioner's axe";
		case WPN_DOUBLE_SWORD: return "double sword";
		case WPN_TRIPLE_SWORD: return "triple sword";
		case WPN_HAMMER: return "hammer";
		case WPN_ANCUS: return "ancus";
		case WPN_WHIP: return "whip";
		case WPN_SABRE: return "sabre";
		case WPN_DEMON_BLADE: return "demon blade";
		case WPN_DEMON_WHIP: return "demon whip";
	}
	return "";
}

std::string standard_name_armour(int item_typ, int item_plus2)
{
	switch(item_typ) {
		case ARM_ROBE: return "robe";
		case ARM_LEATHER_ARMOUR: return "leather armour";
		case ARM_RING_MAIL: return "ring mail";
		case ARM_SCALE_MAIL: return "scale mail";
		case ARM_CHAIN_MAIL: return "chain mail";
		case ARM_SPLINT_MAIL: return "splint mail";
		case ARM_BANDED_MAIL: return "banded mail";
		case ARM_PLATE_MAIL: return "plate mail";
		case ARM_SHIELD: return "shield";
		case ARM_CLOAK: return "cloak";
		case ARM_HELMET: switch(item_plus2) {
							 case 0: return "helmet";
							 case 1: return "helm";
							 case 2: return "cap";
							 case 3: return "wizard's hat";
							 default: return "";
						 }
		case ARM_GLOVES: return "gloves";
		case ARM_BOOTS: switch(item_plus2) {
							case 0: return "boots";
							case 1: return "naga barding";
							case 2: return "centaur barding";
							default: return "";
						}
		case ARM_BUCKLER: return "buckler";
		case ARM_LARGE_SHIELD: return "large shield";
		case ARM_DRAGON_HIDE: return "dragon hide";
		case ARM_TROLL_HIDE: return "troll hide";
		case ARM_CRYSTAL_PLATE_MAIL: return "crystal plate mail";
		case ARM_DRAGON_ARMOUR: return "dragon armour";
		case ARM_TROLL_LEATHER_ARMOUR: return "troll leather armour";
		case ARM_ICE_DRAGON_HIDE: return "ice dragon hide";
		case ARM_ICE_DRAGON_ARMOUR: return "ice dragon armour";
		case ARM_STEAM_DRAGON_HIDE: return "steam dragon hide";
		case ARM_STEAM_DRAGON_ARMOUR: return "steam dragon armour";
		case ARM_MOTTLED_DRAGON_HIDE: return "mottled dragon hide";
		case ARM_MOTTLED_DRAGON_ARMOUR: return "mottled dragon armour";
		case ARM_STORM_DRAGON_HIDE: return "storm dragon hide";
		case ARM_STORM_DRAGON_ARMOUR: return "storm dragon armour";
		case ARM_GOLD_DRAGON_HIDE: return "gold dragon hide";
		case ARM_GOLD_DRAGON_ARMOUR: return "gold dragon armour";
		case ARM_ANIMAL_SKIN: return "animal skin";
		case ARM_SWAMP_DRAGON_HIDE: return "swamp dragon hide";
		case ARM_SWAMP_DRAGON_ARMOUR: return "swamp dragon armour";
	}
	return "";
}



int random4(int randmax)
{
 if (randmax <= 0) return 0;
 return random() % randmax;
}




