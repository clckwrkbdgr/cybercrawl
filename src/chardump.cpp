
#include "linuxlib.h"

#include <vector>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#include "externs.h"
#include "enum.h"
#include "player.h"
#include "religion.h"
#include "itemname.h"
#include "message.h"
#include "mutation.h"
#include "skills.h"
#include "skills2.h"
#include "spells0.h"
#include "shopping.h"

std::string stat_to_string(int stat, int max_stat)
{
	if(stat < max_stat) {
		Format format("@1/@2");
		format << stat << max_stat;
		return format.str();
	}
	Format format("@1");
	format << stat;
	return format.str();
}

std::string location_name()
{
	std::string result = "You are ";

	switch(you[0].level_type) {
		case 3: return result += "in Bioengineerings";
		case 2: return result += "in the Dump";
		case 1: return result += "in a labyrinth";
		default: break;
	}

	switch(you[0].where_are_you) {
		case 1: return result + "in Iron works";
		case 2: return result + "in Refuelings";
		case 3: return result + "in the Lobby";
		case 4: return result + "in Cooling plants";
		case 5: return result + "in Cyborg research labs";
		case 6: return result + "in the Administrations";
		case 7: return result + "in the Pit";
		case 10: return result + "in the Terrorist centre";
		case 11: return result + "in the Hive";
		case 12: return result + "in the Biodome";
		case 13: return result + "in the Waste Pits";
		case 14: return result + "in the Storage Area";
		case 15: return result + "in the Cyborg Manufactory";
		case 16: return result + "in the Armory";
		case 17: return result + "in the Hall of Alice";
		case 18: return result + "in the Terminal Hub";
		case 19: return result + "in the Snake Pit";
		case 20: return result + "in the Ninja Palace";
		case 21: return result + "in the Cyborg Hub";
		case 22: return result + "in the Greenhouse";
		default: break;
	}

	if(you[0].your_level == -1) {
		return "You escaped.";
	}

	Format format("on level @1");
	format << (you[0].your_level + 1);
	return result + format.str();
}

std::string god_attitude_string(int piety)
{
	if (piety <= 5) {
		return "displeased.";
	} else if (piety <= 20) {
		return "noncommittal.";
	} else if (piety <= 40) {
		return "pleased with you.";
	} else if (piety <= 70) {
		return "most pleased with you.";
	} else if (piety <= 100) {
		return "greatly pleased with you.";
	} else if (piety <= 130) {
		return "extremely pleased with you.";
	}
	return "exalted by your work.";
}

std::string hunger_state(int hunger)
{
	if (hunger <= 1000) {
		return "You are starving.";
	} else if (hunger <= 2600) {
		return "You are hungry.";
	} else if (hunger < 7000) {
		return "You are not hungry.";
	} else if (hunger < 11000) {
		return "You are full.";
	}
	return "You are completely stuffed.";
}

std::string inventory_category(int category)
{
	switch(category) {
		case 0: return "Hand weapons"; break;
		case 1: return "Missiles"; break;
		case 2: return "Armour"; break;
		case 3: return "Technological devices"; break;
		case 4: return "Comestibles"; break;
		case 5: return "Program archives"; break;
		case 6: return "Single-use devices"; break;
		case 7: return "Brain implants"; break;
		case 8: return "Vials"; break;
		case 9: return "Gems"; break;
		case 10: return "Program archives"; break;
		case 11: return "ROM units"; break;
		case 12: return "Discs of Power"; break;
		case 13: return "Miscellaneous"; break;
		case 14: return "Carrion"; break;
	}
	return "";
}

int dump_char(int show_prices, const std::string & fname)
{
	std::vector<std::string> dmp;
	Format format("");

	dmp.push_back(" Cybercrawl version "VERSION" character file.");
	dmp.push_back("");

	format = Format("@1 the @2 (@3)");
	format << you[0].your_name << skill_title(best_skill(0, 50, 99), you[0].skills [best_skill(0, 50, 99)], you[0].clas, you[0].xl) << species_name(you[0].species);
	dmp.push_back(format.str());

	format = Format("(Level @1 @2)");
	format << you[0].xl << you[0].clasnam;
	dmp.push_back(format.str());

	format = Format("Experience : @1/@2");
	format << you[0].xl << you[0].xp;
	dmp.push_back(format.str());

	format = Format("Strength @1     Dexterity @2     Intelligence @3");
	format << stat_to_string(you[0].strength, you[0].max_strength);
	format << stat_to_string(you[0].dex, you[0].max_dex);
	format << stat_to_string(you[0].intel, you[0].max_intel);
	dmp.push_back(format.str());

	std::string dead;
	if (you[0].hp <= 0) {
		dead = (you[0].deaths_door == 0) ? " (dead)" : " (almost dead)";
	}

	format = Format("Hit Points : @1@2         Energy Points : @3");
	format << stat_to_string(you[0].hp, you[0].hp_max) << dead << stat_to_string(you[0].ep, you[0].ep_max);
	dmp.push_back(format.str());

	format = Format("AC : @1          Evasion : @2          Shield : @3");
	format << player_AC() << player_evasion() << player_shield_class();
	dmp.push_back(format.str());

	format = Format("Gp : @1");
	format << you[0].gp;
	dmp.push_back(format.str());

	dmp.push_back("");

	dmp.push_back(location_name());

	if (you[0].religion != GOD_NO_GOD)
	{
		format = Format("You work for @1.");
		format << god_name(you[0].religion);
		dmp.push_back(format.str());

		if (you[0].religion != GOD_XOM) /* Xom doesn't care */
		{
			format = Format("@1 is @2");
			format << god_name(you[0].religion) << god_attitude_string(you[0].piety);
			dmp.push_back(format.str());
		}
	}

	switch(you[0].burden_state) {
		case 5: dmp.push_back("You are overloaded with stuff."); break;
		case 2: dmp.push_back("You are encumbered."); break;
	}

	dmp.push_back(hunger_state(you[0].hunger));
	dmp.push_back("");

	switch(you[0].attribute [ATTR_TRANSFORMATION]) {
		case 1: dmp.push_back("You are in spider-form."); dmp.push_back(""); break;
		case 2: dmp.push_back("Your hands are blades."); dmp.push_back(""); break;
		case 3: dmp.push_back("You are a stone statue."); dmp.push_back(""); break;
	}

	int temp_id [4] [50];

	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 50; j ++) {
			temp_id [i] [j] = 1;
		}
	}
	int Inv_class2 [15];
	int inv_count = 0;

	for	(int i = 0; i < 15; i++) {
		Inv_class2 [i] = 0;
	}

	for (int i = 0; i < 52; i++) {
		if (you[0].inv_quant [i] != 0) {
			Inv_class2 [you[0].inv_class [i]] ++; // adds up number of each class in invent.
			inv_count++;
		}
	}

	if (inv_count == 0) {
		dmp.push_back("You aren't carrying anything.");
		goto finished_invent;
	}

	dmp.push_back("  Inventory:");

	for (int i = 0; i < 15; i ++) {
		if (Inv_class2 [i] != 0) {
			dmp.push_back(inventory_category(i));

			for (int j = 0; j < 52; j++) {
				if (you[0].inv_class [j] == i && you[0].inv_quant [j] > 0) {
					format = Format(" @1 - @2@3");
					format << char((j < 26) ? (j + 97) : (j  + 65 - 26));
					format << in_name(j, 3);
					inv_count --;

					std::string position;
					if (j == you[0].equip [EQ_WEAPON]) position = " (weapon)";
					if (j == you[0].equip [EQ_BODY_ARMOUR] || j == you[0].equip [EQ_CLOAK] || j == you[0].equip [EQ_HELMET] || j == you[0].equip [EQ_GLOVES] || j == you[0].equip [EQ_BOOTS] || j == you[0].equip [EQ_SHIELD]) position = " (worn)";
					if (j == you[0].equip [EQ_LEFT_RING]) position = " (left hemisphere)";
					if (j == you[0].equip [EQ_RIGHT_RING]) position = " (right hemisphere)";
					if (j == you[0].equip [EQ_AMULET]) position = " (vertebra)";
					format << position;

					std::string result = format.str();
					if (show_prices == 1) {
						Format price_format(" (@1gp)");
						price_format << item_value(you[0].inv_class [j], you[0].inv_type [j], you[0].inv_dam [j], you[0].inv_plus [j], you[0].inv_plus2 [j], you[0].inv_quant [j], 3, temp_id);
						result += price_format.str();
					}
					dmp.push_back(result);
				}
			}
		}
	}

finished_invent:

	dmp.push_back("");
	dmp.push_back("");

	format = Format(" You have @1 experience left.");
	format << you[0].exp_available;
	dmp.push_back(format.str());

	dmp.push_back("   Skills:");

	for (int i = 0; i < 50; i ++)
	{
		if (you[0].skills [i] > 0)
		{
			format = Format(" @1 Level @2 @3");
			format << ((you[0].practise_skill [i] == 0) ? "-" : "+");
			format << you[0].skills [i];
			format << skill_name(i);
			dmp.push_back(format.str());
		}
	}

	dmp.push_back("");
	dmp.push_back("");


	if (you[0].spell_levels == 1) {
		dmp.push_back("You have one program slot left.");
	} else if (you[0].spell_levels == 0) {
		dmp.push_back("You cannot install any programs.");
	} else {
		format = Format("You have @1 program slots left.");
		format << you[0].spell_levels;
		dmp.push_back(format.str());
	}

	if (you[0].spell_no == 0)
	{
		dmp.push_back("You don't have any program installed.");
		goto finished_spells;
	}

	dmp.push_back("You have the following programs:");

	for (int j = 0; j < 25; j ++)
	{
		if (you[0].spells [j] != 210)
		{

			format = Format(" @1 - @2");
			format << char((j < 26) ? (j + 97) : (j  + 65));
			format << spell_name(you[0].spells [j]);
			dmp.push_back(format.str());

		}
	}

finished_spells:

	/* Can't use how_mutated() here, as it doesn't count demonic powers */

	int xy;
	int xz = 0;

	for (xy = 0; xy < 100; xy ++)
	{
		if (you[0].mutation [xy] > 0) xz ++;
	}


	if (xz > 0)
	{

		dmp.push_back("");
		dmp.push_back("           Mutations & Other Weirdness");

		for (int j = 0; j < 100; j ++)
		{
			if (you[0].mutation [j] == 0) continue;
			if(you[0].demon_pow [j] > 0) {
				dmp.push_back(std::string("* ") + mutation_name(j));
			} else {
				dmp.push_back(mutation_name(j));
			}
		}

	}


	std::string file_name = fname;
	if(file_name != "morgue.txt") {
		file_name += ".txt";
	}

	int handle = open(file_name.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0660);

	msg("File name: @1") << file_name;

	if (handle == -1)
	{
		mpr("Error opening file.");
		return -1;
	}

	int str_length = 0;

	for (size_t i = 0; i < dmp.size(); i ++)
	{
		dmp[i] += "\n";
		str_length = dmp [i].size();
		if (str_length > 79) str_length = 79;
		write(handle, std::string(dmp[i], 0, str_length).data(), str_length);
	}

	close(handle);

	return 1;

}
