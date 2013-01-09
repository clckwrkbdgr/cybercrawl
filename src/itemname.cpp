
#include "linuxlib.h"

#include <string.h>
#include <stdlib.h>

#include "externs.h"
#include "enum.h"

#include "itemname.h"
#include "macro.h"
#include "mstruct.h"
#include "randart.h"
#include "skills2.h"

char reduce(int reducee);
char is_a_vowel(int let);
char retbit(char sed);
char retvow(char sed);
std::string make_name(int var1, int var2, int var3, char ncase);


char id [4] [50];

int prop [4] [50] [3];
int mss [20] [50];



void it_name(int itn, char des, char str_pass [80])
{
		item_name(mitm.iplus2 [itn], mitm.iclass [itn], mitm.itype [itn], mitm.idam [itn], mitm.iplus [itn], mitm.iquant [itn], mitm.iid [itn], des, str_pass);
}

void in_name(int inn, char des, char str_pass [80])
{
		item_name(you[0].inv_plus2 [inn], you[0].inv_class [inn], you[0].inv_type [inn], you[0].inv_dam [inn], you[0].inv_plus [inn], you[0].inv_quant [inn], you[0].inv_ident [inn], des, str_pass);
}

char item_name(int item_plus2, char item_clas, char item_typ, int item_da, int it_plus, int it_quant, char ident_lev, char descrip, char glag [60])
{

char itm_name [60] = "";
char tmp_quant [5];

strcpy(glag, "");

strcpy(itm_name, item_name_2(item_plus2, item_clas, item_typ, item_da, it_plus, it_quant, ident_lev).c_str());


if (item_clas == OBJ_ORBS || (item_clas == OBJ_WEAPONS && item_da > 180 && ident_lev > 0) || (item_clas == OBJ_MISCELLANY && item_typ == MISC_HORN_OF_GERYON && ident_lev > 0) || (item_clas == OBJ_JEWELLERY && (item_da == 200 || item_da == 201) && ident_lev >= 2) || ((item_clas == OBJ_WEAPONS || item_clas == OBJ_ARMOUR) && item_da % 30 >= 25 && ident_lev > 0))
{
 switch(descrip)
 {
  case 2:
  case 4:
 	case 0: strcat(glag, "The "); break;
  case 3:
  case 5:
  case 7: // !
 	case 1: strcat(glag, "the "); break;
 }
} else
if (it_quant > 1)
{
switch(descrip)
{
	case 0: strcat(glag, "The "); break;
	case 1: strcat(glag, "the "); break;
	case 2: break; // A/An
	case 3: break; // a/an
	case 4: strcat(glag, "Your "); break;
	case 5: strcat(glag, "your "); break;
/*	case 6: nothing */
	case 7: strcat(glag, "its "); break;
}

	itoa(it_quant, tmp_quant, 10);
	strcat(glag, tmp_quant);
	strcat(glag, " ");
	} else
{
	switch(descrip)
	{
		case 0: strcat(glag, "The "); break;
		case 1: strcat(glag, "the "); break;
		case 2: strcat(glag, "A");
			if ((itm_name [0] == 97) ^ (itm_name [0] == 101) ^ (itm_name [0] == 105) ^ (itm_name [0] == 111) ^ (itm_name [0] == 117))
			{
				strcat(glag, "n");
			}
		strcat(glag, " ");
		break; // A/An
		case 3:strcat(glag, "a");
			if ((itm_name [0] == 97) ^ (itm_name [0] == 101) ^ (itm_name [0] == 105) ^ (itm_name [0] == 111) ^ (itm_name [0] == 117))
			{
				strcat(glag, "n");
			}
		strcat(glag, " ");
		break; // a/an
		case 4: strcat(glag, "Your "); break;
		case 5: strcat(glag, "your "); break;
/*		case 6: nothing */
		case 7: strcat(glag, "its "); break;
	}

} // end of else

strcat(glag, itm_name);

return 1;

} // end of char name_item

struct ItemInfo {
	int item_plus2;
	int item_clas;
	int item_typ;
	int item_da;
	int it_plus;
	int it_quant;
	int ident_lev;
};

std::string unrandart_weapon_name(int item_id)
{
	switch(item_id) {
		case 1: return "Singing Sword";
		case 2: return "Wrath of Trog";
		case 3: return "Scythe of Curses";
		case 4: return "Mace of Variability";
		case 5: return "Glaive of Prune";
		case 6: return "Sceptre of Torment";
		case 7: return "Sword of Zonguldrok";
		case 8: return "Sword of Cerebov";
		case 9: return "Staff of Dispater";
		case 10: return "Sceptre of Asmodeus";
		case 11: return "Sword of Power";
		case 12: return "Knife of Accuracy";
		case 13: return "Staff of Olgreb";
		case 14: return "Vampire's Tooth";
		case 15: return "Staff of Wucad Mu"; // this random name generator makes some weird things
	}
	return "";
}

std::string unique_weapon_name(int item_id)
{
	switch(item_id) {
		case 1:  return  "golden long sword";
		case 2:  return  "bloodstained battleaxe";
		case 3:  return  "warped scythe";
		case 4:  return  "shimmering mace";
		case 5:  return  "purple glaive";
		case 6:  return  "jewelled golden mace";
		case 7:  return  "bone long sword";
		case 8:  return  "great serpentine sword";
		case 9:  return  "golden staff";
		case 10: return  "ruby sceptre";
		case 11: return  "chunky great sword";
		case 12: return  "thin dagger";
		case 13: return  "green glowing staff";
		case 14: return  "ivory dagger";
		case 15: return  "quarterstaff";
	}
	return "";
}

std::string branded_weapon_name(int item_id)
{
	switch(item_id) {
		case DWPN_RUNED:   return "runed ";
		case DWPN_GLOWING: return "glowing ";
		case DWPN_ORCISH:  return "terrorist ";
		case DWPN_ELVEN:   return "ninja ";
		case DWPN_DWARVEN: return "engineering ";
	}
	return "";
}

std::string vorpal_type_name(int item_class, int item_type)
{
	switch(damage_type(item_class, item_type)) {
		case DVORP_CRUSHING: return " of crushing";
		case DVORP_SLICING:  return " of slicing";
		case DVORP_PIERCING: return " of piercing";
		case DVORP_CHOPPING: return " of chopping";
	}
	return "";
}

std::string damage_type_name(int item_id, int item_class, int item_type)
{
	switch(item_id) {
		// 13 - vamp
		// 25 - 29 are randarts
		case SPWPN_NORMAL: break;
		case SPWPN_FLAMING:       return " of flaming";
		case SPWPN_FREEZING:      return " of freezing";
		case SPWPN_HOLY_WRATH:    return " of holy wrath";
		case SPWPN_ELECTROCUTION: return " of electrocution";
		case SPWPN_ORC_SLAYING:   return " of terrorist slaying";
		case SPWPN_VENOM:         return " of venom";
		case SPWPN_PROTECTION:    return " of protection";
		case SPWPN_DRAINING:      return " of draining";
		case SPWPN_SPEED:         return " of speed";
		case SPWPN_VORPAL:        return vorpal_type_name(item_class, item_type);
		case SPWPN_FLAME:         return " of flame";
		case SPWPN_FROST:         return " of frost";
		case SPWPN_DISRUPTION:    return " of disruption";
		case SPWPN_PAIN:          return " of pain";
		case SPWPN_DISTORTION:    return " of distortion";
	}
	return "";
}

std::string base_weapon_name(ItemInfo item)
{
	std::string result;
	if (item.ident_lev > 0) {
		if (item.it_plus >= 130) {
			result += "virused ";
		}
	}

	if (item.ident_lev > 2) {
		if (item.it_plus % 100 == 50 && item.item_plus2 == 50) {
			result += "+0 ";
		} else {
			bool plused = (item.it_plus >= 50 && (item.it_plus <= 130 || item.it_plus >= 150));
			int enchantment = (item.it_plus > 130) ? (item.it_plus - 150) : (item.it_plus - 50);
			Format format("@1@2,@3@4 ");
			format<< (plused ? "+" : "") << enchantment << ((item.item_plus2 >= 50) ? "+" : "") << (item.item_plus2 - 50);
			result += format.str();
		}
	}

	if (item.item_da % 30 >= 25 && item.item_da % 30 <= 29) {
		return result + randart_name(item.item_clas, item.item_typ, item.item_da, item.it_plus, item.item_plus2, item.ident_lev);
	}

	if (item.item_da > 180) {
		if(item.ident_lev > 0) {
			return result + unrandart_weapon_name(item.item_da - 180);
		}
		return result + unique_weapon_name(item.item_da - 180);
	}

	if (item.ident_lev <= 1 || item.item_da % 30 == SPWPN_NORMAL || item.item_da / 30 >= DWPN_ORCISH) {
		result += branded_weapon_name(item.item_da / 30);
	}

	if(item.ident_lev > 1) {
		if(item.item_da % 30 == SPWPN_VAMPIRICISM) {
			result += "chemist ";
		}
	}

	char str_pass2 [80];
	standard_name_weap(item.item_typ, str_pass2); /* in randart.cc */
	result += str_pass2;

	if (item.ident_lev > 1) {
		result += damage_type_name(item.item_da % 30, item.item_clas, item.item_typ);
	}

	return result;
}

std::string ammunition_brand(int item_id)
{
	switch(item_id) {
		case DWPN_ORCISH: return "terrorist ";
		case DWPN_ELVEN: return "ninja ";
		case DWPN_DWARVEN: return "engineering ";
	}
	return "";
}

std::string ammunition_type(int item_typ)
{
	switch(item_typ) {
		case MI_STONE: return "stone";
		case MI_ARROW: return "arrow";
		case MI_BOLT: return "bolt";
		case MI_DART: return "dart";
		case MI_EGGPLANT: return "eggplant";
		case MI_LARGE_ROCK: return "large rock";
	}
	return "";
}

std::string ammunition_damage_type(int type)
{
	switch(type) {
		case 1: return " of flame";
		case 2: return " of ice";
	}
	return "";
}

std::string ammunition_name(ItemInfo item)
{
	std::string result;
	if (item.item_da % 30 == 3 || item.item_da % 30 == 4) {
		result += "poisoned ";
	}
	if (item.ident_lev > 2) {
		Format format("@1@2 ");
		int gokh = item.it_plus;
		format << ((gokh >= 50 && (gokh <= 130 || gokh >= 150)) ? "+" : "") << ((gokh > 130) ? (gokh - 150) : (gokh - 50));
		result += format.str();
	}
	if (item.item_da != 0) {
		result += ammunition_brand(item.item_da / 30);
	}
	result += ammunition_type(item.item_typ);
	if(item.it_quant > 1) {
		result += "s";
	}
	if (item.ident_lev > 1) {
		result += ammunition_damage_type(item.item_da % 30);
	}
	return result;
}

std::string armour_brand(int item_id, int item_typ, int item_plus2)
{
	bool is_embroidered = (item_typ == ARM_ROBE || item_typ == ARM_CLOAK || item_typ == ARM_GLOVES || item_typ == ARM_BOOTS || (item_typ == ARM_HELMET && item_plus2 >= 2));
	switch(item_id) {
		case DARM_EMBROIDERED_SHINY:
			if(is_embroidered) {
				return "embroidered ";
			} else if (item_typ != 1) {
				return "shiny ";
			} else {
				return "";
			}
		case DARM_RUNED: return "runed ";
		case DARM_GLOWING: return "glowing ";
		case DARM_ELVEN: return "ninja ";
		case DARM_DWARVEN: return "engineering ";
		case DARM_ORCISH: return "terrorist ";
	}
	return "";
}

std::string helmet_type(int type)
{
	switch(type) {
		case DHELM_WINGED: return "winged "; break;
		case DHELM_HORNED: return "horned "; break;
		case DHELM_CRESTED: return "crested "; break;
		case DHELM_PLUMED: return "plumed "; break;
		case DHELM_SPIKED: return "spiked "; break;
		case DHELM_VISORED: return "visored "; break;
		case DHELM_JEWELLED: return "jewelled "; break;
	}
	return "";
}

std::string armour_ability(int type)
{
	switch(type) {
		case SPARM_RUNNING: return " of running";
		case SPARM_FIRE_RESISTANCE: return " of fire resistance";
		case SPARM_COLD_RESISTANCE: return " of cold resistance";
		case SPARM_POISON_RESISTANCE: return " of poison resistance";
		case SPARM_SEE_INVISIBLE: return " of see invisible";
		case SPARM_DARKNESS: return " of darkness";
		case SPARM_STRENGTH: return " of strength";
		case SPARM_DEXTERITY: return " of dexterity";
		case SPARM_INTELLIGENCE: return " of intelligence";
		case SPARM_PONDEROUSNESS: return " of ponderousness";
		case SPARM_LEVITATION: return " of levitation";
		case SPARM_MAGIC_RESISTANCE: return " of hacking resistance";
		case SPARM_PROTECTION: return " of protection";
		case SPARM_STEALTH: return " of stealth";
		case SPARM_RESISTANCE: return " of resistance";
		case SPARM_POSITIVE_ENERGY: return " of positive energy";
		case SPARM_ARCHMAGI: return " of the Superhackers";
		case SPARM_PRESERVATION: return " of preservation";
	}
	return "";
}

std::string armour_name(ItemInfo item)
{
	std::string result;
	if (item.ident_lev > 0) {
		if (item.it_plus >= 130) {
			result += "virused ";
		}
	}
	if (item.ident_lev > 2) {
		Format format("@1@2 ");
		int gokh = item.it_plus;
		format << ((gokh >= 50 && (gokh <= 130 || gokh >= 150)) ? "+" : "") << ((gokh > 130) ? (gokh - 150) : (gokh - 50));
		result += format.str();
	}
	if(item.item_typ == ARM_GLOVES || (item.item_typ == ARM_BOOTS && item.item_plus2 == 0)) {
		result += "pair of ";
	}
	if (item.item_da % 30 >= 25 && item.item_da % 30 <= 29) {
		return result + randart_armour_name(item.item_clas, item.item_typ, item.item_da, item.it_plus, item.item_plus2, item.ident_lev);
	}
	if (item.ident_lev < 2 || item.item_da % 30 == 0 || item.item_da / 30 >= 4) {
		if (item.item_typ != 10) {
			result += armour_brand((int) item.item_da / 30, item.item_typ, item.item_plus2);
		}
		if (item.item_typ == ARM_HELMET && item.item_plus2 <= 1) {
			result += helmet_type(item.item_da / 30);
		}
	}
	char str_pass2 [80];
	standard_name_armour(item.item_typ, item.item_plus2, str_pass2); /* in randart.cc */
	result += str_pass2;
	if (item.ident_lev > 1) {
		result += armour_ability(item.item_da % 30);
	}
	return result;
}

std::string identified_gun_name(int item_typ)
{
	switch(item_typ) {
		case WAND_FLAME: return "gun of flame";
		case WAND_FROST: return "gun of frost";
		case WAND_SLOWING: return "gun of slowing";
		case WAND_HASTING: return "gun of hasting";
		case WAND_MAGIC_DARTS: return "gun of energy darts";
		case WAND_HEALING: return "gun of healing";
		case WAND_PARALYSIS: return "gun of paralysis";
		case WAND_FIRE: return "gun of fire";
		case WAND_COLD: return "gun of cold";
		case WAND_CONFUSION: return "gun of confusion";
		case WAND_INVISIBILITY: return "gun of invisibility";
		case WAND_DIGGING: return "gun of digging";
		case WAND_FIREBALL: return "gun of fireball";
		case WAND_TELEPORTATION: return "gun of ventilation";
		case WAND_LIGHTNING: return "gun of lightning";
		case WAND_POLYMORPH_OTHER: return "gun of polymorph other";
		case WAND_ENSLAVEMENT: return "gun of enslavement";
		case WAND_DRAINING: return "gun of draining";
		case WAND_RANDOM_EFFECTS: return "gun of random effects";
		case WAND_DISINTEGRATION: return "gun of disintegration";
	}
	return "";
}

std::string gun_material(int type)
{
	switch(type) {
		case 0: return "iron gun";
		case 1: return "brass gun";
		case 2: return "bone gun";
		case 3: return "wooden gun";
		case 4: return "copper gun";
		case 5: return "gold gun";
		case 6: return "silver gun";
		case 7: return "bronze gun";
		case 8: return "ivory gun";
		case 9: return "glass gun";
		case 10: return "lead gun";
		case 11: return "plastic gun";
	}
	return "";
}

std::string gun_characteristic(int type)
{
	switch(type) {
		case 1: return "jewelled ";
		case 2: return "curved ";
		case 3: return "long ";
		case 4: return "short ";
		case 5: return "twisted ";
		case 6: return "crooked ";
		case 7: return "forked ";
		case 8: return "shiny ";
		case 9: return "blackened ";
		case 10: return "tapered ";
		case 11: return "glowing ";
		case 12: return "worn ";
		case 13: return "encrusted ";
		case 14: return "runed ";
		case 15: return "sharpened ";
	}
	return "";
}

std::string gun_name(ItemInfo item)
{
	std::string result;
	if (id [0] [int(item.item_typ)] == 1) {
		result += identified_gun_name(item.item_typ);
	} else {
		if (item.item_da <= 11) {
			result += gun_material(item.item_da);
		} else {
			result += gun_characteristic(item.item_da / 12) + gun_material(item.item_da);
		}
	}
	if (item.ident_lev > 1) {
		Format format(" (@1)");
		format << ((item.it_plus > 80) ? (item.it_plus - 100) : (item.it_plus));
		result += format.str();
	}
	return result;
}

std::string identified_vial_type(int type)
{
	switch(type) {
		case POT_HEALING: return "healing";
		case POT_HEAL_WOUNDS: return "heal wounds";
		case POT_SPEED: return "speed";
		case POT_MIGHT: return "might";
		case POT_GAIN_STRENGTH: return "gain strength";
		case POT_GAIN_DEXTERITY: return "gain dexterity";
		case POT_GAIN_INTELLIGENCE: return "gain intelligence";
		case POT_LEVITATION: return "levitation";
		case POT_POISON: return "poison";
		case POT_SLOWING: return "slowing";
		case POT_PARALYSIS: return "paralysis";
		case POT_CONFUSION: return "confusion";
		case POT_INVISIBILITY: return "invisibility";
		case POT_PORRIDGE: return "porridge";
		case POT_DEGENERATION: return "degeneration";
		case POT_DECAY: return "decay"; // Rare!
		case POT_WATER: return "water";
		case POT_EXPERIENCE: return "experience";
		case POT_MAGIC: return "energy";
		case POT_RESTORE_ABILITIES: return "restore abilities";
		case POT_STRONG_POISON: return "strong poison";
		case POT_BERSERK_RAGE: return "battle mode";
		case POT_CURE_MUTATION: return "cure mutation";
		case POT_MUTATION: return "mutation";
	}
	return "";
}

std::string vial_colour(int type)
{
	switch(type) {
		case 0: return "clear potion";
		case 1: return "blue potion";
		case 2: return "black potion";
		case 3: return "silvery potion";
		case 4: return "cyan potion";
		case 5: return "purple potion";
		case 6: return "orange potion";
		case 7: return "inky potion";
		case 8: return "red potion";
		case 9: return "yellow potion";
		case 10: return "green potion";
		case 11: return "brown potion";
		case 12: return "pink potion";
		case 13: return "white potion";
	}
	return "";
}

std::string vial_characteristic(int type)
{
	switch(type) {
		case 0: return "bubbling ";
		case 1: return "lumpy ";
		case 2: return "fuming ";
		case 3: return "smoky ";
		case 4: return "fizzy ";
		case 5: return "glowing ";
		case 6: return "sedimented ";
		case 7: return "metallic ";
		case 8: return "murky ";
		case 9: return "gluggy ";
		case 10: return "viscous ";
		case 11: return "oily ";
		case 12: return "slimy ";
	}
	return "";
}

std::string vial_name(ItemInfo item)
{
	std::string result;
	if(id [3] [int(item.item_typ)] == 1) {
		if (item.it_quant == 1) {
			result += "vial of ";
		} else {
			result += "vials of ";
		}
	}
	if(id [3] [int(item.item_typ)] == 1) {
		return result + identified_vial_type(item.item_typ);
	}
	if(item.item_da <= 13) {
		result += vial_colour(item.item_da);
	} else {
		result += vial_characteristic(item.item_da / 14) + vial_colour(item.item_da);
	}
	if(item.it_quant > 1) {
		result += "s";
	}
	return result;
}

std::string chunk_name(ItemInfo item)
{
	char gmo_n2[80];
	moname(item.it_plus, 0, 1, 100, gmo_n2);
	Format format("@1chunk@2 of @3 flesh");
	format << ((item.item_da < 100) ? "rotting " : "") << ((item.it_quant > 1) ? "s" : "") << gmo_n2;
	return format.str();
}

std::string base_food_name(ItemInfo item)
{
	// Note: If I add another food type, must set for carnivorous chars (Kobolds and mutants)
	switch(item.item_typ) {
		case FOOD_MEAT_RATION: return "meat ration";
		case FOOD_BREAD_RATION: return "bread ration";
		case FOOD_PEAR: return "pear";
		case FOOD_APPLE: return "apple"; // make this less common.
		case FOOD_CHOKO: return "choko";
		case FOOD_HONEYCOMB: return "honeycomb";
		case FOOD_ROYAL_JELLY: return "royal jell"; // maybe a joke monster of the same name? - mix something with jelly crystals?
		case FOOD_SNOZZCUMBER: return "snozzcumber";
		case FOOD_PIZZA: return "slice of pizza";
		case FOOD_APRICOT: return "apricot";
		case FOOD_ORANGE: return "orange";
		case FOOD_BANANA: return "banana";
		case FOOD_STRAWBERRY: return "strawberr";
		case FOOD_RAMBUTAN: return "rambutan";
		case FOOD_LEMON: return "lemon";
		case FOOD_GRAPE: return "grape";
		case FOOD_SULTANA: return "sultana";
		case FOOD_LYCHEE: return "lychee";
		case FOOD_BEEF_JERKY: return "beef jerk";
		case FOOD_CHEESE: return "cheese";
		case FOOD_SAUSAGE: return "sausage";
		case FOOD_CHUNK: return chunk_name(item);
	}
	return "";
}

std::string food_name(ItemInfo item)
{
	std::string result = base_food_name(item);
	if (item.item_typ == FOOD_ROYAL_JELLY || item.item_typ == FOOD_STRAWBERRY || item.item_typ == FOOD_BEEF_JERKY) {
		if (item.it_quant > 1) {
			result += "ie";
		} else {
			result += "y";
		}
	}
	return result;
}

std::string identified_device_name(int type)
{
	switch(type) {
		case SCR_IDENTIFY: return "of identify";
		case SCR_TELEPORTATION: return "of ventilation";
		case SCR_FEAR: return "of fear";
		case SCR_NOISE: return "of noise";
		case SCR_REMOVE_CURSE: return "of remove virus";
		case SCR_DETECT_CURSE: return "of detect virus";
		case SCR_SUMMONING: return "of summoning";
		case SCR_ENCHANT_WEAPON_I: return "of upgrade weapon I";
		case SCR_ENCHANT_ARMOUR: return "of upgrade armour";
		case SCR_TORMENT: return "of torment";
		case SCR_RANDOM_USELESSNESS: return "of hologram";
		case SCR_CURSE_WEAPON: return "of virus weapon";
		case SCR_CURSE_ARMOUR: return "of virus armour";
		case SCR_IMMOLATION: return "of immolation";
		case SCR_BLINKING: return "of blinking";
		case SCR_PAPER: return "of paper";
		case SCR_MAGIC_MAPPING: return "of cyber mapping";
		case SCR_FORGETFULNESS: return "of uninstalling";
		case SCR_ACQUIREMENT: return "of 3D printing";
		case SCR_ENCHANT_WEAPON_II: return "of upgrade weapon II";
		case SCR_VORPALISE_WEAPON: return "of vorpalise weapon";
		case SCR_RECHARGING: return "of recharging";
		case SCR_ENCHANT_WEAPON_III: return "of upgrade weapon III";
	}
	return "";
}

std::string device_name(ItemInfo item)
{
	std::string result;
	if (item.it_quant == 1) {
		result += "device ";
	} else {
		result += "devices ";
	}
	if (id [1] [int(item.item_typ)] == 1) {
		return result + identified_device_name(item.item_typ);
	}
	return "labelled " + make_name(item.item_da, item.it_plus, item.item_clas, 2);
}

std::string identified_implant_name(int type)
{
	switch(type) {
		case RING_REGENERATION: return "chip of regeneration";
		case RING_PROTECTION: return "chip of protection";
		case RING_PROTECTION_FROM_FIRE: return "chip of protection from fire";
		case RING_POISON_RESISTANCE: return "chip of poison resistance";
		case RING_PROTECTION_FROM_COLD: return "chip of protection from cold";
		case RING_STRENGTH: return "chip of strength";
		case RING_SLAYING: return "chip of slaying";
		case RING_SEE_INVISIBLE: return "chip of see invisible";
		case RING_INVISIBILITY: return "chip of invisibility";
		case RING_HUNGER: return "chip of hunger";
		case RING_TELEPORTATION: return "chip of ventilation";
		case RING_EVASION: return "chip of evasion";
		case RING_SUSTAIN_ABILITIES: return "chip of sustain abilities";
		case RING_SUSTENANCE: return "chip of sustenance";
		case RING_DEXTERITY: return "chip of dexterity";
		case RING_INTELLIGENCE: return "chip of intelligence";
		case RING_WIZARDRY: return "chip of hacking";
		case RING_MAGICAL_POWER: return "chip of cyber power";
		case RING_LEVITATION: return "chip of levitation";
		case RING_LIFE_PROTECTION: return "chip of life protection";
		case RING_PROTECTION_FROM_MAGIC: return "chip of protection from hacking";
		case RING_FIRE: return "chip of fire";
		case RING_ICE: return "chip of ice";
		case RING_TELEPORT_CONTROL: return "chip of ventilation control";
		case AMU_RAGE: return "implant of rage";
		case AMU_MAINTAIN_SPEED: return "implant of maintain speed";
		case AMU_CLARITY: return "implant of clarity";
		case AMU_WARDING: return "implant of warding";
		case AMU_RESIST_CORROSION: return "implant of resist corrosion";
		case AMU_THE_GOURMAND: return "implant of the gourmand";
		case AMU_CONSERVATION: return "implant of conservation";
		case AMU_CONTROLLED_FLIGHT: return "implant of controlled flight";
		case AMU_INACCURACY: return "implant of inaccuracy";
		case AMU_RESIST_MUTATION: return "implant of resist mutation";
	}
	return "";
}

std::string chip_second_characteristic(int type)
{
	switch(type) {
		case 1: return "encrusted ";
		case 2: return "glowing ";
		case 3: return "tubular ";
		case 4: return "runed ";
		case 5: return "blackened ";
		case 6: return "scratched ";
		case 7: return "small ";
		case 8: return "large ";
		case 9: return "twisted ";
		case 10: return "shiny ";
		case 11: return "notched ";
		case 12: return "knobbly ";
	}
	return "";
}

std::string basic_chip_name(int type)
{
	switch(type) {
		case 0: return "wooden chip";
		case 1: return "silver chip";
		case 2: return "golden chip";
		case 3: return "iron chip";
		case 4: return "steel chip";
		case 5: return "bronze chip";
		case 6: return "brass chip";
		case 7: return "copper chip";
		case 8: return "granite chip";
		case 9: return "ivory chip";
		case 10: return "bone chip";
		case 11: return "marble chip";
		case 12: return "jade chip";
		case 13: return "glass chip";
	}
	return "";
}

std::string implant_second_characteristic(int type)
{
	switch(type) {
		case 0: return "dented ";
		case 1: return "square ";
		case 2: return "thick ";
		case 3: return "thin ";
		case 4: return "runed ";
		case 5: return "blackened ";
		case 6: return "glowing ";
		case 7: return "small ";
		case 8: return "large ";
		case 9: return "twisted ";
		case 10: return "tiny ";
		case 11: return "triangular ";
		case 12: return "lumpy ";
	}
	return "";
}

std::string basic_implant_name(int type)
{
	switch(type) {
		case 0: return "zirconium implant";
		case 1: return "sapphire implant";
		case 2: return "golden implant";
		case 3: return "emerald implant";
		case 4: return "garnet implant";
		case 5: return "bronze implant";
		case 6: return "brass implant";
		case 7: return "copper implant";
		case 8: return "ruby implant";
		case 9: return "ivory implant";
		case 10: return "bone implant";
		case 11: return "platinum implant";
		case 12: return "jade implant";
		case 13: return "plastic implant";
	}
	return "";
}

std::string implant_name(ItemInfo item)
{
	std::string result;
	if (item.ident_lev > 0) {
		if (item.it_plus >= 80) {
			result += "virused ";
		}
	}

	if (item.ident_lev > 1 || id [2] [int(item.item_typ)] > 0) {
		if (item.item_da == 200 || item.item_da == 201) {
			return result += randart_ring_name(item.item_clas, item.item_typ, item.item_da, item.it_plus, item.item_plus2, item.ident_lev);
		}
	}

	if (id [2] [int(item.item_typ)] > 0) {
		if (item.ident_lev > 1 && (item.item_typ == RING_PROTECTION || item.item_typ == RING_STRENGTH || item.item_typ == RING_SLAYING || item.item_typ == RING_EVASION || item.item_typ == RING_DEXTERITY || item.item_typ == RING_INTELLIGENCE))
		{
			int gokh = item.it_plus - 50;
			bool plused = (gokh >= 0 && (gokh <= 80 || gokh >= 100));
			if(item.item_typ == RING_SLAYING) {
				Format format("@1@2, @3@4 ");
				format << (plused ? "+" : "") << ((gokh > 80) ? (gokh - 100) : gokh) << ((item.item_plus2 >= 50) ? "+" : "") << (item.item_plus2 - 50);
				result += format.str();
			} else {
				Format format("@1@2 ");
				format << plused << ((gokh > 80) ? (gokh - 100) : gokh);
				result += format.str();
			}
		}
		return result += identified_implant_name(item.item_typ);
	}
	bool is_chip = (item.item_typ < AMU_RAGE);
	if(is_chip) {
		if (item.item_da == 200 || item.item_da == 201) {
			return result + randart_ring_name(item.item_clas, item.item_typ, item.item_da, item.it_plus, item.item_plus2, item.ident_lev);
		}
		result += chip_second_characteristic(item.item_da / 13) + basic_chip_name(item.item_da % 13);
	} else {
		if (item.item_da == 200 || item.item_da == 201) {
			return result + randart_ring_name(item.item_clas, item.item_typ, item.item_da, item.it_plus, item.item_plus2, item.ident_lev);
		}
		if (item.item_da > 13) {
			result += implant_second_characteristic(item.item_da / 13);
		}
		result += basic_implant_name(item.item_da % 13);
	}
	return result;
}

std::string keycard_name(int type)
{
	switch(type) {
		case 1: return "iron ";
		case 2: return "obsidian ";
		case 4: return "icy ";
		case 5: return "bone ";
		case 13: return "slimy ";
		case 14: return "silver ";
		case 19: return "serpentine ";
		case 20: return "ninja ";
		case 21: return "golden ";
		case 22: return "decaying ";
		case 50: return "genetical "; /* found in pandemonium */
		case 51: return "trash "; /* found in abyss */
	}
	return "";
}

std::string misc_item_name(ItemInfo item)
{
	if(item.ident_lev == 0) {
		switch(item.item_typ) {
			case MISC_BOTTLED_EFREET: return "sealed bronze flask";
			case MISC_CRYSTAL_BALL_OF_SEEING: return "crystal ball";
			case MISC_AIR_ELEMENTAL_FAN: return "fan";
			case MISC_LAMP_OF_FIRE: return "lamp";
			case MISC_STONE_OF_EARTH_ELEMENTALS: return "lump of rock";
			case MISC_LANTERN_OF_SHADOWS: return "bone lantern";
			case MISC_HORN_OF_GERYON: return "silver horn";
			case MISC_BOX_OF_BEASTS: return "small ebony casket";
			case MISC_DECK_OF_WONDERS: return "nanite construction kit";
			case MISC_DECK_OF_SUMMONINGS: return "nanite construction kit";
			case MISC_CRYSTAL_BALL_OF_ENERGY: return "crystal ball";
			case MISC_EMPTY_EBONY_CASKET: return "small ebony casket";
			case MISC_CRYSTAL_BALL_OF_FIXATION: return "crystal ball";
			case MISC_DISC_OF_STORMS: return "grey disc";
			case MISC_RUNE_OF_ZOT: return keycard_name(item.it_plus) + "keycard";
			case MISC_DECK_OF_TRICKS: return "nanite construction kit";
			case MISC_DECK_OF_POWER: return "nanite construction kit";
			case MISC_PORTABLE_ALTAR_OF_NEMELEX: return "portable terminal of Triads";
		}
		return "";
	}

	switch(item.item_typ) {
		case MISC_BOTTLED_EFREET: return "bottled efreet";
		case MISC_CRYSTAL_BALL_OF_SEEING: return "crystal ball of seeing";
		case MISC_AIR_ELEMENTAL_FAN: return "air elemental fan";
		case MISC_LAMP_OF_FIRE: return "lamp of fire";
		case MISC_STONE_OF_EARTH_ELEMENTALS: return "stone of earth elementals";
		case MISC_LANTERN_OF_SHADOWS: return "lantern of shadows";
		case MISC_HORN_OF_GERYON: return "Horn of Geryon";
		case MISC_BOX_OF_BEASTS: return "box of beasts";
		case MISC_DECK_OF_WONDERS: return "nanite construction kit of wonders";
		case MISC_DECK_OF_SUMMONINGS: return "nanite construction kit of summonings";
		case MISC_CRYSTAL_BALL_OF_ENERGY: return "crystal ball of energy";
		case MISC_EMPTY_EBONY_CASKET: return "empty ebony casket";
		case MISC_CRYSTAL_BALL_OF_FIXATION: return "crystal ball of fixation";
		case MISC_DISC_OF_STORMS: return "disc of storms";
		case MISC_RUNE_OF_ZOT: return keycard_name(item.it_plus) + "keycard of Alice";
		case MISC_DECK_OF_TRICKS: return "nanite construction kit of tricks";
		case MISC_DECK_OF_POWER: return "nanite construction kit of power";
		case MISC_PORTABLE_ALTAR_OF_NEMELEX: return "portable terminal of Nemelex";
	}
	return "";
}

std::string archive_look(int type)
{
	switch(type) {
		case 0: return "";
		case 1: return "chunky ";
		case 2: return "thick ";
		case 3: return "thin ";
		case 4: return "wide ";
		case 5: return "glowing ";
		case 6: return "dog-eared ";
		case 7: return "oblong ";
		case 8: return "runed ";
		case 9: return " ";
		case 10: return " ";
		case 11: return " ";
	}
	return "";
}

std::string archive_type(int type)
{
	switch(type) {
		case 0: return "paperback archive";
		case 1: return "hardcover archive";
		case 2: return "leatherbound archive";
		case 3: return "metal-bound archive";
		case 4: return "papyrus archive";
		case 5: return "archive";
		case 6: return "archive";
	}
	return "";
}

std::string identified_archive_name(ItemInfo item)
{
	switch(item.item_typ) {
		case BOOK_MINOR_MAGIC_I: return "archive of Minor Hacking";
		case BOOK_MINOR_MAGIC_II: return "archive of Minor Hacking";
		case BOOK_MINOR_MAGIC_III: return "archive of Minor Hacking";
		case BOOK_CONJURATIONS_I: return "archive of Conjurations";
		case BOOK_CONJURATIONS_II: return "archive of Conjurations";
		case BOOK_FLAMES: return "archive of Flames";
		case BOOK_FROST: return "archive of Frost";
		case BOOK_INVOCATIONS: return "archive of Communications";
		case BOOK_FIRE: return "archive of Fire";
		case BOOK_ICE: return "archive of Ice";
		case BOOK_SURVEYANCES: return "archive of Surveyances";
		case BOOK_SPATIAL_TRANSLOCATIONS: return "archive of Spatial Translocations";
		case BOOK_ENCHANTMENTS: return "archive of Plugins";
		case BOOK_POISONINGS: return "archive of Poisonings";
		case BOOK_STORMS_AND_FIRE: return "archive of Storms and Fire";
		case BOOK_DEATH: return "archive of Death";
		case BOOK_HINDERANCE: return "archive of Hinderance";
		case BOOK_CHANGES: return "archive of Changes";
		case BOOK_TRANSFIGURATIONS: return "archive of Transfigurations";
		case BOOK_USEFUL_MAGIC: return "archive of Useful Hacking";
		case BOOK_WAR_CHANTS: return "archive of War Chants";
		case BOOK_CLOUDS: return "archive of Clouds";
		case BOOK_HEALING: return "archive of Healing";
		case BOOK_NECROMANCY: return "archive of Cyborgizationgs";
		case BOOK_NECRONOMICON: return "Cyborgizator";
		case BOOK_SUMMONINGS: return "archive of Summonings";
		case BOOK_CHARMS: return "archive of Utilities";
		case BOOK_DEMONOLOGY: return "archive of Genetics";
		case BOOK_AIR: return "archive of Air";
		case BOOK_SKY: return "archive of the Sky";
		case BOOK_DIVINATIONS: return "archive of Statistics";
		case BOOK_WARP: return "archive of the Warp";
		case BOOK_ENVENOMATIONS: return "archive of Envenomations";
		case BOOK_ANNIHILATIONS: return "archive of Annihilations";
		case BOOK_UNLIFE: return "archive of Cyborgs";
		case BOOK_DESTRUCTION: return "tome of destruction";
		case BOOK_CONTROL: return "archive of Control";
		case BOOK_MUTATIONS: return "archive of Mutations";
		case BOOK_TUKIMA: return "archive of Tukima";
		case BOOK_GEOMANCY: return "archive of Geomancy";
		case BOOK_EARTH: return "archive of Earth";
		case BOOK_MANUAL: return std::string("manual of ") + skill_name(item.it_plus);
		case BOOK_WIZARDRY: return "archive of Wizardry";
		case BOOK_POWER: return "archive of Power";
	}
	return "";
}

std::string archive_name(ItemInfo item)
{
	/*
	   Spellbook binary thing:
	   6 spells: 127
	   5 spells: 126
	   4 spells: 124
	   3 spells: 120
	   2 spells: 112
	   1 spells: 96
	   0 spells: 64
	   (assuming all from start, no empty spaces)
	   */
	std::string result;
	if (item.ident_lev == 0)
	{
		result += archive_look(item.item_da / 10) + archive_type(item.item_da % 10);
	} else {
		result += identified_archive_name(item);
	}
	return result;
}

std::string rom_device_look(int type)
{
	switch(type) {
		case 0: return "curved ";
		case 1: return "glowing ";
		case 2: return "thick ";
		case 3: return "thin ";
		case 4: return "long ";
		case 5: return "twisted ";
		case 6: return "jewelled ";
		case 7: return "runed ";
		case 8: return "smoking ";
	}
	return " ";
}

std::string identified_rom_device_name(int type)
{
	switch(type) {
		case STAFF_WIZARDRY: return " of hacking";
		case STAFF_POWER: return " of power";
		case STAFF_FIRE: return " of fire";
		case STAFF_COLD: return " of cold";
		case STAFF_POISON: return " of poison";
		case STAFF_ENERGY: return " of energy"; // crappy name. oh well
		case STAFF_DEATH: return " of death";
		case STAFF_CONJURATION: return " of conjuration";
		case STAFF_ENCHANTMENT: return " of plugins";
		case STAFF_SUMMONING_I: return " of summoning";
		case STAFF_SMITING: return " of smiting";
		case STAFF_SUMMONING_II: return " of summoning";
		case STAFF_DESTRUCTION_I: return " of destruction";
		case STAFF_DESTRUCTION_II: return " of destruction";
		case STAFF_DESTRUCTION_III: return " of destruction";
		case STAFF_DESTRUCTION_IV: return " of destruction";
		case STAFF_WARDING: return " of warding";
		case STAFF_DISCOVERY: return " of discovery";
		case STAFF_DEMONOLOGY: return " of genetics";
		case STAFF_AIR: return " of air";
		case STAFF_EARTH: return " of earth";
		case STAFF_CHANNELING: return " of channeling";
	}
	return "";
}

std::string rom_device_name(ItemInfo item)
{
	std::string result;
	if (item.ident_lev == 0) {
		result == rom_device_look(item.item_da);
	}
	if (item.ident_lev > 0 && item.item_typ >= STAFF_SMITING && item.item_typ < STAFF_AIR) {
		result += "program ";
	}
	result += "ROM";
	if (item.ident_lev > 0) {
		result += identified_rom_device_name(item.item_typ);
	}
	return result;
}

std::string corpse_name(ItemInfo item)
{
	char gmo_n [80];
	bool is_skeleton = (item.item_typ == 1);
	moname(item.it_plus, 0, 1, 100, gmo_n);
	if(is_skeleton) {
		return std::string(gmo_n) + " skeleton";
	}
	return std::string((item.item_da < 100) ? "rotting " : "") + gmo_n + " corpse";
}

std::string item_name_2(int item_plus2, char item_clas, char item_typ, int item_da, int it_plus, int it_quant, char ident_lev)
{
	ItemInfo item;
	item.item_plus2 = item_plus2;
	item.item_clas =  item_clas;
	item.item_typ =   item_typ;
	item.item_da =    item_da;
	item.it_plus =    it_plus;
	item.it_quant =   it_quant;
	item.ident_lev =  ident_lev;

	std::string result;
	bool stacks = (item_clas == OBJ_MISSILES || item_clas == OBJ_SCROLLS || item_clas == OBJ_POTIONS || (item_clas == OBJ_FOOD && item_typ == FOOD_CHUNK));
	switch(item_clas) {
		case OBJ_WEAPONS: result += base_weapon_name(item); break;
		case OBJ_MISSILES: result += ammunition_name(item); break;
		case OBJ_ARMOUR: result += armour_name(item); break;
		case OBJ_WANDS: result += gun_name(item); break;
		case OBJ_POTIONS: result += vial_name(item); break;
		case OBJ_FOOD: result += food_name(item); break;
		case OBJ_SCROLLS: result += device_name(item); break;
		case OBJ_JEWELLERY: result += implant_name(item); break;
		case OBJ_MISCELLANY: result += misc_item_name(item); break;
		case OBJ_BOOKS: result += archive_name(item); break;
		case OBJ_STAVES: result += rom_device_name(item); break;
		case OBJ_ORBS: result += "Disc of Alice"; break;
		case 15: result += "credit chip"; break;
		case OBJ_CORPSES: result += corpse_name(item);
		default: result += ""; break;
	}
	if(result.size() < 3) {
		Format format("questionable item (c@1,t@2,p@3,p(2)@4,d@5:q@6)");
		format << item_clas << item_typ << it_plus << it_plus << item_da << it_quant;
		result += format.str();
	}
	if(it_quant > 1 && !stacks) {
		result += "s";
	}
	return result;
}

void save_id(char identy [4] [50])
{

int ix = 0;
int jx = 0;

for (ix = 0; ix < 4; ix ++)
{
 for (jx = 0; jx < 50; jx ++)
 {
  identy [ix] [jx] = id [ix] [jx];
 }
}

}


void initial(void)
{
int i = 0;
int j = 0;

for (i = 0; i < 4; i ++)
{
	for (j = 0; j < 50; j ++)
	{
		id [i] [j] = 0;
	}
}

} // end of void initial


void set_id(char cla, char ty, char setting)
{
	if (cla > 99)
	{
		cla -= 100;
		id [int(cla)] [int(ty)] = setting;
		return;
	}

	switch(cla)
	{
	case 3: id [0] [int(ty)] = setting; break;
	case 6: id [1] [int(ty)] = setting; break;
	case 7: id [2] [int(ty)] = setting; break;
	case 8: id [3] [int(ty)] = setting; break;
	/* if it's none of these, will just return*/
	}
} /* end of void set_id */


char get_id(char cla_char, char ty_char)
{
	int cla = cla_char, ty = ty_char;
	if (cla > 99)
	{
		cla -= 100;
		return id [cla] [ty];
	}

	switch(cla)
	{
	case 3: return id [0] [ty];
	case 6: return id [1] [ty];
	case 7: return id [2] [ty];
	case 8: return id [3] [ty];
	/* if it's none of these, will just return 0 */
	}
return 0;

} /* end of char get_id */




int property(int pr1, int pr2, int pr3)
{
 return prop [pr1] [pr2] [pr3];
}

int mass(int pr1, int pr2)
{
 return mss [pr1] [pr2];
}

void init_properties(void)
{

	//strcpy(all_items [2] [0], "robe");
	prop [OBJ_ARMOUR] [ARM_ROBE] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_ROBE] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_ROBE] = 60;

	//strcpy(all_items [2] [0], "leather armour");
	prop [OBJ_ARMOUR] [ARM_LEATHER_ARMOUR] [PARM_AC] = 2; // AC
	prop [OBJ_ARMOUR] [ARM_LEATHER_ARMOUR] [PARM_EVASION] = -1; // evasion
	mss [OBJ_ARMOUR] [ARM_LEATHER_ARMOUR] = 150;

	//strcpy(all_items [2] [1], "ring mail");
	prop [OBJ_ARMOUR] [ARM_RING_MAIL] [PARM_AC] = 3; // AC
	prop [OBJ_ARMOUR] [ARM_RING_MAIL] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_RING_MAIL] = 300;

	//strcpy(all_items [2] [2], "scale mail");
	prop [OBJ_ARMOUR] [ARM_SCALE_MAIL] [PARM_AC] = 4; // AC
	prop [OBJ_ARMOUR] [ARM_SCALE_MAIL] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_SCALE_MAIL] = 400;

	//strcpy(all_items [2] [3], "chain mail");
	prop [OBJ_ARMOUR] [ARM_CHAIN_MAIL] [PARM_AC] = 5; // AC
	prop [OBJ_ARMOUR] [ARM_CHAIN_MAIL] [PARM_EVASION] = -3; // evasion
	mss [OBJ_ARMOUR] [ARM_CHAIN_MAIL] = 450;

	//strcpy(all_items [2] [4], "splint mail");
	prop [OBJ_ARMOUR] [ARM_SPLINT_MAIL] [PARM_AC] = 6; // AC
	prop [OBJ_ARMOUR] [ARM_SPLINT_MAIL] [PARM_EVASION] = -5; // evasion
	mss [OBJ_ARMOUR] [ARM_SPLINT_MAIL] = 550;

	//strcpy(all_items [2] [5], "banded mail");
	prop [OBJ_ARMOUR] [ARM_BANDED_MAIL] [PARM_AC] = 6; // AC
	prop [OBJ_ARMOUR] [ARM_BANDED_MAIL] [PARM_EVASION] = -4; // evasion
	mss [OBJ_ARMOUR] [ARM_BANDED_MAIL] = 500;

	//strcpy(all_items [2] [6], "plate mail");
	prop [OBJ_ARMOUR] [ARM_PLATE_MAIL] [PARM_AC] = 7; // AC
	prop [OBJ_ARMOUR] [ARM_PLATE_MAIL] [PARM_EVASION] = -5; // evasion
	mss [OBJ_ARMOUR] [ARM_PLATE_MAIL] = 650;

        // dragon hide
	prop [OBJ_ARMOUR] [ARM_DRAGON_HIDE] [PARM_AC] = 2; // AC
	prop [OBJ_ARMOUR] [ARM_DRAGON_HIDE] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_DRAGON_HIDE] = 220;

        // troll hide
	prop [OBJ_ARMOUR] [ARM_TROLL_HIDE] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_TROLL_HIDE] [PARM_EVASION] = -1; // evasion
	mss [OBJ_ARMOUR] [ARM_TROLL_HIDE] = 180;

        // crystal plate
	prop [OBJ_ARMOUR] [ARM_CRYSTAL_PLATE_MAIL] [PARM_AC] = 14; // AC
	prop [OBJ_ARMOUR] [ARM_CRYSTAL_PLATE_MAIL] [PARM_EVASION] = -8; // evasion
	mss [OBJ_ARMOUR] [ARM_CRYSTAL_PLATE_MAIL] = 1200;

        // dragon scale
	prop [OBJ_ARMOUR] [ARM_DRAGON_ARMOUR] [PARM_AC] = 8; // AC
	prop [OBJ_ARMOUR] [ARM_DRAGON_ARMOUR] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_DRAGON_ARMOUR] = 220;

        // troll leather
	prop [OBJ_ARMOUR] [ARM_TROLL_LEATHER_ARMOUR] [PARM_AC] = 2; // AC
	prop [OBJ_ARMOUR] [ARM_TROLL_LEATHER_ARMOUR] [PARM_EVASION] = -1; // evasion
	mss [OBJ_ARMOUR] [ARM_TROLL_LEATHER_ARMOUR] = 180;

        // ice dragon hide
	prop [OBJ_ARMOUR] [ARM_ICE_DRAGON_HIDE] [PARM_AC] = 2; // AC
	prop [OBJ_ARMOUR] [ARM_ICE_DRAGON_HIDE] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_ICE_DRAGON_HIDE] = 220;

        // ice dragon scale
	prop [OBJ_ARMOUR] [ARM_ICE_DRAGON_ARMOUR] [PARM_AC] = 8; // AC
	prop [OBJ_ARMOUR] [ARM_ICE_DRAGON_ARMOUR] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_ICE_DRAGON_ARMOUR] = 220;

        // steam dragon hide
	prop [OBJ_ARMOUR] [ARM_STEAM_DRAGON_HIDE] [PARM_AC] = 0; // AC
	prop [OBJ_ARMOUR] [ARM_STEAM_DRAGON_HIDE] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_STEAM_DRAGON_HIDE] = 120;

        // steam dragon armour
	prop [OBJ_ARMOUR] [ARM_STEAM_DRAGON_ARMOUR] [PARM_AC] = 3; // AC
	prop [OBJ_ARMOUR] [ARM_STEAM_DRAGON_ARMOUR] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_STEAM_DRAGON_ARMOUR] = 120;

        // mottled dragon hide
	prop [OBJ_ARMOUR] [ARM_MOTTLED_DRAGON_HIDE] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_MOTTLED_DRAGON_HIDE] [PARM_EVASION] = -1; // evasion
	mss [OBJ_ARMOUR] [ARM_MOTTLED_DRAGON_HIDE] = 150;

        // mottled dragon hide
	prop [OBJ_ARMOUR] [ARM_MOTTLED_DRAGON_ARMOUR] [PARM_AC] = 5; // AC
	prop [OBJ_ARMOUR] [ARM_MOTTLED_DRAGON_ARMOUR] [PARM_EVASION] = -1; // evasion
	mss [OBJ_ARMOUR] [ARM_MOTTLED_DRAGON_ARMOUR] = 150;

        // storm dragon hide
	prop [OBJ_ARMOUR] [ARM_STORM_DRAGON_HIDE] [PARM_AC] = 2; // AC
	prop [OBJ_ARMOUR] [ARM_STORM_DRAGON_HIDE] [PARM_EVASION] = -5; // evasion
	mss [OBJ_ARMOUR] [ARM_STORM_DRAGON_HIDE] = 400;

        // storm dragon armour
	prop [OBJ_ARMOUR] [ARM_STORM_DRAGON_ARMOUR] [PARM_AC] = 9; // AC
	prop [OBJ_ARMOUR] [ARM_STORM_DRAGON_ARMOUR] [PARM_EVASION] = -5; // evasion
	mss [OBJ_ARMOUR] [ARM_STORM_DRAGON_ARMOUR] = 400;

        // gold dragon hide
	prop [OBJ_ARMOUR] [ARM_GOLD_DRAGON_HIDE] [PARM_AC] = 2; // AC
	prop [OBJ_ARMOUR] [ARM_GOLD_DRAGON_HIDE] [PARM_EVASION] = -10; // evasion
	mss [OBJ_ARMOUR] [ARM_GOLD_DRAGON_HIDE] = 1100;

        // gold dragon armour
	prop [OBJ_ARMOUR] [ARM_GOLD_DRAGON_ARMOUR] [PARM_AC] = 10; // AC
	prop [OBJ_ARMOUR] [ARM_GOLD_DRAGON_ARMOUR] [PARM_EVASION] = -10; // evasion
	mss [OBJ_ARMOUR] [ARM_GOLD_DRAGON_ARMOUR] = 1100;

	// animal skin
	prop [OBJ_ARMOUR] [ARM_ANIMAL_SKIN] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_ANIMAL_SKIN] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_ANIMAL_SKIN] = 100;

    // swamp dragon hide
	prop [OBJ_ARMOUR] [ARM_SWAMP_DRAGON_HIDE] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_SWAMP_DRAGON_HIDE] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_SWAMP_DRAGON_HIDE] = 200;

    // swamp dragon hide
	prop [OBJ_ARMOUR] [ARM_SWAMP_DRAGON_ARMOUR] [PARM_AC] = 6; // AC
	prop [OBJ_ARMOUR] [ARM_SWAMP_DRAGON_ARMOUR] [PARM_EVASION] = -2; // evasion
	mss [OBJ_ARMOUR] [ARM_SWAMP_DRAGON_ARMOUR] = 200;



	// other armour:

	// shield:
	prop [OBJ_ARMOUR] [ARM_SHIELD] [PARM_AC] = 0; // AC
	prop [OBJ_ARMOUR] [ARM_SHIELD] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_SHIELD] = 100;

	// cloak
	prop [OBJ_ARMOUR] [ARM_CLOAK] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_CLOAK] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_CLOAK] = 20;

  	// helmet
	prop [OBJ_ARMOUR] [ARM_HELMET] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_HELMET] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_HELMET] = 80;


	// gloves
	prop [OBJ_ARMOUR] [ARM_GLOVES] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_GLOVES] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_GLOVES] = 20;

	// boots
	prop [OBJ_ARMOUR] [ARM_BOOTS] [PARM_AC] = 1; // AC
	prop [OBJ_ARMOUR] [ARM_BOOTS] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_BOOTS] = 40;

	// buckler:
	prop [OBJ_ARMOUR] [ARM_BUCKLER] [PARM_AC] = 0; // AC
	prop [OBJ_ARMOUR] [ARM_BUCKLER] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_BUCKLER] = 50;

	// large shield:
	prop [OBJ_ARMOUR] [ARM_LARGE_SHIELD] [PARM_AC] = 0; // AC
	prop [OBJ_ARMOUR] [ARM_LARGE_SHIELD] [PARM_EVASION] = 0; // evasion
	mss [OBJ_ARMOUR] [ARM_LARGE_SHIELD] = 250;


int i = 0;

for (i = 0; i < 50; i ++)
{
	mss [OBJ_WANDS] [i] = 100;
	mss [OBJ_FOOD] [i] = 100;
	mss [5] [i] = 200;
	mss [OBJ_SCROLLS] [i] = 50;
	mss [OBJ_JEWELLERY] [i] = 20;
	mss [OBJ_POTIONS] [i] = 60;
	mss [9] [i] = 5;
   // don't know what these are, yet:
   mss [OBJ_BOOKS] [i] = 100; // books
   mss [OBJ_STAVES] [i] = 130; // staves
   mss [OBJ_ORBS] [i] = 300; // the Orb!
   mss [OBJ_MISCELLANY] [i] = 100;
   mss [OBJ_CORPSES] [i] = 100;
   
         


}

// this is food, right?
mss [OBJ_FOOD] [FOOD_MEAT_RATION] = 80;
mss [OBJ_FOOD] [FOOD_BREAD_RATION] = 80;
mss [OBJ_FOOD] [FOOD_PEAR] = 20;
mss [OBJ_FOOD] [FOOD_APPLE] = 20;
mss [OBJ_FOOD] [FOOD_CHOKO] = 30;
mss [OBJ_FOOD] [FOOD_HONEYCOMB] = 40;
mss [OBJ_FOOD] [FOOD_ROYAL_JELLY] = 55;
mss [OBJ_FOOD] [FOOD_SNOZZCUMBER] = 50;
mss [OBJ_FOOD] [FOOD_PIZZA] = 40;
mss [OBJ_FOOD] [FOOD_APRICOT] = 15;
mss [OBJ_FOOD] [FOOD_ORANGE] = 20;
mss [OBJ_FOOD] [FOOD_BANANA] = 20;
mss [OBJ_FOOD] [FOOD_STRAWBERRY] = 5;
mss [OBJ_FOOD] [FOOD_RAMBUTAN] = 10;
mss [OBJ_FOOD] [FOOD_LEMON] = 20;
mss [OBJ_FOOD] [FOOD_GRAPE] = 5;
mss [OBJ_FOOD] [FOOD_SULTANA] = 3;
mss [OBJ_FOOD] [FOOD_LYCHEE] = 10;
mss [OBJ_FOOD] [FOOD_BEEF_JERKY] = 20;
mss [OBJ_FOOD] [FOOD_CHEESE] = 40;
mss [OBJ_FOOD] [FOOD_SAUSAGE] = 40;
mss [OBJ_FOOD] [FOOD_CHUNK] = 100; // chunk of flesh
/*mss [4] [21] = 40;
mss [OBJ_FOOD] [22] = 50;
mss [OBJ_FOOD] [23] = 60;
mss [OBJ_FOOD] [24] = 60;
mss [OBJ_FOOD] [25] = 100;*/

mss [OBJ_MISCELLANY] [MISC_BOTTLED_EFREET] = 250; // flask
mss [OBJ_MISCELLANY] [MISC_CRYSTAL_BALL_OF_SEEING] = 200; // crystal ball

/*
	case 0: strcat(glog , "meat ration"); break;
	case 1: strcat(glog , "bread ration"); break;
	case 2: strcat(glog , "pear"); break;
	case 3: strcat(glog , "apple"); break; // make this less common.
	case 4: strcat(glog , "choko"); break;
	case 5: strcat(glog , "honeycomb"); break;
	case 6: strcat(glog , "royal jell"); break; // maybe a joke monster of the same name? - mix something with jelly crystals?
	case 7: strcat(glog , "snozzcumber"); break;
 case 8: strcat(glog , "slice of pizza"); break;
 case 9: strcat(glog , "apricot"); break;
 case 10: strcat(glog , "orange"); break;
 case 11: strcat(glog , "banana"); break;
 case 12: strcat(glog , "strawberr"); break;
 case 13: strcat(glog , "rambutan"); break;
 case 14: strcat(glog , "lemon"); break;
 case 15: strcat(glog , "grape"); break;
 case 16: strcat(glog , "sultana"); break;
 case 17: strcat(glog , "lychee"); break;
 case 18: strcat(glog , "beef jerk"); break;
 case 19: strcat(glog , "cheese"); break;
 case 20: strcat(glog , "sausage"); break;
*/

	// weapons: blunt weapons are first to help grouping them together
	//  note: AC prop can't be 0 or -ve because of division.
	// 	  If it's 1, makes no difference


	// NOTE: I have *removed* AC bit for weapons - just doesn't work

	// prop [x] [2] is speed

	// club
	prop [OBJ_WEAPONS] [WPN_CLUB] [PWPN_DAMAGE] = 5; // damage
	prop [OBJ_WEAPONS] [WPN_CLUB] [PWPN_HIT] = 4; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_CLUB] [PWPN_SPEED] = 12; // speed
	mss [OBJ_WEAPONS] [WPN_CLUB] = 50;

	// mace:
	prop [OBJ_WEAPONS] [WPN_MACE] [PWPN_DAMAGE] = 7; // damage
	prop [OBJ_WEAPONS] [WPN_MACE] [PWPN_HIT] = 3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_MACE] [PWPN_SPEED] = 13; // speed
	mss [OBJ_WEAPONS] [WPN_MACE] = 140;

	// flail
	prop [OBJ_WEAPONS] [WPN_FLAIL] [PWPN_DAMAGE] = 8; // damage
	prop [OBJ_WEAPONS] [WPN_FLAIL] [PWPN_HIT] = 1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_FLAIL] [PWPN_SPEED] = 15; // speed
	mss [OBJ_WEAPONS] [WPN_FLAIL] = 150;

	// dagger
	prop [OBJ_WEAPONS] [WPN_DAGGER] [PWPN_DAMAGE] = 3; // damage
	prop [OBJ_WEAPONS] [WPN_DAGGER] [PWPN_HIT] = 6; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_DAGGER] [PWPN_SPEED] = 11; // speed
	mss [OBJ_WEAPONS] [WPN_DAGGER] = 20;

	// morningstar
	prop [OBJ_WEAPONS] [WPN_MORNINGSTAR] [PWPN_DAMAGE] = 8; // damage
	prop [OBJ_WEAPONS] [WPN_MORNINGSTAR] [PWPN_HIT] = 3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_MORNINGSTAR] [PWPN_SPEED] = 14; // speed
	mss [OBJ_WEAPONS] [WPN_MORNINGSTAR] = 120;

	// short sword
	prop [OBJ_WEAPONS] [WPN_SHORT_SWORD] [PWPN_DAMAGE] = 6; // damage
	prop [OBJ_WEAPONS] [WPN_SHORT_SWORD] [PWPN_HIT] = 5; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_SHORT_SWORD] [PWPN_SPEED] = 12; // speed
	mss [OBJ_WEAPONS] [WPN_SHORT_SWORD] = 100;

	// long sword
	prop [OBJ_WEAPONS] [WPN_LONG_SWORD] [PWPN_DAMAGE] = 10; // damage
	prop [OBJ_WEAPONS] [WPN_LONG_SWORD] [PWPN_HIT] = 3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_LONG_SWORD] [PWPN_SPEED] = 14; // speed
	mss [OBJ_WEAPONS] [WPN_LONG_SWORD] = 160;

	// great sword
	prop [OBJ_WEAPONS] [WPN_GREAT_SWORD] [PWPN_DAMAGE] = 16; // damage
	prop [OBJ_WEAPONS] [WPN_GREAT_SWORD] [PWPN_HIT] = -1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_GREAT_SWORD] [PWPN_SPEED] = 17; // speed
	mss [OBJ_WEAPONS] [WPN_GREAT_SWORD] = 250;

	// scimitar
	prop [OBJ_WEAPONS] [WPN_SCIMITAR] [PWPN_DAMAGE] = 11; // damage
	prop [OBJ_WEAPONS] [WPN_SCIMITAR] [PWPN_HIT] = 1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_SCIMITAR] [PWPN_SPEED] = 14; // speed
	mss [OBJ_WEAPONS] [WPN_SCIMITAR] = 170;

	// hand axe
	prop [OBJ_WEAPONS] [WPN_HAND_AXE] [PWPN_DAMAGE] = 7; // damage
	prop [OBJ_WEAPONS] [WPN_HAND_AXE] [PWPN_HIT] = 2; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_HAND_AXE] [PWPN_SPEED] = 13; // speed
	mss [OBJ_WEAPONS] [WPN_HAND_AXE] = 110;

	// battleaxe
	prop [OBJ_WEAPONS] [WPN_BATTLEAXE] [PWPN_DAMAGE] = 17; // damage
	prop [OBJ_WEAPONS] [WPN_BATTLEAXE] [PWPN_HIT] = -2; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_BATTLEAXE] [PWPN_SPEED] = 18; // speed
	mss [OBJ_WEAPONS] [WPN_BATTLEAXE] = 200;

	// spear
	prop [OBJ_WEAPONS] [WPN_SPEAR] [PWPN_DAMAGE] = 5; // damage
	prop [OBJ_WEAPONS] [WPN_SPEAR] [PWPN_HIT] = 3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_SPEAR] [PWPN_SPEED] = 13; // speed
	mss [OBJ_WEAPONS] [WPN_SPEAR] = 50;

	// halberd
	prop [OBJ_WEAPONS] [WPN_HALBERD] [PWPN_DAMAGE] = 13; // damage
	prop [OBJ_WEAPONS] [WPN_HALBERD] [PWPN_HIT] = -3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_HALBERD] [PWPN_SPEED] = 19; // speed
	mss [OBJ_WEAPONS] [WPN_HALBERD] = 200;

	// sling - the three properties are _not_ irrelevant here - when something hits something else (either may be you) in melee, these are used.
	prop [OBJ_WEAPONS] [WPN_SLING] [PWPN_DAMAGE] = 1; // damage
	prop [OBJ_WEAPONS] [WPN_SLING] [PWPN_HIT] = -1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_SLING] [PWPN_SPEED] = 11; // speed
	mss [OBJ_WEAPONS] [WPN_SLING] = 10;

	// bow
	prop [OBJ_WEAPONS] [WPN_BOW] [PWPN_DAMAGE] = 2; // damage
	prop [OBJ_WEAPONS] [WPN_BOW] [PWPN_HIT] = -3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_BOW] [PWPN_SPEED] = 11; // speed
	mss [OBJ_WEAPONS] [WPN_BOW] = 100;

	// crossbow
	prop [OBJ_WEAPONS] [WPN_CROSSBOW] [PWPN_DAMAGE] = 2; // damage
	prop [OBJ_WEAPONS] [WPN_CROSSBOW] [PWPN_HIT] = -1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_CROSSBOW] [PWPN_SPEED] = 15; // speed
	mss [OBJ_WEAPONS] [WPN_CROSSBOW] = 250;

	// hand crossbow
	prop [OBJ_WEAPONS] [WPN_HAND_CROSSBOW] [PWPN_DAMAGE] = 1; // damage
	prop [OBJ_WEAPONS] [WPN_HAND_CROSSBOW] [PWPN_HIT] = -1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_HAND_CROSSBOW] [PWPN_SPEED] = 15; // speed
	mss [OBJ_WEAPONS] [WPN_HAND_CROSSBOW] = 70;

	// glaive
	prop [OBJ_WEAPONS] [WPN_GLAIVE] [PWPN_DAMAGE] = 15; // damage
	prop [OBJ_WEAPONS] [WPN_GLAIVE] [PWPN_HIT] = -3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_GLAIVE] [PWPN_SPEED] = 18; // speed
	mss [OBJ_WEAPONS] [WPN_GLAIVE] = 200;

	// staff - hmmm
	prop [OBJ_WEAPONS] [WPN_QUARTERSTAFF] [PWPN_DAMAGE] = 7; // damage
	prop [OBJ_WEAPONS] [WPN_QUARTERSTAFF] [PWPN_HIT] = 6; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_QUARTERSTAFF] [PWPN_SPEED] = 12; // speed
	mss [OBJ_WEAPONS] [WPN_QUARTERSTAFF] = 130;

	// scythe
	prop [OBJ_WEAPONS] [WPN_SCYTHE] [PWPN_DAMAGE] = 14; // damage
	prop [OBJ_WEAPONS] [WPN_SCYTHE] [PWPN_HIT] = -4; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_SCYTHE] [PWPN_SPEED] = 22; // speed
	mss [OBJ_WEAPONS] [WPN_SCYTHE] = 230;


	// giant club
	prop [OBJ_WEAPONS] [WPN_GIANT_CLUB] [PWPN_DAMAGE] = 15; // damage
	prop [OBJ_WEAPONS] [WPN_GIANT_CLUB] [PWPN_HIT] = -5; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_GIANT_CLUB] [PWPN_SPEED] = 16; // speed
	mss [OBJ_WEAPONS] [WPN_GIANT_CLUB] = 750;

	// giant spiked club
	prop [OBJ_WEAPONS] [WPN_GIANT_SPIKED_CLUB] [PWPN_DAMAGE] = 18; // damage
	prop [OBJ_WEAPONS] [WPN_GIANT_SPIKED_CLUB] [PWPN_HIT] = -6; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_GIANT_SPIKED_CLUB] [PWPN_SPEED] = 17; // speed
	mss [OBJ_WEAPONS] [WPN_GIANT_SPIKED_CLUB] = 850;

	// these two ^^^ should have the same speed because of 2-h ogres.


	// eveningstar
	prop [OBJ_WEAPONS] [WPN_EVENINGSTAR] [PWPN_DAMAGE] = 12; // damage
	prop [OBJ_WEAPONS] [WPN_EVENINGSTAR] [PWPN_HIT] = 2; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_EVENINGSTAR] [PWPN_SPEED] = 15; // speed
	mss [OBJ_WEAPONS] [WPN_EVENINGSTAR] = 150;

	// quick blade
	prop [OBJ_WEAPONS] [WPN_QUICK_BLADE] [PWPN_DAMAGE] = 5; // damage
	prop [OBJ_WEAPONS] [WPN_QUICK_BLADE] [PWPN_HIT] = 6; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_QUICK_BLADE] [PWPN_SPEED] = 7; // speed
	mss [OBJ_WEAPONS] [WPN_QUICK_BLADE] = 100;

	// katana
	prop [OBJ_WEAPONS] [WPN_KATANA] [PWPN_DAMAGE] = 13; // damage
	prop [OBJ_WEAPONS] [WPN_KATANA] [PWPN_HIT] = 4; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_KATANA] [PWPN_SPEED] = 13; // speed
	mss [OBJ_WEAPONS] [WPN_KATANA] = 160;

	// exec axe
	prop [OBJ_WEAPONS] [WPN_EXECUTIONERS_AXE] [PWPN_DAMAGE] = 20; // damage
	prop [OBJ_WEAPONS] [WPN_EXECUTIONERS_AXE] [PWPN_HIT] = -4; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_EXECUTIONERS_AXE] [PWPN_SPEED] = 20; // speed
	mss [OBJ_WEAPONS] [WPN_EXECUTIONERS_AXE] = 320;

	// double sword
	prop [OBJ_WEAPONS] [WPN_DOUBLE_SWORD] [PWPN_DAMAGE] = 15; // damage
	prop [OBJ_WEAPONS] [WPN_DOUBLE_SWORD] [PWPN_HIT] = 3; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_DOUBLE_SWORD] [PWPN_SPEED] = 16; // speed
	mss [OBJ_WEAPONS] [WPN_DOUBLE_SWORD] = 220;

	// triple sword
	prop [OBJ_WEAPONS] [WPN_TRIPLE_SWORD] [PWPN_DAMAGE] = 19; // damage
	prop [OBJ_WEAPONS] [WPN_TRIPLE_SWORD] [PWPN_HIT] = -1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_TRIPLE_SWORD] [PWPN_SPEED] = 19; // speed
	mss [OBJ_WEAPONS] [WPN_TRIPLE_SWORD] = 300;

	// hammer
	prop [OBJ_WEAPONS] [WPN_HAMMER] [PWPN_DAMAGE] = 7; // damage
	prop [OBJ_WEAPONS] [WPN_HAMMER] [PWPN_HIT] = 2; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_HAMMER] [PWPN_SPEED] = 13; // speed
	mss [OBJ_WEAPONS] [WPN_HAMMER] = 130;

	// ancus
	prop [OBJ_WEAPONS] [WPN_ANCUS] [PWPN_DAMAGE] = 8; // damage
	prop [OBJ_WEAPONS] [WPN_ANCUS] [PWPN_HIT] = 2; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_ANCUS] [PWPN_SPEED] = 15; // speed
	mss [OBJ_WEAPONS] [WPN_ANCUS] = 160;

	// whip
	prop [OBJ_WEAPONS] [WPN_WHIP] [PWPN_DAMAGE] = 3; // damage
	prop [OBJ_WEAPONS] [WPN_WHIP] [PWPN_HIT] = 1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_WHIP] [PWPN_SPEED] = 14; // speed
	mss [OBJ_WEAPONS] [WPN_WHIP] = 30;

	// sabre
	prop [OBJ_WEAPONS] [WPN_SABRE] [PWPN_DAMAGE] = 7; // damage
	prop [OBJ_WEAPONS] [WPN_SABRE] [PWPN_HIT] = 4; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_SABRE] [PWPN_SPEED] = 12; // speed
	mss [OBJ_WEAPONS] [WPN_SABRE] = 110;

	// demon blade
	prop [OBJ_WEAPONS] [WPN_DEMON_BLADE] [PWPN_DAMAGE] = 13; // damage
	prop [OBJ_WEAPONS] [WPN_DEMON_BLADE] [PWPN_HIT] = 2; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_DEMON_BLADE] [PWPN_SPEED] = 15; // speed
	mss [OBJ_WEAPONS] [WPN_DEMON_BLADE] = 200;

	// demon whip
	prop [OBJ_WEAPONS] [WPN_DEMON_WHIP] [PWPN_DAMAGE] = 10; // damage
	prop [OBJ_WEAPONS] [WPN_DEMON_WHIP] [PWPN_HIT] = 1; // helps to get past evasion
	prop [OBJ_WEAPONS] [WPN_DEMON_WHIP] [PWPN_SPEED] = 14; // speed
	mss [OBJ_WEAPONS] [WPN_DEMON_WHIP] = 30;


	// Missiles:

	// stone
	prop [OBJ_MISSILES] [MI_STONE] [PWPN_DAMAGE] = 2;
	prop [OBJ_MISSILES] [MI_STONE] [PWPN_HIT] = 4;
	mss [OBJ_MISSILES] [MI_STONE] = 5;

	// arrow
	prop [OBJ_MISSILES] [MI_ARROW] [PWPN_DAMAGE] = 2;
	prop [OBJ_MISSILES] [MI_ARROW] [PWPN_HIT] = 7;
	mss [OBJ_MISSILES] [MI_ARROW] = 10;

	// crossbow bolt
	prop [OBJ_MISSILES] [MI_BOLT] [PWPN_DAMAGE] = 2;
	prop [OBJ_MISSILES] [MI_BOLT] [PWPN_HIT] = 9; // but increases less with skill
	mss [OBJ_MISSILES] [MI_BOLT] = 12;

	// dart
	prop [OBJ_MISSILES] [MI_DART] [PWPN_DAMAGE] = 3;
	prop [OBJ_MISSILES] [MI_DART] [PWPN_HIT] = 5; //whatever - for hand crossbow
	mss [OBJ_MISSILES] [MI_DART] = 5;

	// large rock
	prop [OBJ_MISSILES] [MI_LARGE_ROCK] [PWPN_DAMAGE] = 20;
	mss [OBJ_MISSILES] [MI_LARGE_ROCK] = 1000;

}



int check_item_knowledge(void)
{


   char st_pass [60];

   int i;
   int j;
   char lines = 0;
   int anything = 0;
   char ft = 0;

   char yps = 0;
   strcpy(st_pass, "");

   clrscr();



int inv_count = 0;
int ki = 0;



for (i = 0; i < 4; i++)
{
 for (j = 0; j < 30; j ++)
 {
	if (id [i] [j] != 0)
	{
		inv_count++;
	}
 }
}

if (inv_count == 0)
{
   cprintf("You don't recognise anything yet!");
   if (getch() == 0) getch();
   goto putty;
}
textcolor(BLUE);
cprintf("  You recognise:");
textcolor(LIGHTGREY);
lines++;


for (i = 0; i < 4; i ++)
	{
switch(i)
{
 case 0: ft = OBJ_WANDS; break; // magic devices
 case 1: ft = OBJ_SCROLLS; break; // scrolls
 case 2: ft = OBJ_JEWELLERY; break; // rings
 case 3: ft = OBJ_POTIONS; break; // potions
}

	for (j = 0; j < 30; j++)
	{

		if (lines > 23 && inv_count > 0)
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

		if (id [i] [j] == 1)
		{
			anything ++;

			if (lines > 0) cprintf(EOL);
			lines++;
				cprintf(" ");

   yps = wherey();

			cprintf(item_name_2(0, ft, j, 0, 0, 1, 0).c_str());

         inv_count --;

   if (wherey() != yps) lines++;

		}
	} // end of j loop
}



   if (anything > 0)
   {
	   ki = getch();
	   //ki = getch();
	   //ki = anything;
	   if (ki >= 65 && ki < 123)
	   {
		   return ki;
	   }
	   if (ki == 0) ki = getch();
	   return anything;
   }

   putty :

   return ki;

} // end of check_item_knowledge



int weapon_skill(char wclass, char wtype)
{
if (wclass == 11) return 7; // staff

if (wclass != 0) return 0; // no skill

switch(wtype)
{

	case WPN_CLUB: return SK_MACES_FLAILS; //strcat(glog , "club"); break;
	case WPN_MACE: return SK_MACES_FLAILS; //strcat(glog , "mace"); break;
	case WPN_FLAIL: return SK_MACES_FLAILS; //strcat(glog , "flail"); break;
	case WPN_DAGGER: return SK_SHORT_BLADES; //strcat(glog , "dagger"); break;
	case WPN_MORNINGSTAR: return SK_MACES_FLAILS;//??? //strcat(glog , "spiked mace"); break;
	case WPN_SHORT_SWORD: return SK_SHORT_BLADES; //strcat(glog , "short sword"); break;
	case WPN_LONG_SWORD: return SK_LONG_SWORDS; //strcat(glog , "long sword"); break;
	case WPN_GREAT_SWORD: return SK_GREAT_SWORDS; //strcat(glog , "great sword"); break;
	case WPN_SCIMITAR: return SK_LONG_SWORDS; //strcat(glog , "scimitar"); break;
	case WPN_HAND_AXE: return SK_AXES; //strcat(glog , "hand axe"); break;
	case WPN_BATTLEAXE: return SK_AXES; //strcat(glog , "battleaxe"); break;
	case WPN_SPEAR: return SK_POLEARMS; //strcat(glog , "spear"); break;
	case WPN_HALBERD: return SK_POLEARMS; //strcat(glog , "halberd"); break;
	case WPN_SLING: return SK_SLINGS; //strcat(glog , "sling"); break;
	case WPN_BOW: return SK_BOWS; //strcat(glog , "bow"); break;
	case WPN_CROSSBOW: return SK_CROSSBOWS; //strcat(glog , "crossbow"); break;
 case WPN_HAND_CROSSBOW: return SK_CROSSBOWS; //strcat(glog , "hand crossbow"); break;
 case WPN_GLAIVE: return SK_POLEARMS; //strcat(glog , "glaive"); break;
	case WPN_QUARTERSTAFF: return SK_STAVES; //strcat(glog , "quarterstaff"); break;
 case WPN_SCYTHE: return SK_POLEARMS; //strcat(glog , "scythe"); break;
	case WPN_GIANT_CLUB: return SK_MACES_FLAILS; //strcat(glog , "giant club"); break;
	case WPN_GIANT_SPIKED_CLUB: return SK_MACES_FLAILS; //strcat(glog , "giant spiked club"); break;
	case WPN_EVENINGSTAR: return SK_MACES_FLAILS; // eveningstar
	case WPN_QUICK_BLADE: return SK_SHORT_BLADES; // quick blade
	case WPN_KATANA: return SK_LONG_SWORDS; // katana

	case WPN_EXECUTIONERS_AXE: return SK_AXES; // exec axe
	case WPN_DOUBLE_SWORD: return SK_LONG_SWORDS; // 2x sword
	case WPN_TRIPLE_SWORD: return SK_GREAT_SWORDS; // 3x sword
	case WPN_HAMMER: return SK_MACES_FLAILS; // hammer
	case WPN_ANCUS: return SK_MACES_FLAILS; // ancus
	case WPN_WHIP: return SK_MACES_FLAILS; // whip
	case WPN_SABRE: return SK_SHORT_BLADES; // sabre
	case WPN_DEMON_BLADE: return SK_LONG_SWORDS; // demon blade
	case WPN_DEMON_WHIP: return SK_MACES_FLAILS; // demon whip

}

return 0;

}


char damage_type(char wclass, char wtype)
{

if (wclass != 0) return 0; // bludgeon
// 1 = slicing
// 2 = piercing
// 3 = chopping

switch(wtype)
{

	case WPN_CLUB: return DVORP_CRUSHING; //strcat(glog , "club"); break;
	case WPN_MACE: return DVORP_CRUSHING; //strcat(glog , "mace"); break;
	case WPN_FLAIL: return DVORP_CRUSHING; //strcat(glog , "flail"); break;
	case WPN_DAGGER: return DVORP_SLICING; //strcat(glog , "dagger"); break;
	case WPN_MORNINGSTAR: return DVORP_PIERCING;//??? //strcat(glog , "spiked mace"); break;
	case WPN_SHORT_SWORD: return DVORP_SLICING; //strcat(glog , "short sword"); break;
	case WPN_LONG_SWORD: return DVORP_SLICING; //strcat(glog , "long sword"); break;
	case WPN_GREAT_SWORD: return DVORP_SLICING; //strcat(glog , "great sword"); break;
	case WPN_SCIMITAR: return DVORP_SLICING; //strcat(glog , "scimitar"); break;
	case WPN_HAND_AXE: return DVORP_CHOPPING; //strcat(glog , "hand axe"); break;
	case WPN_BATTLEAXE: return DVORP_CHOPPING; //strcat(glog , "battleaxe"); break;
	case WPN_SPEAR: return DVORP_PIERCING; //strcat(glog , "spear"); break;
	case WPN_HALBERD: return DVORP_CHOPPING; //strcat(glog , "halberd"); break;
	case WPN_SLING: return DVORP_CRUSHING; //strcat(glog , "sling"); break;
	case WPN_BOW: return DVORP_CRUSHING; //strcat(glog , "bow"); break;
	case WPN_CROSSBOW: return DVORP_CRUSHING; //strcat(glog , "crossbow"); break;
 case WPN_HAND_CROSSBOW: return DVORP_CRUSHING; //strcat(glog , "hand crossbow"); break;
 case WPN_GLAIVE: return DVORP_CHOPPING; //strcat(glog , "glaive"); break;
	case WPN_QUARTERSTAFF: return DVORP_CRUSHING; //strcat(glog , "quarterstaff"); break;
 case WPN_SCYTHE: return DVORP_SLICING; //strcat(glog , "scythe"); break;
	case WPN_GIANT_CLUB: return DVORP_CRUSHING; //strcat(glog , "giant club"); break;
	case WPN_GIANT_SPIKED_CLUB: return DVORP_PIERCING; //strcat(glog , "giant spiked club"); break;
	case WPN_EVENINGSTAR: return DVORP_PIERCING; // eveningstar
	case WPN_QUICK_BLADE: return DVORP_SLICING; // quick blade
	case WPN_KATANA: return DVORP_SLICING; // katana
	case WPN_EXECUTIONERS_AXE: return DVORP_CHOPPING; // exec axe
	case WPN_DOUBLE_SWORD: return DVORP_SLICING; // 2x sword
	case WPN_TRIPLE_SWORD: return DVORP_SLICING; // 3x sword
	case WPN_HAMMER: return DVORP_CRUSHING; // hammer
	case WPN_ANCUS: return DVORP_CRUSHING; // ancus
	case WPN_WHIP: return DVORP_CRUSHING; // whip
	case WPN_SABRE: return DVORP_SLICING; // sabre
	case WPN_DEMON_BLADE: return DVORP_SLICING; // demon blade
	case WPN_DEMON_WHIP: return DVORP_CRUSHING; // demon whip


}

return 0;

} // end damage_type


std::string make_name(int var1, int var2, int var3, char ncase)
{
	char name [30] = "";
	int numb [15];
	char len;
	int i = 0;
	char nexty = 0;
	int j = 0;
	char igo = 0;

	int ix = 0;


	char glag [30];

	strcpy(glag, "");


	numb [0] = var1 * var2;
	numb [1] = var1 * var3;
	numb [2] = var2 * var3;
	numb [3] = var1 * var2 * var3;
	numb [4] = var1 + var2;
	numb [5] = var2 + var3;
	numb [6] = var1 * var2 + var3;
	numb [7] = var1 * var3 + var2;
	numb [8] = var2 * var3 + var1 ;
	numb [9] = var1 * var2 * var3 - var1;
	numb [10] = var1 + var2 + var2;
	numb [11] = var2 + var3 * var1;
	numb [12] = var1 * var2 * var3;
	numb [13] = var1 * var3 * var1;
	numb [14] = var2 * var3 * var3;


	for (i = 0; i < 15; i ++)
	{
		while(numb [i] >= 25)
		{
			numb [i] -= 25;
		}
	} // end of for i

	j = numb [6];

	len = reduce(numb [reduce(numb [11]) / 2]);
	while (len < 5 && j < 10)
	{
		len += reduce(numb [j] + 1) + 1;
		j++;
	}

	while (len > 14)
	{
		len -= 8;
	}

	nexty = retbit(numb [4]);
	char k = 0;
	j = 0;

	for (i = 0; i < len; i ++)
	{
		j ++;
		if (j >= 15)
		{
			j = 0;
			k++;
			if (k > 9) break;
		}

		if (nexty == 1 || (i > 0 && is_a_vowel(name [i]) == 0))
		{
			name [i] = retvow(numb [j]);
			if ((i == 0 || i == len - 1) && name [i] == 32)
			{
				i--;
				continue;
			}
		} else
		{
			if (numb [i / 2] <= 1 && i > 3 && is_a_vowel(name [i]) == 1) goto two_letter;
			else
				name [i] = numb [j];
hello : igo ++;
		}

		if ((nexty == 0 && is_a_vowel(name [i]) == 1) || (nexty == 1 && is_a_vowel(name [i]) == 0))
		{
			if (nexty == 1 && i > 0 && is_a_vowel(name [i - 1]) == 0) i --;
			i--;
			continue;
		}


		if (is_a_vowel(name [i]) == 0)
		{
			nexty = 1;
		} else nexty = 0;

		ix++;

	}

	switch(ncase)
	{
		case 2:
			for (i = 0; i < len + 1; i ++)
			{
				if (i > 3 && name [i] == 0 && name [i + 1] == 0)
				{
					name [i] = 0;
					if (name [i - 1] == 32) name [i - 1] = 0;
					break;
				}
				if (name [i] != 32 && name [i] < 30)
					name [i] += 65;
				if (name [i] > 96) name [i] -= 32;
			}
			break;

		case 3:
			for (i = 0; i < len + 0; i ++)
			{
				if (i != 0 && name [i] >= 65 && name [i] < 97)
				{
					if (name [i - 1] == 32) name [i] += 32;
				}

				if (name [i] > 97)
				{
					if (i == 0 || name [i - 1] == 32) name [i] -= 32;
				}


				if (name [i] < 30)
				{
					if (i == 0 || (name [i] != 32 && name [i - 1] == 32)) name [i] += 65;
					else name [i] += 97;
				}


			}
			break;

		case 0:
			for (i = 0; i < len; i ++)
			{
				if (name [i] != 32 && name [i] < 30)
					name [i] += 97;
			}
			break;

		case 1:
			name [i] += 65;
			for (i = 1; i < len; i ++)
			{
				if (name [i] != 32 && name [i] < 30)
					name [i] += 97; //97;
			}
			break;
	}

	strcpy(glag, name);

	if (strlen(glag) == 0) strcpy(glag, "Plog");

	return glag;




two_letter :
	if (nexty == 1) goto hello;

	if (!is_a_vowel(name [i - 1])) goto hello;

	i ++;
	switch(i * (retbit(j) + 1))
	{
		case 0: strcat(name, "sh"); break;
		case 1: strcat(name, "ch"); break;
		case 2: strcat(name, "tz"); break;
		case 3: strcat(name, "ts"); break;
		case 4: strcat(name, "cs"); break;
		case 5: strcat(name, "cz"); break;
		case 6: strcat(name, "xt"); break;
		case 7: strcat(name, "xk"); break;
		case 8: strcat(name, "kl"); break;
		case 9: strcat(name, "cl"); break;
		case 10: strcat(name, "fr"); break;
		case 11: strcat(name, "sh"); break;
		case 12: strcat(name, "ch"); break;
		case 13: strcat(name, "gh"); break;
		case 14: strcat(name, "pr"); break;
		case 15: strcat(name, "tr"); break;
		case 16: strcat(name, "mn"); break;
		case 17: strcat(name, "ph"); break;
		case 18: strcat(name, "pn"); break;
		case 19: strcat(name, "cv"); break;
		case 20: strcat(name, "zx"); break;
		case 21: strcat(name, "xz"); break;
		case 23: strcat(name, "dd"); break;
		case 24: strcat(name, "tt"); break;
		case 25: strcat(name, "ll"); break;
				 //case 26: strcat(name, "sh"); break;
				 //case 12: strcat(name, "sh"); break;
				 //case 13: strcat(name, "sh"); break;
		default:
				 i--;
				 goto hello;
	}

	ix += 2;

	goto hello;

} // end of make_name



char reduce(int reducee)
{

while(reducee >= 26)
{
	reducee -= 26;
}

return reducee;

} // end of char reduce



char is_a_vowel(int let)
{
	//if (let == 'a' || let == 'e' || let == 'i' || let == 'o' || let == 'u')
	if (let == 0 || let == 4 || let == 8 || let == 14 || let == 20 || let == 24 || let == 32)
	{
		return 1;
	} else return 0;
} // end of char reduce



char retvow(char sed)
{

while(sed > 6)
{
	sed -= 6;
}

switch(sed)
{
	case 0: return 0;
	case 1: return 4;
	case 2: return 8;
	case 3: return 14;
	case 4: return 20;
	case 5: return 24;
	case 6: return 32;
}

return 0;
}


char retbit(char sed)
{

	if (sed % 2 == 0) return 0;

	return 1;

}



