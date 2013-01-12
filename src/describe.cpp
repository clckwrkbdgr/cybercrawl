#include <stdlib.h>
#include <string.h>
#include "enum.h"
#include "externs.h"
#include "itemname.h"
#include "linuxlib.h"
#include "macro.h"
#include "mstruct.h"
#include "player.h"
#include "randart.h"
#include "skills2.h"
#include "stuff.h"

struct ItemInfo {
	int item_class;
	int type;
	int plus;
	int plus2;
	int dam;
	int id;
};

void print_description(const std::string & description);
std::string print_ench(int item_plus);
std::string randart_descpr(ItemInfo item);
std::string describe_demon();

std::string get_ghost_status(int status)
{
	if(status < 4) {
		return "a weakling";
	} else if(status < 7) {
		return "an average";
	} else if(status < 11) {
		return "an experienced";
	} else if(status < 16) {
		return "a powerful";
	} else if(status < 22) {
		return "a mighty";
	} else if(status < 26) {
		return "a great";
	} else if(status < 27) {
		return "an awesomely powerful";
	}
	return "a legendary";
}

std::string get_ghost_name()
{
	Format format("The ghost of @1, @2 @3 @4.");
	format << ghost.gname << get_ghost_status(ghost.ghs[12]) << species_name(ghost.ghs [9]) << skill_title(ghost.ghs [10], ghost.ghs [11], you[0].clas, ghost.ghs [12]);
	return format.str();
}

std::string monster_description(int monster_class)
{
	switch(monster_class) {
		case 0: return "A black ant with poisonous pincers, about the size of a large dog.";
		case 1: return "A huge black bat.";
		case 2: return "A hybrid with the torso of a human atop the mechanical legs.";
		case 3: return "The Red Guard is slightly shorter than a human, but muscular and covered in spikes and horns. Two short wings sprout from its shoulders.";
		case 5: return "A lumpy grey fungus, growing well in the dank underground dungeon.";
		case 6: return "A race of short, ugly humanoids.";
		case 7: return "A fearsome hunting dog.";
		case 8: return "An ugly little minor drone.";
		case 9: return "A canine scavenger. Packs of these creatures roam the underworld, searching for carrion to devour.";
		case 10: return "A giant bee, bearing a deadly sting which can sting repeatedly.";
		case 12: return "A big fully armed cyber tank.";
		case 13: return "A vacuum cleaner robot with corrupter cyberbrain. It uses its tools as weapons and charges on dead corpses.";
		case 14: return "An ugly subterranean race, terrorists combine the worst features of humans, pigs, and several other unpleasant creatures put together.";
		case 15: return "A certain type of visual projection.";
		case 17: return "Rats who have grown large and aggressive in the pestilential dungeon environment.";
		case 18: return "A giant black scorpion, its body covered in thick armour plating, and its tail tipped by a nasty venomous sting.";
		case 56:
		case 19: return "A gargantuan worm, its huge maw capable of crushing rock into dust with little trouble.";
		case 20: return "An ugly thing. Yuck.";
		case 21: return "A swirling cloud of flame.";
		case 22: return "A giant worm, with unusually large teeth (for a worm).";
		case 23: return "A hideous form, created or summoned by some arcane process.";
		case 24: return "A giant wasp covered with thick plates of yellow chitinous armour.";
		case 25: return "A corpse raised from the dead by cyborgization.";
		case 27: return "A huge black beetle with great crunching mandibles and an armour-plated body.";
		case 28: return "A giant with one eye in the centre of its forehead. Despite their lack of binocular vision, cyclopses throw boulders with fearsomely accuracy.";
		case 29: return "A great reptilian beast, covered in thick green scales and with two huge bat-like wings. Little trails of smoke spill from its toothy maw.";
		case 30: return "A huge ogre with two heads on top of a bloated ogre body. It is capable of holding a weapon in each giant hand.";
		case 31: return "One of the most fearsome denizens of Facilities. A huge and powerful mutant wreathed in hellfire, with great scaly wings.";
		case 32: return "A volatile floating ball of spores, covered in knobbly rhizome growths.";
		case 33: return "Larger relatives of bums.";
		case 34: return "A terrible creature, formed of snow and crystalline ice. Its feet leave puddles of icy water on the floor.";
		case 35: return "A pulsating mass of acidic protoplasm. It can and will eat almost anything, and grows a little each time...";
		case 36: return "Smugglers are small bum-like creatures.";
		case 37: return "A hacker who didn't want to die, a Lich is a skeleton or decaying corpse kept alive by a mighty exercise of cyborgization. These cyber creatures can wield great hacking abilities and are best avoided by all but the most confident (or stupid) adventurers.";
		case 38: return "A cyborg figure covered in bandages and embalming fluids, compelled to walk by an ancient virus. It radiates a malign aura to those who intrude on its domain.";
		case 39: return "A quick half-serpent, half-human mutant.";
		case 40: return "A larger, uglier and usually fatter relative of terrorists and bums.";
		case 41: return "Few plants can grow in the unpleasant dungeon environment, but some have managed to adapt and even thrive underground in the absence of the sun.";
		case 42: return "Even larger and more dangerous-looking than its offspring, this creature wants you out of its hive. Now!";
		case 78:
		case 43: return "A type of robot who comes to the material world in search of power and knowledge. They are experts in the art of illusion, among other things.";
		case 44: return "The common dungeon snake.";
		case 45: return "A huge, nasty-looking creature. Its thick and knobbly hide seems to heal almost instantly from most wounds.";
		case 46: return "These creatures are usually unseen by the eyes of most, and those few who have seen them would rather not have.";
		case 47: return "A powerful chemist. It wants to use your blood and other body fluids! ";
		case 48: return "This robots appears as a cloud of black mist surrounding an insubstantial skeletal form. Its eyes burn bright with unholy malevolence.";
		case 49: return "A huge and hideous creature, created or summoned by some arcane process.";
		case 50: return "The common dungeon yak. Covered in shaggy yak hair and bearing a nasty pair of yak horns.";
		case 51: return "A corpse raised from the dead by zombification.";
		case 52: return "An armoured terrorist, obviously experienced in the ways of hacking other creatures apart.";
		case 53: return "A smuggler who has learned to summon and direct mutants.";
		case 54: return "While terrorists are generally quite stupid, occasionally one of them develops an aptitude for hacking.";
		case 55: return "A heavily armoured terrorist, covered in scars from many past battles.";
		case 57: return "Although smaller and less formidable than true planes, roboplanes are nonetheless a foe to be reckoned with.";
		case 58: return "A smuggler of unusual size.";
		case 59: return "A giant camera, with a captivating stare.";
		case 60: return "An ancient warrior, kept in a state of cyborg by its will to live.";
		case 61: return "A vicious plant, dripping with vitriol.";
		case 62: return "A large hairy spider with vicious mandibles, roaming the dungeon in search of food.";
		case 63: return "A walking cyborg shadow.";
		case 64: return "The mailfunctioning medbot. Once used to heal people now they seek to damage them.";
		case 65: return "These hovering cameras are especially loathed by hackers.";
		case 66: return "A large multicoloured butterfly with beautifully patterned wings.";
		case 67: return "A large, fat mushroom.";
		case 68: return "A huge and muscular figure engulfed in a cloud of searing flame.";
		case 69: return "A slimy mauve worm with a greatly distended head.";
		case 70: return "A huge wrinkled brain, floating just off the floor. Every now and then it seems to pulsate.";
		case 71: return "A huge beetle with an almost impenetrable rocky carapace.";
		case 72: return "Cybernetics keeps a disembodied cyber turrent hovering above the floor. It has a nasty set of blades.";
		case 73: return "A great black dog, with smoke pouring from its fanged mouth.";
		case 74: return "A large human with the head of a bull. It makes its home in secluded labyrinths.";
		case 75: return "Like a normal dragon, only white and covered in frost.";
		case 76: return "An icky glob of slime, which slithers along the ground.";
		case 77: return "A cloud of freezing air surrounding an incorporeal skeletal form.";
		case 79: return "A levitating ball, covered in malignant camera lenses.";
		case 80: return "A frightful mutant, covered in roaring hellfire.";
		case 81: return "A hideous decaying form.";
		case 82: return "This cruel and malicious robot is covered in all manner of claws, spines and hooks.";
		case 83: return "A cyber form wielding a giant scythe. It has come for your soul!";
		case 84: return "This greater mutant looks like a shadow gliding through the air towards you. It radiates an intense aura of negative power.";
		case 85: return "A mutant covered in hair. Watch out - it may have fleas!";
		case 86: return "A mutant covered in glittering ice.";
		case 87: return "A nasty blue thing. It looks cold.";
		case 88: return "A weird and hideous cross between beast and human.";
		case 89: return "A hideous humanoid figure with metal skin.";
		case 98: return "A shapeshifter who has lost control over his, her or its transformations, and is constantly changing form.";
		case 99: return "A weird creature with the power to change its form. It is very rarely observed in its natural state.";
		case 100: return "A large arachnid with vicious poisoned mouth-parts.";
		case 101: return "A relatively small grey dragon, with steam pouring from its mouth.";
		case 102: return "A very ugly thing. Double yuck.";
		case 103: return "An terrorist hacker who uses destructive hacks.";
		case 104: return "A flying large tank.";
		case 105: return "A giant flying tank..";
		case 106: return "A great reptilian beast, distantly related to the dragon. It has many heads, and the potential to grow many more!";
		case 107:
		case 108: return "A skeleton compelled to unlife by the exercise of cyborgization.";
		case 109: return "A heavily armoured warrior, in league with the powers of Facilities.";
		case 110: return "A hacker who specialises in the practice of cyborgization programming.";
		case 111: return "An evil hacker.";
		case 112: return "A servant of the employers of the terrorists.";
		case 113: return "An exalted servant of the employers of the terrorists";
		case 114: return "";
		case 115: return "A large relative of bums and terrorists.";
		case 116: return "A huge animated clay statue.";
		case 117: return "An animated wooden statue.";
		case 118: return "A huge animated stone statue.";
		case 119: return "A huge animated metal statue.";
		case 120: return "A huge animated crystal statue.";
		case 121: return "A huge animated statue made entirely from toenail clippings. Some people just have too much time on their hands.";
		case 122: return "A small dragon with strangely mottled scales.";
		case 123: return "A spirit drawn from the elemental plane of earth, which exists in this world by inhabiting a lump of earth and rocks.";
		case 124: return "A spirit drawn from the elemental plane of fire, which exists in this world as a brilliant column of raging flames.";
		case 125: return "A spirit drawn from the elemental plane of air. It exists in this world as a swirling vortex of air, which is often undetectable without magical means.";
		case 126: return "One of the most terrible denizens of the many Hells, the Ice Fiend is a huge icy figure, covered in frost and wreathed in freezing air.";
		case 127: return "One of the most terrible denizens of the many Hells, this horrible being appears as a great mass of writhing shadows which occasionally reveal the skeletal form of a huge demon.";
		case 128: return "A large brown snake.";
		case 129: return "A huge lizard, with great crunching jaws.";
		case 130: return "A hideous translucent green cyber hologram.";
		case 131: return "A revolting glob of writhing flesh.";
		case 132: return "A huge and very powerful dragon. Sparks crackle along its enormous scaly wings.";
		case 133: return "Like a ranger, but half yak.";
		case 134: return "A larger and beefier relative of the common dungeon yak. Its little red eyes gleam with hunger for living flesh.";
		case 135: return "An enormous and very nasty-looking creature. Its rocky hide seems to heal almost instantaneously from most wounds.";
		case 136: return "A gigantic humanoid figure, with grey skin almost as hard as rock. It carries a supply of boulders, and wants to play catch with you.";
		case 137: return "A hideous cybernitized creature, with torn skin hanging from an emaciated body.";
		case 138: return "A very large and fat hairy bee.";
		case 139: return "A vicious black spider with a splash of red on its swollen abdomen. Its mandibles drip with lethal poison.";
		case 140: return "A thin wisp of floating gas.";
		case 141: return "A normally invisible cloud of weird-looking vapour.";
		case 142: return "An rare mutant, skilled in the use of hacking.";
		case 143: return "A great black worm, its many-segmented body covered in spikey plates of chitinous armour. Acidic venom drips from its toothy maw.";
		case 144: return "A weapon dancing in the air. ";
		case 145: return "A huge and very powerful giant. ";
		case 146: return "A great dragon covered in shining golden scales. ";
		case 148: return "A very large and very fat lizard. ";
		case 149: return "A huge grey slug with folds of wrinkled skin. ";
		case 150: return "A vicious dog, trained to kill. ";
		case 151: return "A very large grey rat. ";
		case 152: return "A very large rat, with hair and skin of a most peculiar green colour. ";
		case 153: return "A huge rat, with weird knobbly orange skin. It glows with unholy energies. ";
		case 154: return "A large black snake. ";
		case 155: return "A stupid woolly animal, with murder in its eyes. ";
		case 156: return "An cyber creature created by a similar virus to that used in the production of the vacuum cleaner. ";
		case 157: return "A large, fat and very ugly pig. ";
		case 158: return "A huge, bloated mosquito. It looks diseased.";
		case 159: return "It has a lot of legs.";
		case 160: return "A great builder covered in armour plates of rusty iron. ";
		case 161: return "A weird hybrid; human from the waist up, it has a snake tail instead of legs.";
		case 162: return "A red giant, breathing clouds of smoke.";
		case 163: return "A frozen giant.";
		case 164: return "A small dragon, puffing clouds of smoke.";
		case 165: return "A great shadowy shape, radiating evil and death.";
		case 166: return "A large yellow tubular reptile.";
		case 167: return "A very large grey python.";
		case 168: return "A stooped builder.";
		case 169: return "A huge and irritating fly.";
		case 170: return "A huge wasp with a viciously barbed stinger.";
		case 171: return "A slimy dragon, covered in swamp muck. Poisonous gasses dribble from its snout.";
		case 172: return "A small and slimy dragon, covered in swamp muck. It smells horrible.";
		case 173: return "A giant ant with large mandibles and a vicious sting.";
		case 174: return "Although one of the smaller giant varieties, the hill giant is still big enough to be dangerous.";
		case 175: return "A bloated insect, covered in thick chitinous armour. Now you know where all those ants keep coming from!";
		case 176: return "A baby ant. Isn't it cute?";
		case 177: return "It probably didn't get this big by eating little insects.";
		case 178: return "A very large and vicious-looking carnivorous frog. Its knobbly brown skin blends in with the rough rock of your surroundings.";
		case 179: return "Although slightly smaller than its cousin, the giant brown frog, the spiny frog makes up for lack of size by being covered in wickedly barbed spines and spurs.";
		case 180: return "A weird-looking frog, constantly blinking in and out of reality.";
		case 240: return "A mist-wreathed skeletal shadow hanging in mid-air, this creature is almost invisible even to your enhanced sight.";
		case 241: return "A pulsating lump of protoplasm. ";
		case 242: return "A huge and disgusting gastropod. ";
		case 243: return "A huge and disgusting shelled gastropod. ";
		case 244: return "A shimmering, twisted coil of space.";
		case 245: return "A huge winged fiend with incredibly tough skin.";
		case 246: return "A large brown beetle with huge, rock-crushing mandibles.";
		case 247: return "A hideous stone statue come to life.";
		case 248: return "A hideous metal statue come to life.";
		case 249: return "A hideous melting stone statue come to life.";
		case 260:
		case 261: return "A weird hybrid; human from the waist up, it has a snake tail instead of legs.";
		case 262: return "A very large and strong terrorist.";
		case 263:
		case 264:
		case 265:
		case 266:
		case 267:
		case 268:
		case 269:
		case 270:
		case 271:
		case 272:
		case 273:
		case 274: return "One of the race of ninja which inhabits this dreary cave. ";
		case 282: return "A twisted bum. ";
		case 284: return "A particularly fat and ugly terrorist. ";
		case 287: return "A beefy mutant. ";
		case 289: return "A terrorist. ";
		case 292: return "A hairy builder. ";
		case 306: return "An ancient and mighty dragon. ";
		case 310: return "An ancient lich, the air around his shrouded form crackling with evil energy. ";
		case 220: return "A small and mischevious minor drone. ";
		case 221: return "A vaguely humanoid blob of putrid white flesh. ";
		case 222: return "A chattering and shrieking minor drone. ";
		case 223: return "An ugly, twisted little minor drone. ";
		case 224: return "A small flying drone. ";
		case 225: return "A weirdly shaped drone. ";
		case 226: return "A bright orange drone with a venomous stinger. ";
		case 227: return "A hideous skeletal drone, with wings of ancient withered skin. ";
		case 228: return "A writhing cloud of smoke hanging in the air. ";
		case 229: return "A drone with shiny metallic scales. ";
		case 230: return "A horribly powerful drone. ";
		case 231: return "A bloated form covered in oozing sores and exhaling clouds of lethal poison. ";
		case 232: return "A blue greater drone. ";
		case 233: return "A huge and very powerful drone, wreathed in fire and shadows. ";
		case 234: return "A hideously ugly drone of rage and legendary power. ";
		case 235: return "A long and bloated body, supported by dozens of short legs and topped with an evil-looking head. ";
		case 236: return "A drone figure shining with the light and fury of the sun. ";
		case 237: return "A small and shadowy figure. ";
		case 238: return "A mysterious drone figure, constantly blurring into multiple shadows of iteself.";
		case 239: return "A tall and gaunt figure, draped in long robes which flow as if alive.";
		case 251: return "A weirdly glowing figure, dancing through the twisted air of Bioengineering. ";
		case 252: return "An ancient and strangely serene robot. It regards you coldly from the huge glowing eye in the centre of its forehead. ";
		case 253: return "A violent and wrathful cyborg, Cerebov appears as a giant human covered in shining golden armour and wielding a huge twisted sword. ";
		case 254: return "A shadowy figure clothed in impenetrable darkness. ";
		case 275: return "A viscous liquid, flowing along the floor in search of organic matter to corrode. ";
		case 276: return "A faintly glowing blob of bright blue cytoplasm. ";
		case 277: return "A putrid mass of decaying flesh. ";
		case 278: return "A lump of sickly green flesh, dripping with lethal acid. ";
		case 279: return "A particularly rich and golden gelatinous thing. ";
		case 280:
		case 281:
		case 283:
		case 285:
		case 286:
		case 288:
		case 290:
		case 291:
		case 293:
		case 294:
		case 295:
		case 296:
		case 297:
		case 298:
		case 299:
		case 300:
		case 301:
		case 302:
		case 303:
		case 304:
		case 305:
		case 307:
		case 308:
		case 309: return "A human wandering the installation, trying to keep you away from whatever is concealed in the depths. ";
		case 340: return "A huge and slithery arch-cyborg, guarding the gates of Facilities. ";
		case 341: return "The lord of the Iron Works. ";
		case 342: return "One of the arch-cyborgs who dwell in the depths of Facilities. ";
		case 343: return "A great cyborg who lives in the depths of Cooling plants. ";
		case 344: return "A fearsome arch-fiend who rules the deathly netherworld of Cyborg Labs. ";
		case 356: return "A lich who has grown mighty over countless years.";
		case 360: return "A heavily armed and armoured guardian of the Storage Area. ";
		case 361: return "A charred turret floating in the air and rotating slowly. Mystic symbols carved into its blackened surface indicate its resistance to almost any form of attack. ";
		case 362: return "A deadly warrior whose skills have not diminished even in science. ";
		case 363: return "A powerful hacker whose skills have not diminished even in science. ";
		case 364: return "A huge and strangely deformed camera, pulsating with light. ";
		case 365: return "A huge and glowing purple creature, created by the Disc to defend itself. ";
		case 366: return "A divine agent of the Knights Templar. It manifests as a winged figure obscured by an aura of brilliant golden light. ";
		case 367: return "A hideous glowing apparition.";
		case 368: return "An unusually large and powerful assassin.";
		case 369: return "A huge undead abomination, pieced together from the broken bones of many dragons.";
		case 370: return "A writhing mass of tentacles, all covered in putrid mucous.";
		case 371: return "A large creature with a human head, the body of a lion, and the wings of a huge bird.";
		case 372: return "A shambling cyborg, related to the lowlife.";
		case 373: return "An ancient warrior, embalmed and cursed with the ability to walk even when cybernitized.";
		case 374: return "The embalmed and cyber corpse of an ancient ruler.";
		case 375: return "The embalmed and cyber corpse of an ancient servant of darkness.";
		case 376: return "A hybrid with the torso of a human atop the body of a large machine. It looks aggressive.";
		case 377: return "Like a ranger, but half yak. It looks aggressive.";
		case 378: return "A comical figure full of life and laughter. It looks very happy to see you.$But is there a slightly malicious cast to its features? Is that red facepaint something altogether less pleasant? Join in the fun, and maybe you'll find out!";
		case 379: return "A moving column of metal.";
		case 380: return "\"The dorgi...was a huge grumping machine with a pronounced propensity for violence. It was huge, heavy, brown and bulbous. A hulking thing stubbled with inscrutable protruberances. A monstrous thing which moved upon its victims with a sound like heavy breathing.\"$(Hugh Cook, _The Wazir and the Witch_)";
		case 381: return "An incandescent globe of light, capable of impressive pyrotechnics.";
		case 382: return "A small marsupial mammal, altered by nanotechnological manipulators.";
		case 385: return "A huge camera, encased in a levitating globe of incandescant energy. ";
		case 386: return "A huge moth, as violent as it is hairy.";
		case 387: return "A dreadful undead cob of corn.";
		case 388: return "A disembodied toe, hanging in the air and radiating an intense field of negative energy.";
		case 389: return "An apparently harmless pile of gold coins hides a nasty venomous shapechanging predator.";
		case 390: return "An apparently abandoned weapon, actually a vicious little beast in disguise.";
		case 391: return "An apparently abandoned suit of finely-made armour, actually a vicious little beast in disguise.";
		case 392: return "An ancient parchment covered in arcane runes. Did it just twitch?";
		case 393: return "A delicious looking magical drink. Go on, pick it up!";
		case 394: return "A large, fat and very ugly pig, suckled in the pits of Hell. ";
		case 395: return "A huge dragon, burning with hellfire. ";
		case 396: return "A twisted little sprite-bum. Beware of its hacking tricks!";
		case 397: return "A long and sinuous dragon, seemingly more neck and tail than anything else. Its skin shines like molten mercury, and magical energies arc from its pointed snout.";
		case 398: return "A very heavy and apparently flightless dragon.";
		case 399: return "The vicious and heavily armed skeleton of a humanoid creature, animated by cybernitization.";
		case 400: return get_ghost_name();
		case 401: return describe_demon();
		case MLAVA0: return "A vicious red worm which swims through molten rock.";
		case MLAVA1: return "A fish which lives in radioactive waste.";
		case MLAVA2: return "A burning red snake which rears up from pools of radioactive waste and tries to bite you.";
		case MWATER0: return "A fish of unusual size.";
		case MWATER1: return "This is what happens when you give your pet goldfish too much food!";
		case MWATER2: return "A small and slimy eel, crackling with electrical discharge.";
		case MWATER3: return "A pulsating glob of transparent flesh, waiting just below the surface to sting you with its many tentacles.";
		case MWATER4: return "A spirit drawn from the elemental plane of water. It exists on this world as part of a body of water.";
		case MWATER5: return "A large worm, adept at swimming through the muck of this foul swamp.";
		default: return "A monster whose description is yet to be written.";
	}
	return "";
}

void describe_monsters(int class_described)
{
	clrscr();
	print_description(monster_description(class_described));
	if(getkey() == 0) getkey();
}

std::string describe_ordinary_weapon(int item_type)
{
	switch(item_type) {
		case 0: return "A heavy piece of wood. ";
		case 1: return "A long handle with a heavy lump on one end. ";
		case 2: return "Like a mace, but with a length of chain between the handle and the lump of metal. ";
		case 3: return "A long knife or a very short sword, which can be held or thrown. ";
		case 4: return "A mace covered in spikes. ";
		case 5: return "A sword with a short, slashing blade. ";
		case 6: return "A sword with a long, slashing blade. ";
		case 7: return "A sword with a very long, heavy blade and a long handle, designed to be wielded with two hands. ";
		case 8: return "A long sword with a curved blade. ";
		case 9: return "An axe designed for either hand combat or throwing. ";
		case 10: return "A large axe with a double-headed blade, held with two hands. ";
		case 11: return "A long stick with a pointy blade on one end, to be held or thrown. ";
		case 12: return "A long pole with a spiked axe head on one end. ";
		case 13: return "A piece of cloth and leather for launching stones, which do a small amount of damage on impact. ";
		case 14: return "A curved piece of wood and string, for shooting arrows. It does good damage in combat, and a skilled user can use it to great effect. ";
		case 15: return "A piece of machinery used for firing bolts, which takes some time to load and fire. It does very good damage in combat. ";
		case 16: return "A small crossbow, for firing darts. ";
		case 17: return "A pole with a large, heavy blade on one end. ";
		case 18: return "A pole, held with both hands. ";
		case 19: return "A farm implement, usually unsuited to combat. ";
		case 20: return "A giant lump of wood, shaped for an ogre's hands. ";
		case 21: return "A giant lump of wood covered in sharp spikes. ";
		case 22: return "The opposite to a morningstar. ";
		case 23: return "A small and magically quick sword. ";
		case 24: return "A very rare and extremely effective imported weapon. ";
		case 25: return "A huge axe. ";
		case 26: return "A magical weapon with two razor-sharp blades. ";
		case 27: return "A magical weapon with three great razor-sharp blades. ";
		case 28: return "The kind of thing you hit nails with, adapted for battle. ";
		case 29: return "A large and vicious toothed club. ";
		case 30: return "A whip. ";
		case 31: return "A sword with a medium length slashing blade. ";
		case 32: return "A terrible weapon, forged in the fires of Facilites. ";
		case 33: return "A terrible weapon, woven in the depths of the Administrations. ";
	}
	return "";
}

std::string special_weapon_description(int item_dam)
{
	switch(item_dam) {
		case 181: return "This blessed weapon loves nothing more than to sing to its owner, whether they want it to or not. ";
		case 182: return "This was the favourite weapon of the Human Liberation Front, before they lost it one day. It induces a bloodthirsty berserker rage in anyone who uses it to strike another. ";
		case 183: return "This weapon carries a terrible and highly irritating virus. ";
		case 184: return "It is rather unreliable. ";
		case 185: return "It is the creation of a mad Corporation, and carries a virus which transforms anyone possessing it into a prune. Fortunately, the virus works very slowly, and one can use it briefly with no consequences worse than slightly purple skin and a few wrinkles. ";
		case 186: return "This truly accursed weapon is an instrument of Facilities. ";
		case 187: return "This dreadful weapon is used at the user's peril. ";
		case 188: return "Flames cover its twisted blade. ";
		case 189: return "This legendary item can unleash the fury of Facilities. ";
		case 190: return "It carries some of the powers of the arch-fiend Asmodeus. ";
		case 191: return "It rewards the powerful with power and the meek with weakness. ";
		case 192: return "It is almost unerringly accurate. ";
		case 193: return "It was the cyber weapon wielded by the mighty hacker Olgreb before he met his fate somewhere within these dungeons. It grants its wielder resistance to the effects of poison and increases their ability to use venomous hacks, and carries cyber powers which can be invoked by one skilled in the use of hacking. ";
		case 194: return "It is lethally vampiric. ";
		case 195: return "Its power varies in proportion to its wielder's intelligence. Using it can be a bit risky. ";
	}
	return "";
}

std::string special_weapon_enchantment(int spec_ench)
{
	switch(spec_ench) {
		case 1: return "It emits flame when wielded, causing extra injury to most foes and up to double damage against particularly susceptible opponents. ";
		case 2: return "It has been specially enchanted to freeze those struck by it, causing extra injury to most foes and up to double damage against particularly susceptible opponents. ";
		case 3: return "It has been blessed by the UNATCO to harm cyborgs and cause great damage to the unholy creatures of Facilities or Bioengineerings. ";
		case 4: return "Occasionally upon striking a foe it will discharge some electrical energy and cause terrible harm. ";
		case 5: return "It is especially effective against all of terrorists descent. ";
		case 6: return "It poisons the flesh of those it strikes. ";
		case 7: return "It protects the one who wields it against injury. ";
		case 8: return "A truly terrible weapon, it drains the life of those it strikes. ";
		case 9: return "It allows its wielder to attack twice when they would otherwise have struck only once. ";
		case 10: return "It inflicts extra damage upon your enemies. ";
		case 11: return "It turns projectiles fired from it into bolts of fire. ";
		case 12: return "It turns projectiles fired from it into bolts of frost. ";
		case 13: return "It inflicts no extra harm, but heals its wielder somewhat when he or she strikes a living foe. ";
		case 14: return "It is a weapon blessed by Knight Templar, and can inflict up to fourfold damage when used against the cyborgs. ";
		case 15: return "In the hands of one skilled in cybernitization it inflicts extra damage on living creatures. ";
	}
	return "";
}

std::string weapon_category(int item_class, int item_type)
{
	switch(weapon_skill(item_class, item_type)) {
		case 1: return "It falls into the 'short blades' category. ";
		case 2: return "It falls into the 'long swords' category. ";
		case 3: return "It falls into the 'great swords' category. ";
		case 4: return "It falls into the 'axes' category. ";
		case 5: return "It falls into the 'maces and flails' category. ";
		case 6: return "It falls into the 'pole-arms' category. ";
		case 7: return "It falls into the 'staves' category. ";
		case 8: return "It falls into the 'slings' category. ";
		case 9: return "It falls into the 'bows' category. ";
		case 10: return "It falls into the 'crossbows' category. ";
	}
	return "";
}

std::string weapon_description(ItemInfo item)
{
	std::string unrandart_description = std::string(unrandart_descrip(1, item.item_class, item.type, item.plus, item.plus2));
	bool is_unrandart = (item.dam % 30 == 25 && unrandart_description.size() != 0);
	bool is_missile_device = (item.type >= 13 && item.type <= 16);
	bool is_virused = (item.plus >= 100 && item.id >= 1);
	bool is_enchanted = (item.id >= 3 && !(item.item_class == 0 && item.dam % 30 >= 25));
	bool is_randart = (item.id >= 2 && item.dam % 30 >= 25);
	int spec_ench = (item.dam % 30 >= 25) ? randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_BRAND) : item.dam % 30;
	bool has_special_enchantment = (spec_ench != 0 && item.id >= 2);

	std::string result = is_unrandart ? unrandart_description : describe_ordinary_weapon(item.type);

	if(!is_missile_device) {
		Format format("$Damage rating: @1$Accuracy rating: @2$Speed multiplier (x10): @3$");
		format << property(0, item.type, 0) << property(0, item.type, 1) << property(0, item.type, 2);
		result += format.str();
	}
	if(is_virused) {
		result += "It has a virus placed upon it. ";
		item.plus -= 100;
	}
	if(is_enchanted) {
		if(item.plus < 50) {
			result += "It has been damaged to be less accurate. ";
		} else if(item.plus > 50) {
			result += "It has been " + print_ench(item.plus) + "to be more accurate. ";
		} else if(item.plus2 < 50) {
			result += "It has been damaged to cause less damage. ";
		} else if(item.plus2 > 50) {
			result += "It has been " + print_ench(item.plus2) + "to inflict greater damage. ";
		}
	}
	if(item.dam >= 180) {
		if (item.id < 2) return result;
		result += special_weapon_description(item.dam);
		return result;
	}
	if(has_special_enchantment) {
		result += special_weapon_enchantment(spec_ench);
	}
	if(is_randart) {
		result += randart_descpr(item);
	}
	result += weapon_category(item.item_class, item.type);
	return result;
}

std::string describe_ammunition(ItemInfo item)
{
	std::string result;
	switch(item.type) {
		case 0: result += "A stone. "; break;
		case 1: result += "An arrow. "; break;
		case 2: result += "A crossbow bolt. "; break;
		case 3: result += "A small throwing weapon. "; break;
		case 4: result += "A purple vegetable. The presence of this object in the game indicates a bug (or some kind of cheating on your part). "; break;
		case 5: result += "A rock, used by giants as a missile. "; break;
	}
	if(item.dam % 30 != 0 && item.id >= 2) {
		switch(item.dam % 30) {
			case 1: result += "When fired from an appropriate launcher, it turns into a bolt of flame. "; break;
			case 2: result += "When fired from an appropriate launcher, it turns into a bolt of ice. "; break;
			case 3:
			case 4: result += "It is coated with poison. "; break;
		}
	}
	if(item.id == 3 && item.plus > 50) {
		result += "It has been " + print_ench(item.plus) + "to be more accurate and cause more damage. ";
	}
	return result;
}

std::string head_armour_description(int item_plus2)
{
	switch(item_plus2) {
		case 0: return "A piece of metal headgear. ";
		case 1: return "A piece of metal headgear. ";
		case 2: return "A cloth or leather cap. ";
		case 3: return "A conical cloth hat. ";
	}
	return "";
}

std::string boots_description(int item_plus2)
{
	switch(item_plus2) {
		case 1: return "A special armour made for Assassins, to wear on their tails. ";
		case 2: return "An armour made for ranger, to wear on their mechanical bodies. ";
		default: return "A pair of sturdy boots. ";
	}
}

std::string ordinary_armour_description(int item_type, int item_plus2)
{
	switch(item_type) {
		case 0: return "A cloth robe. ";
		case 1: return "A suit made of hardened leather. ";
		case 2: return "A leather suit covered in little rings. ";
		case 3: return "A leather suit covered in little metal plates. ";
		case 4: return "A suit made of interlocking metal rings. ";
		case 5: return "A suit made of splints of metal. ";
		case 6: return "A suit made of bands of metal. ";
		case 7: return "A suit of mail and large plates of metal. ";
		case 8: return "A piece of metal, to be strapped on one's arm. ";
		case 9: return "A cloth cloak. ";
		case 10: return head_armour_description(item_plus2);
		case 11: return "A pair of gloves. ";
		case 12: return boots_description(item_plus2);
		case 13: return "A small shield. ";
		case 14: return "Like a normal shield, only larger. ";
		case 15: return "The scaly skin of a dragon. I suppose you could wear it if you really wanted to. ";
		case 16: return "The rough and knobbly hide of a troll. I suppose you could wear it if you really wanted to. ";
		case 17: return "An incredibly heavy but extremely effective suit of crystalline armour. ";
		case 18: return "A magical armour, made from the scales of a fire-breathing dragon. It provides great protection from the effects of fire, but renders its wearer more susceptible to the effects of cold. ";
		case 19: return "A magical armour, made from the skin of a common troll. It magically regenerates its wearer's flesh at a fairly slow rate (unless they're already a troll). ";
		case 20: return "The scaly skin of a dragon. I suppose you could wear it if you really wanted to. ";
		case 21: return "A magical armour, made from the scales of a cold-breathing dragon. It provides great protection from the effects of cold, but renders its wearer more susceptible to the effects of fire and heat. ";
		case 22: return "The soft and supple scaley skin of a steam dragon. I suppose you could wear it if you really wanted to. ";
		case 23: return "A magical armour, made from the scales of a steam-breathing dragon. Although unlike the armour made from the scales of some larger dragons it does not provide its wearer with any special magical protection, it is extremely light and as supple as cloth. ";
		case 24: return "The weirdly-patterned scaley skin of a mottled dragon. I suppose you could wear it if you really wanted to. ";
		case 25: return "A magical armour made from the scales of a mottled dragon. Although unlike the armour made from the scales of some larger dragons it does not provide its wearer with any special magical protection, it is as light and relatively uncumbersome as leather armour. ";
		case 26: return "The hide of a storm dragon, covered in extremely hard blue scales. I suppose you could wear it if you really wanted to. ";
		case 27: return "A magical armour made from the scales of a lightning-breathing dragon. It is heavier than most dragon scale armours, but gives its wearer great resistance to electrical discharges. ";
		case 28: return "The extremely tough and heavy skin of a golden dragon, covered in shimmering golden scales. I suppose you could wear it if you really wanted to. ";
		case 29: return "A magical armour made from the golden scales of a golden dragon. It is extremely heavy and cumbersome, but confers resistance to fire, cold, and poison on its wearer. ";
		case 30: return "The skins of several animals. ";
		case 31: return "The slimy and smelly skin of a swamp-dwelling dragon. I suppose you could wear it if you really wanted to. ";
		case 32: return "A magical armour made from the scales of a swamp dragon. It confers resistance to poison on its wearer. ";
	}
	return "";
}

std::string special_armour_ability(ItemInfo item)
{
	switch(item.dam % 30) {
		case 1: return "It allows its wearer to run at a great speed. ";
		case 2: return "It protects its wearer from heat and fire. ";
		case 3: return "It protects its wearer from cold. ";
		case 4: return "It protects its wearer from poison. ";
		case 5: return "It allows its wearer to see invisible things. ";
		case 6: return "When activated it hides its wearer from the sight of others, but also increases their metabolic rate by a large amount. ";
		case 7: return "It increases the physical power of its wearer. ";
		case 8: return "It increases the dexterity of its wearer. ";
		case 9: return "It makes you more cleverer. ";
		case 10: return "It is highly cumbersome. ";
		case 11: return "It can be activated to allow its wearer to float above the ground. ";
		case 12: return "It increases its wearer's resistance to enchantments. ";
		case 13: return "It protects its wearer from harm. ";
		case 14: return "It enhances the stealth of its wearer. ";
		case 15: return "It protects its wearer from the effects of both cold and heat. ";
		case 16: return "It protects its wearer from the effects of negative energy. ";
		case 17: return "It greatly increases the power of its wearer's hacking programs, but is only intended for those who have very little left to learn. ";
		case 18: return "It protects its wearer's possessions from damage and destruction. ";
	}
	return "";
}

std::string describe_armour(ItemInfo item)
{
	std::string unrandart_description = unrandart_descrip(1, item.item_class, item.type, item.plus, item.plus2);
	bool is_unrandart = (item.dam % 30 == 25 && unrandart_description.size() != 0);
	bool is_virused = (item.plus >= 100 && item.id >= 1);
	bool is_randart = (item.id >= 2 && item.dam % 30 >= 25);

	std::string result = is_unrandart ? unrandart_descrip(1, item.item_class, item.type, item.plus, item.plus2) : ordinary_armour_description(item.type, item.plus2);

	if(item.type != 8 && item.type != 13 && item.type != 14) {
		Format format("$Armour rating: @1$Evasion modifier: @2$");
		format << property(2, item.type, 0) << property(2, item.type, 1);
		result += format.str();
	}
	if(is_virused) {
		result += "It has a virus placed upon it. ";
		item.plus -= 100;
	}
	if (item.id >= 3) {
		if (item.plus < 50) {
			result += "It has been damaged to offer less protection against injury. ";
		} else if (item.plus > 50) {
			result += "It has been " + print_ench(item.plus) + "to be more effective in reducing injury. ";
		}
	}
	if(is_randart) {
		result += randart_descpr(item);
	}
	if (item.id > 1) {
		result += special_armour_ability(item);
	}
	return result;
}

std::string gun_ability(int item_type)
{
	switch (item_type) {
		case 0: return " throws little bits of flame. ";
		case 1: return " throws little bits of frost. ";
		case 2: return " casts enchantments to slow down the actions of a creature at which it is directed. ";
		case 3: return " casts enchantments to speed up the actions of a creature at which it is directed. ";
		case 4: return " throws small bolts of destructive energy. ";
		case 5: return " can heal a creature's wounds. ";
		case 6: return " can hold a creature immobile. ";
		case 7: return " throws great bolts of fire. ";
		case 8: return " throws great bolts of cold. ";
		case 9: return " induces confusion and bewilderment in a target creature. ";
		case 10: return " hides a creature from the view of others. ";
		case 11: return " drills tunnels through unworked rock. ";
		case 12: return " throws exploding blasts of flame. ";
		case 13: return " causes a creature to be randomly translocated. ";
		case 14: return " throws great bolts of lightning. ";
		case 15: return " causes a creature to be transmogrified into another form. It doesn't work on you, so don't even try. ";
		case 16: return " causes slavish obedience in a creature. ";
		case 17: return " throws a bolt of negative energy which drains the life essences of living creatures, but is useless against the undead. ";
		case 18: return " can produce a variety of effects. ";
		case 19: return " disrupts the physical structure of an object, especially a creature's body. ";
	}
	return "";
}

std::string describe_gun(ItemInfo item)
{
	bool is_identified = get_id(3, item.type);
	if(!is_identified) {
		return "A stick. Maybe it's magical. ";
	}
	std::string result = "A technological device which";
	result += gun_ability(item.type);
	if (item.id == 3 && item.plus == 0) {
		result += "Unfortunately, it has no charges left. ";
	}
	return result;
}

std::string describe_food(ItemInfo item)
{
	switch (item.type) {
		case 0: return "A filling ration of dried and preserved meats. ";
		case 1: return "A filling ration of bread. ";
		case 2: return "A delicious juicy fruit. ";
		case 3: return "A delicious red or green fruit. ";
		case 4: return "An almost tasteless green vegetable, which grows on a vine. ";
		case 5: return "A lump of the delicious honeycomb made by giant bees. ";
		case 6: return "A lump of the magical substance produced by giant bees to be fed to their queens. ";
		case 7: return "A repulsive cucumber-shaped vegetable. ";
		case 8: return "A slice of pizza. Don't tell me you don't know what that is! ";
		case 9: return "A delicious orange fruit. ";
		case 10: return "A delicious juicy orange fruit. ";
		case 11: return "A delicious yellow fruit, probably grown and imported by some amoral multinational as the result of a corrupt trade deal. ";
		case 12: return "A small but delicious red fruit. "; // are you noticing a pattern here?
		case 13: return "A small but delicious tropical fruit. How it got into this dungeon is anyone's guess. ";
		case 14: return "A yellow fruit. ";
		case 15: return "A small fruit. ";
		case 16: return "A dried grape. ";
		case 17: return "A tropical fruit. ";
		case 18: return "A strip of preserved dead cow. ";
		case 19: return "A lump of cheese. ";
		case 20: return "An elongated lump of low-grade gristle, entrails and cereal products encased in an intestine. Yum! ";
		case 21: return "A piece of dungeon meat. It looks rather unpleasant. " + (item.dam < 100) ? "In fact, it is rotting away before your eyes. Eating it would probably be unwise. " : "";
	}
	return "";
}

std::string describe_vial(ItemInfo item)
{
	bool is_identified = get_id(8, item.type);
	if(!is_identified) {
		return "A small bottle of liquid. ";
	}

	switch(item.type) {
		case 0: return "A blessed fluid which heals some wounds, clears the mind, and cures diseases. If one uses it when they are at or near full health, it can also slightly repair permanent injuries. ";
		case 1: return "A cybernetical healing elixir, which causes wounds to close and heal almost instantly. If one uses it when they are at or near full health, it can also repair permanent injuries. ";
		case 2: return "An enchanted beverage which speeds the actions of anyone who drinks it. ";
		case 3: return "A cyber vial which greatly increases the strength and physical power of one who drinks it. ";
		case 4: return "A vial of beneficial mutation. ";
		case 5: return "A vial of beneficial mutation. ";
		case 6: return "A vial of beneficial mutation. ";
		case 7: return "A vial which confers great buoyancy on one who consumes it. ";
		case 8: return "A nasty poisonous liquid. ";
		case 9: return "A vial which slows your actions. ";
		case 10: return "A vial which eliminates your control over your own body. ";
		case 11: return "A vial which confuses your perceptions and reduces your control over your own actions. ";
		case 12: return "A vial which hides you from the sight of others. ";
		case 13: return "A filling vial of sludge, high in cereal fibre. ";
		case 14: return "A noxious concoction, which can do terrible things to your body, brain and reflexes. ";
		case 15: return "A vile and putrid cursed liquid, which causes your flesh to decay before your very eyes. ";
		case 16: return "A unique substance, vital for the existence of most life. ";
		case 17: return "A truly wonderful and very rare drink. ";
		case 18: return "A valuable vial which grants a person with an infusion of cybernetical energy. ";
		case 19: return "A vial which restores the abilities of one who drinks it. ";
		case 20: return "A terribly venomous vial. ";
		case 21: return "A vial which can send one into an incoherent rage. ";
		case 22: return "A vial which removes some or all of any mutations which may be afflicting you. ";
		case 23: return "A vial which does very strange things to you. ";
	}
	return "";
}

std::string describe_device(ItemInfo item)
{
	bool is_identified = get_id(6, item.type);
	if(!is_identified) {
		return "A single-use device covered in writing. ";
	}
	switch(item.type) {
		case 0: return "This useful cyber device allows you to determine the properties of any object. ";
		case 1: return "Reading the words on this device translocates you to a random position. ";
		case 2: return "This device causes great fear in those who see the one who reads it. ";
		case 3: return "This device causes a loud noise. It is not universally noted for its usefulness.";
		case 4: return "Reading this device removes curses from the items you are using. ";
		case 5: return "This device allows you to detect the presence of cursed items among your possessions. ";
		case 6: return "This device opens a conduit to the Abyss and draws a terrible beast to this world for a limited time. ";
		case 7: return "This device places an enchantment on a weapon, making it more accurate in combat. It may fail to affect weapons already heavily enchanted. ";
		case 8: return "This device places an enchantment on a piece of armour. ";
		case 9: return "This device calls on the powers of Hell to inflict great pain on any nearby creature - including you! ";
		case 10: return "Does some randomly useless thing. ";
		case 11: return "This device places a curse on a weapon. ";
		case 12: return "This device places a curse on a piece of armour. ";
		case 13: return "Upon being read, this device causes a large explosion. ";
		case 14: return "This device allows its reader to teleport a short distance, with precise control. ";
		case 15: return "Apart from a label, this device is blank. ";
		case 16: return "This device reveals the nearby surroundings of one who reads it. ";
		case 17: return "This device induces an irritating disorientation. ";
		case 18: return "This wonderful device causes the creation of an item valuable to the reader at the reader's feet. It is especially treasured by specialist cyberians, as they can use it to obtain the powerful spells of their speciality. It will avoid creating some types of item if one such is already in the possession of the reader. ";
		case 19: return "This device places an enchantment on a weapon, making it inflict greater damage in combat. It may fail to affect weapons already heavily enchanted. ";
		case 20: return "This device enchants a weapon so as to make it far more effective at inflicting harm on its wielder's enemies. Using it on a weapon already affected by some kind of special enchantment (other than that produced by a normal device of enchant weapon) is not advised. ";
		case 21: return "This device restores the charges of any cybernetical wand wielded by its reader. ";
		case 22: return "This device enchants a weapon to be far more effective in combat. Although it can be used in the creation of especially enchanted weapons, it may fail to affect those already heavily enchanted. ";
	}
	return "";
}

std::string implant_ability(ItemInfo item)
{
	switch(item.type) {
		case 0: return "This wonderful chip greatly increases the recuperative powers of its wearer, but also considerably speeds his or her metabolism. ";
		case 1: return "This chip either protects its wearer from harm or makes them more vulnerable to injury, to a degree dependent on its power. ";
		case 2: return "This chip provides protection from heat and fire. ";
		case 3: return "This chip provides protection from the effects of poisons and venom. ";
		case 4: return "This chip provides protection from cold. ";
		case 5: return "This chip increases or decreases the physical strength of its wearer, to a degree dependent on its power. ";
		case 6: return "This chip increases the hand-to-hand and missile combat skills of its wearer. ";
		case 7: return "This chip allows its wearer to see those things hidden from view by magic. ";
		case 8: return "This powerful chip can be activated to hide its wearer from the view of others, but increases the speed of his or her metabolism greatly while doing so. ";
		case 9: return "This accursed chip causes its wearer to hunger considerably more quickly. ";
		case 10: return "This chip occasionally exerts its power to randomly translocate its wearer to another place, and can be deliberately activated for the same effect. ";
		case 11: return "This chip makes its wearer either more or less capable of avoiding attacks, depending on its degree of enchantment. ";
		case 12: return "This chip protects its wearer from the loss of their strength, dexterity and intelligence. ";
		case 13: return "This chip provides energy to its wearer, so that they need eat less often. ";
		case 14: return "This chip increases or decreases the dexterity of its wearer, depending on the degree to which it has been enchanted. ";
		case 15: return "This chip increases or decreases the mental ability of its wearer, depending on the degree to which it has been enchanted. ";
		case 16: return "This chip increases the ability of its wearer to use magical spells. ";
		case 17: return "This chip increases its wearer's reserves of magical power. ";
		case 18: return "This chip causes its wearer to hover above the floor. ";
		case 19: return "This blessed chip protects the life-force of its wearer from negative energy, making them immune to the draining effects of undead and necromantic magic. ";
		case 20: return "This chip increases its wearer's resistance to hostile enchantments. ";
		case 21: return "This chip brings its wearer more in contact with the powers of fire. He or she gains resistance to heat and can use fire magic more effectively, but becomes more vulnerable to the effects of cold. ";
		case 22: return "This chip brings its wearer more in contact with the powers of cold and ice. He or she gains resistance to cold and can use ice magic more effectively, but becomes more vulnerable to the effects of fire. ";
		case 23: return "This chip allows its wearer to control the destination of any teleportation, although without perfect accuracy. Trying to teleport into a solid object will result in a random teleportation, at least in the case of a normal teleportation. ";
		case 35: return "This implant enables its wearer to attempt to enter a state of berserk rage, and increases their chance of successfully doing so. ";
		case 36: return "This implant protects its wearer from some magically induced forms of slowness, and increases the duration of enchantments which speed his or her actions. ";
		case 37: return "This implant protects its wearer from some forms of mental confusion. ";
		case 38: return "This implant repels some of the attacks of creatures which have been magically summoned. ";
		case 39: return "This implant protects the armour and weaponry of its wearer from corrosion caused by acids, although not infallibly so. ";
		case 40: return "This implant allows its wearer to consume meat in various states of decay without suffering unduly as a result. Poisonous or cursed flesh is still not recommended. ";
		case 41: return "This implant protects some of the possessions of its wearer from outright destruction, but not infallibly so. ";
		case 42: return "Should the wearer of this implant be levitated by magical means, he or she will be able to exercise some control over the resulting motion. This allows the descent of staircases and the retrieval of items lying on the ground, for example, but does not deprive the wearer of the benefits of levitation. ";
		case 43: return "This implant makes its wearer less accurate in hand combat. ";
		case 44: return "This implant protects its wearer from mutations, although not infallibly so. ";
	}
	return "";
}

std::string describe_implant(ItemInfo item)
{
	std::string unrandart_description = unrandart_descrip(1, item.item_class, item.type, item.plus, item.plus2);
	bool is_unrandart = (item.dam == 201 && unrandart_description.size() != 0);
	bool is_identified = get_id(7, item.type);
	bool is_virused = (item.id > 0 && item.plus >= 100);
	bool is_randart = (item.id >= 2 && (item.dam == 200 || item.dam == 201));

	std::string result = is_unrandart ? unrandart_description : (is_identified ? implant_ability(item) : "A piece of neuroimplant. ");
	if(is_virused) {
		result += "It has a virus placed upon it. ";
	}
	if(is_randart) {
		result += randart_descpr(item);
	}
	return result;
}

std::string describe_archive(ItemInfo item)
{
	switch(item.type) {
		case 35: return "An extremely powerful but unpredictable archive of hacking. ";
		case 41: return "A valuable archive of hacking which allows one to practise a certain skill greatly. As it is used, it gradually disintegrates and will eventually fall apart. ";
		default: return "A archive of hacking programs. Beware, for some of the more powerful grimoires are not to be toyed with. ";
	}
}

std::string rom_device_ability(ItemInfo item)
{
	switch(item.type) {
		case 0: return "This ROM increases the magical proficiency of its wielder by a considerable degree, increasing the power of their programs. ";
		case 1: return "This ROM provides a reservoir of magical power to its wielder. ";
		case 2: return "This ROM increases the power of fire programs cast by its wielder, and protects him or her from the effects of heat and fire. ";
		case 3: return "This ROM increases the power of ice programs cast by its wielder, and protects him or her from the effects of cold. ";
		case 4: return "This ROM increases the power of poisoning programs cast by its wielder, and protects him or her from the effects of poison. ";
		case 5: return "This ROM allows its wielder to cast magical programs without hungering as a result. ";
		case 6: return "This ROM increases the power of necromantic programs cast by its wielder. ";
		case 7: return "This ROM increases the power of conjurations cast by its wielder. ";
		case 8: return "This ROM increases the power of enchantments cast by its wielder. ";
		case 9: return "This ROM increases the power of summonings cast by its wielder. ";
		case 10: return "This holy program ROM allows its wielder to smite foes from afar. ";
		case 11: return "This program ROM contains programs of summoning. ";
		case 16: return "This program ROM contains programs designed to repel one's enemies. ";
		case 17: return "This program ROM contains programs which reveal various aspects of an explorer's surroundings to them. ";
		case 25: return "This ROM increases the power of air programs cast by its wielder. ";
		case 26: return "This ROM increases the power of earth programs cast by its wielder. ";
		case 27: return "This ROM allows its caster to channel ambient magical energy for his or her own purposes. ";
		default: return "This program ROM contains programs of mayhem and destruction. ";
	}
}

std::string describe_rom_device(ItemInfo item)
{
	bool is_identified = (item.id != 0);
	if(!is_identified) {
		return "A stick embued with magical properties. ";
	}
	std::string result = rom_device_ability(item);
	if(item.type >= 10 && item.type < 25) {
		result += "Running a program from it consumes no food, and will not fail. ";
	}
	result += "$Damage rating: 7 $Accuracy rating: +6 $Speed multiplier (x10): +12";
	result += "$It falls into the 'staves' category. ";
	return result;
}

std::string describe_misc_item(ItemInfo item)
{
	bool is_identified = (item.id != 0);
	if(is_identified) {
		switch(item.type) {
			case 0: return "A mighty efreet, captured by some wizard and bound into a bronze flask. Breaking the flask's seal will release it to wreak havoc - possibly on you. ";
			case 1: return "A magical device which allows one to see the layout of their surroundings. It requires a degree of magical ability to be used reliably, otherwise it can produce unpredictable and possibly harmful results. ";
			case 2: return "A magical device for summoning air elementals. It is rather unreliable, and usually requires several attempts to function correctly. Using it carries an element of risk, which is reduced if one is skilled in the appropriate elemental magic. ";
			case 3: return "A magical device for summoning fire elementals. It is rather unreliable, and usually requires several attempts to function correctly. Using it carries an element of risk, which is reduced if one is skilled in the appropriate elemental magic. ";
			case 4: return "A magical device for summoning earth elementals. It is rather unreliable, and usually requires several attempts to function correctly. Using it carries an element of risk, which is reduced if one is skilled in the appropriate elemental magic. ";
			case 5: return "An unholy device which calls on the powers of darkness to assist its user, with a small cost attached. ";
			case 6: return "The horn belonging to Geryon, guardian of the Vestibule of Hell. Legends say that a mortal who desires access into one of the Hells must use it in order to gain entry. ";
			case 7: return "A magical box containing many wild beasts. One may allow them to escape by opening the box's lid. ";
			case 8: return "A deck of highly mysterious and magical cards. One may draw a random card from it, but should be prepared to suffer the possible consequences! ";
			case 9: return "A deck of magical cards, depicting a range of weird and wondrous creatures. ";
			case 10: return "A magical device which can be used to restore one's reserves of magical energy, but the use of which carries the risk of draining all of those energies completely. This risk varies inversely with the proportion of their maximum energy which the user possesses; a user near his or her full potential will find this item most beneficial. ";
			case 11: return "A magical box after its power is spent. ";
			case 12: return "A dangerous item which hypnotises anyone so unwise as to gaze into it, leaving them helpless for a significant length of time. ";
			case 13: return "This extremely powerful item can unleash a destructive storm of electricity. It is especially effective in the hands of one skilled in air elemental magic, but cannot be used by one protected from its effects. ";
			case 14: return "A talisman which allows entry into Zot's domain. ";
			case 15: return "A deck of magical cards, full of amusing tricks. ";
			case 16: return "A deck of powerful magical cards. ";
			case 17: return "An altar to Nemelex Xobeh, built for easy assembly and disassembly. Invoke it to place on a clear patch of floor, then pick it up again when you've finished. ";
		}
	} else  {
		switch(item.type) {
			case 0: return "A heavy bronze flask, warm to the touch. ";
			case 10:
			case 12:
			case 1: return "A sphere of clear crystal. ";
			case 2: return "A fan. ";
			case 3: return "A lamp. ";
			case 4: return "A lump of rock. ";
			case 5: return "A strange lantern made out of ancient bones. ";
			case 6: return "A great silver horn, radiating unholy energies. ";
			case 7: return "A small black box. I wonder what's inside? ";
			case 8:
			case 15:
			case 16:
			case 9: return "A nanite construction kit. ";
			case 11: return "A small black box. I wonder what's inside? ";
			case 13: return "A grey disc. ";
			case 17: return "An terminal to Triads, built for easy assembly and disassembly. Invoke it to place on a clear patch of floor, then pick it up again when you've finished. ";
		}

	}
	return "";
}

std::string describe_corpse(ItemInfo item)
{
	return (item.type == 0) ? "A corpse. " : "A decaying skeleton. ";
}

std::string item_description(ItemInfo item)
{
	switch(item.item_class) {
		case 0: return weapon_description(item);
		case 1: return describe_ammunition(item);
		case 2: return describe_armour(item);
		case 3: return describe_gun(item);
		case 4: return describe_food(item);
		case 8: return describe_vial(item);
		case 6: return describe_device(item);
		case 7: return describe_implant(item);
		case 10: return describe_archive(item);
		case 11: return describe_rom_device(item);
		case 12: return "Once you have escaped to the surface with this invaluable artefact, your quest is complete.";
		case 13: return describe_misc_item(item);
		case 14: return describe_corpse(item);
		default: return "This item should not exist. Mayday! Mayday!";
	}
}

int item_mass(ItemInfo item)
{
	if(item.item_class == 14) {
		if(item.type == 0) {
			return mons_weight(item.plus) / 10;
		} else {
			return mons_weight(item.plus) / 20;
		}
	} else {
		return mass(item.item_class, item.type) / 10;
	}
	return -1;
}

void describe_item(int item_class, int item_type, int item_plus, int item_plus2, int item_dam, int item_id)
{
	ItemInfo item;
	item.item_class = item_class;
	item.type = item_type;
	item.plus = item_plus;
	item.plus2 = item_plus2;
	item.dam = item_dam;
	item.id = item_id;

	clrscr();
	Format format(item_description(item) + "It weighs around @1 aum. "); /* arbitrary unit of mass */
	format << item_mass(item);
	print_description(format.str());
	if (getkey() == 0) getkey();
}

/*
Appends the various powers of a random artefact to the descpr string.
*/
std::string randart_descpr(ItemInfo item)
{
	std::string result;
	int ac_affection           = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_AC);
	int evasion_affection      = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_EVASION);
	int strength_affection     = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_STRENGTH);
	int intelligence_affection = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_INTELLIGENCE);
	int dexterity_affection    = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_DEXTERITY);
	int accuracy_affection     = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_ACCURACY);
	int damage_affection       = randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_DAMAGE);
	bool affection = (ac_affection + evasion_affection + strength_affection + intelligence_affection + dexterity_affection + accuracy_affection + damage_affection) > 0;

	if(affection) {
		Format format("$It affects your @1 (@2).");
		if(ac_affection)           format << "AC"                       << ac_affection;
		if(evasion_affection)      format << "evasion"                  << evasion_affection;
		if(strength_affection)     format << "strength"                 << strength_affection;
		if(intelligence_affection) format << "intelligence"             << intelligence_affection;
		if(dexterity_affection)    format << "dexterity"                << dexterity_affection;
		if(accuracy_affection)     format << "accuracy"                 << accuracy_affection;
		if(damage_affection)       format << "damage-dealing abilities" << damage_affection;
		result += format.str();
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_FIRE) <= -1) {
		result += "$It makes you susceptible to fire. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_FIRE) == 1) {
		result += "$It protects you from fire. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_FIRE) == 2) {
		result += "$It renders you almost immune to fire. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_COLD) <= -1) {
		result += "$It makes you susceptible to cold. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_COLD) == 1) {
		result += "$It protects you from cold. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_COLD) == 2) {
		result += "$It renders you almost immune to cold. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_ELECTRICITY) != 0) {
		result += "$It insulates you from electricity. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_POISON) != 0) {
		result += "$It protects you from poison. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_NEGATIVE_ENERGY) != 0) {
		result += "$It protects you from negative energy. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_MAGIC) != 0) {
		result += "$It protects you from magic. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_EYESIGHT) != 0) {
		result += "$It enhances your eyesight. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_INVISIBLE) != 0) {
		result += "$It lets you turn invisible. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_LEVITATE) != 0) {
		result += "$It lets you levitate. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_BLINK) != 0) {
		result += "$It lets you blink. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_CAN_TELEPORT) != 0) {
		result += "$It lets you teleport. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_BERSERK) != 0) {
		result += "$It lets you go berserk. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_MAPPING) != 0) {
		result += "$It lets you sense your surroundings. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_NOISES) != 0) {
		result += "$It makes noises. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_PREVENT_SPELLCASTING) != 0) {
		result += "$It prevents spellcasting. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_CAUSE_TELEPORTATION) != 0) {
		result += "$It causes teleportation. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_PREVENT_TELEPORTATION) != 0) {
		result += "$It prevents most forms of teleportation. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_ANGRY) != 0) {
		result += "$It makes you angry. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, RAP_METABOLISM) != 0) {
		result += "$It speeds your metabolism. ";
	}
	if (randart_wpn_properties(item.item_class, item.type, item.dam, item.plus, item.plus2, 0, 25) != 0) {
		result += "$It emits mutagenic radiations, which may remain in your system for quite some time. ";
	}
	result += "$";

	if((item.dam == 25 && (item.item_class == OBJ_WEAPONS || item.item_class == OBJ_ARMOUR)) || (item.dam == 201 && item.item_class == OBJ_JEWELLERY)) {
		result += unrandart_descrip(0, item.item_class, item.type, item.plus, item.plus2);
		result += "$";
	}
	return result;
}

/*
Used for weapons, armour and ammo.
*/
std::string print_ench(int item_plus)
{
	if (item_plus < 52) {
		return "lightly enchanted ";
	} else if (item_plus < 54) {
		return "moderately enchanted ";
	} else if (item_plus < 56) {
		return "heavily enchanted ";
	} else if (item_plus < 58) {
		return "very heavily enchanted ";
	}
	return "extremely heavily enchanted ";
}

std::string program_description(int program_id)
{
	switch(program_id) {
		case 0: return " allows the caster to determine the properties of an otherwise inscrutable magic item. ";
		case 1: return " teleports the caster to a random location. ";
		case 2: return " causes fear in those near to the caster. ";
		case 3: return " causes a loud noise to be heard. ";
		case 4: return " removes curses from any items which are being used by the caster. ";
		case 5: return " hurls a small bolt of magical energy. ";
		case 6: return " hurls an exploding bolt of fire. ";
		case 13: return " creates a column of roaring flame. ";
		case 14: return " digs a tunnel through unworked rock. ";
		case 15: return " hurls a great bolt of flames. ";
		case 16: return " hurls a great bolt of ice and frost. ";
		case 17: return " hurls a mighty bolt of lightning. Although this spell causes less damage than the similar fire and ice spells, it can rip through whole rows of creatures. ";
		case 20: return " randomly alters the form of another creature. ";
		case 21: return " slows the actions of a creature. ";
		case 22: return " speeds the actions of a creature. ";
		case 23: return " prevents a creature from moving. ";
		case 24: return " induces a state of bewilderment and confusion in a creature's mind. ";
		case 25: return " hides a creature from the sight of others. ";
		case 26: return " throws a small bolt of flame. ";
		case 27: return " throws a small bolt of frost. ";
		case 28: return " allows a short-range translocation, with precise control. ";
		case 29: return " conjures up a large cloud of lethally cold vapour. ";
		case 30: return " conjures up a large but short-lived cloud of vile fumes. ";
		case 31: return " surrounds the caster with a mobile ring of searing flame. It is not recommended for those vulnerable to the effects of fire. ";
		case 32: return " restores the physical strength of the caster. ";
		case 33: return " restores the intelligence of the caster. ";
		case 34: return " restores the dexterity of the caster. ";
		case 35: return " throws a bolt of deadly poison. ";
		case 36: return " bathes the caster's surroundings with a poisonous green light. ";
		case 37: return " randomly translocates another creature. ";
		case 38: return " heals a small amount of damage to the caster's body. ";
		case 39: return " heals a large amount of damage to the caster's body. ";
		case 40: return " removes poison from the caster's system. ";
		case 41: return " purifies the caster's body, removing poison, disease, and some malign enchantments. ";
		case 42: return " is extremely powerful, but carries a degree of risk. It renders a living caster almost invulnerable to harm for a brief period of time, but can bring them dangerously close to death (how close depends on their necromantic abilities). The spell can be cancelled at any time by any healing effect, and the caster will recieve one warning shortly before the spell expires. Undead cannot use this spell. ";
		case 43: return " allows the caster to selectively erase a spell from memory and regain the magical energy bound up in it. One will be able to memorise this spell even if their mind is otherwise full of magic (ie you have the maximum number of spells already; mere lack of spell levels is insufficient). ";
		case 44: return " causes confusion in all who see the caster. ";
		case 45: return " smites a creature of the caster's choice. ";
		case 46: return " calls on divine power to repel the unholy. ";
		case 47: return " involves the speaking of a word of power which repels and can destroy unholy creatures. ";
		case 48: return " alerts the caster to the presence of curses on his or her possessions. ";
		case 49: return " summons a small creature to the caster's aid. ";
		case 50: return " attempts to send hostile summoned creatures to the place from whence they came, or at least reduce the duration of their presence in the caster's locality. ";
		case 51: return " summons one or more giant scorpions to the caster's assistance. ";
		case 52: return " allows the caster to float in the air. ";
		case 53: return " hurls a deadly bolt of negative energy, which drains the life of any living creature which it strikes. ";
		case 54: return " hurls a lethally sharp bolt of crystal. ";
		case 55: return " causes enormous damage to any creature struck by the bolt of incandescent energy that it conjures into existence. Unfortunately, it is very difficult to aim and very rarely hits anything. Pity, that. ";
		case 56: return " conjures forth a great cloud of lethal gasses. ";
		case 57: return " creates a mighty storm of roaring flame. ";
		case 58: return " detects any traps in the caster's vicinity. ";
		case 59: return " randomly translocates the caster a short distance. ";
		case 60: return " throws a crackling sphere of destructive energy. ";
		case 61: return " summons forth a pestilential swarm. ";
		case 62: return " opens a gate to the Abyss and calls through one or more hideous abominations from that dreadful place. The powers who supervise this invocation require the caster to surrender some of his or her intelligence in exchange for their service. ";
		case 63: return " causes an otherwise hostile creature to fight on your side for a while. ";
		case 64: return " reveals your surroundings. ";
		case 65: return " heals another creature from a distance. ";
		case 66: return " causes the dead to rise up and serve the caster; all corpses within a certain distance of the caster are affected. A powerful magician can create an army of mindless undead slaves with this spell. ";
		case 67: return " causes an extremely painful injury to a living creature. ";
		case 68: return " extends the durations of most beneficial enchantments affecting the caster. ";
		case 69: return " attempts to charm any undead in the vicinity of the caster. ";
		case 70: return " raises a skeleton to a state of unlife. ";
		case 71: return " steals the life of a living creature and grants it to the caster. It will not drain more life than the caster is capable of absorbing. ";
		case 72: return " calls on the powers of the undead to aid the caster. ";
		case 73: return " detects any items lying in the caster's general vicinity. ";
		case 74: return " instantly heals any and all wounds suffered by the caster, but permanently reduces their resilience by an amount dependent on their magical skill. ";
		case 75: return " burns a creature. ";
		case 76: return " freezes a creature. ";
		case 77: return " calls forth a spirit from the elemental planes to aid the caster. A large quantity of the desired element must be available; this is rarely a problem for earth and air, but may be for fire or water. The elemental will usually be friendly to the caster (especially if they are skilled in the appropriate form of elemental magic). ";
		case 78: return " drains the heat from the caster and the caster's surroundings, causing harm to all creatures not resistant to cold. ";
		case 79: return " conjures a sticky glob of liquid fire, which adheres to any creature it strikes and burns. ";
		case 80: return " calls forth a beast of ice to serve the caster. ";
		case 81: return " covers the caster's body with a protective layer of ice, the power of which depends on his or her skill with Ice magic. The caster and the caster's equipment are protected from the cold, but this spell will not function if the caster is already wearing heavy armour. ";
		case 82: return " calls forth a minor demon from the pits of Hell. ";
		case 83: return " reduces the chance of projectile attacks striking the caster. Even powerful attacks such as lightning bolts or dragon breath are affected, although smaller missiles are repelled to a much greater degree.";
		case 84: return " sends the caster into a temporary psychotic rage. ";
		case 85: return " inflicts a great deal of damage on an undead creature. ";
		case 110: return " allows its caster to imbue a mass of deceased flesh with a magical life force. Casting this spell involves the assembling of a heap of several corpses; the greater the mass of flesh available, the greater the chances of success. ";
		case 111: return " dramatically but temporarily increases the caster's recuperative abilities, while also increasing the rate at which he or she consumes food. ";
		case 112: return " uses the bones of a skeleton (or the bone-like exoskeleton of, for example, an insect) as the raw materials for a lethal spray of slicing bone fragments, allowing its creator to dispense with conjuration magic entirely and use necromancy alone for a low-level but very powerful combat spell. Using a large and heavy skeleton (by wielding it) results in a more powerful effect. ";
		case 113: return " banishes a creature to the Abyss. Magicians wishing to visit that unpleasant place may use it to banish themselves. ";
		case 114: return " mutates a creature into a pulsating mass of flesh. ";
		case 115: return " throws a magical dart of poison. ";
		case 116: return " converts flesh, blood, and other bodily fluids into magical energy. The caster may use his or her own body as the focus for this spell (which can be dangerous but never directly lethal), or can wield a piece of butchered flesh and draw power from that. ";
		case 117: return " causes a weapon held in the caster's hand to dance into the air and strike the caster's enemies. It will not function on magical staves or certain items of a unique nature. ";
		case 118: return " should only be available from Dispater's staff. So how are you reading this? ";
		case 119: return " opens a gate to the realm of Pandemonium and draws forth one of its inhabitants to serve the caster for a time. ";
		case 120: return " draws a small swarm of small demons to do battle with the caster's foes. ";
		case 121: return " calls one of the greater demons of Pandemonium forth to serve the caster. Beware, for the spell binding it to service may not outlast the spell binding it to this world! ";
		case 122: return " rapidly accelerates the process of decay in any corpses lying around the caster, in the process emitting a foul miasmic vapour which eats away at the life force of any creature within it. ";
		case 123: return " bestows a lethal but temporary sharpness on a sword held by the caster. It will not affect weapons which are otherwise affected by special enchantments. ";
		case 124: return " sets a weapon held by the caster on fire. It will not affect weapons which are otherwise affected by special enchantments. ";
		case 125: return " infuses a weapon held by the caster with an aura of freezing cold. It will not affect weapons which are otherwise affected by special enchantments. ";
		case 126: return " infuses a weapon held by the caster with unholy energies. It will not affect weapons which are otherwise affected by special enchantments. ";
		case 127: return " crushes a nearby creature with waves of gravitational force. ";
		case 128: return " hurls a large and heavy metal bolt at the caster's foes. ";
		case 129: return " hurls a sharp spine of rock. ";
		case 130: return " entombs the caster within four walls of rock. These walls will destroy most objects in their way, but their growth is obstructed by the presence of any creatures. Beware - one would be unwise to use this spell without a reliable escape route. ";
		case 131: return " protects the caster with chunky scales of stone, the power of which depends on his or her Earth magic skill. These scales can coexist with other forms of armour, but are extremely heavy and cumbersome. ";
		case 132: return " throws a bolt of electricity. ";
		case 133: return " imbues its caster with the ability to run extremely fast. It will not increase the speed of those who are fleet of foot anyway (such as centaurs). ";
		case 134: return " grants its caster the ability to fly through the air. ";
		case 135: return " protects its caster from electric shocks. ";
		case 136: return " hurls a crackling orb of electrical energy which causes a huge explosion on impact. ";
		case 137: return " allows the caster to detect any creatures within a certain radius. ";
		case 138: return " removes some or all of any poisons from the caster's system. ";
		case 139: return " allows the caster to control his or her translocations. ";
		case 140: return " poisons missile ammunition held by the caster. ";
		case 141: return " covers a sharp bladed weapon with poison for a while. ";
		case 142: return " protects your body from any poisons which may enter it for a period of time. ";
		case 143: return " produces a noise emanating from a place of the caster's choice. ";
		case 144: return " causes a weird alteration in the caster's body, leaving the caster in a weakened state (but never killing them). It may fail to affect those who are heavily mutated already. ";
		case 146: return " is greatly prized by summoners and necromancers, as it allows the caster to recall any friendly creatures who may be nearby to a position adjacent to the caster.";
		case 147: return " creates a gate allowing long-distance travel through a relatively ordinary environment (ie the Dungeon only). It lasts long enough for the caster and nearby creatures to enter. It will not take you past the level limits of your current area.";
		case 148: return " cuts the resilience of a target creature in half, although it will never cause death directly.";
		case 149: return " temporarily transforms the caster into a venomous spider-like creature.";
		case 150: return " disrupts the matter of a creature's body, causing injury.";
		case 151: return " tears apart the matter making up an object (especially a creature's body), causing severe damage.";
		case 152: return " causes long, scythe-shaped blades to grow from the caster's hands. It makes spellcasting a little difficult.";
		case 153: return " temporarily transforms the caster into a slow-moving but extremely robust stone statue.";
		case 154: return " temporarily transform's the caster's body into a frozen ice-creature.";
		case 155: return " temporarily transforms the caster into a great fire-breathing dragon.";
		case 156: return " first transforms the caster into a semi-corporeal apparition receptive to negative energy, then infuses that form with the powers of Death. The caster becomes resistant to cold, poison, magic and hostile negative energies.";
		case 157: return " raises living creatures slain by the caster into a state of unliving slavery as spectral horrors.";
		case 158: return " calls on the powers of Hell to cause agonising injuries to anyone or anything living in the caster's vicinity. It carries a degree of danger for any necromancer brave enough to invoke it, for the Symbol also affects its caller and indeed will not function if they are immune to its terrible effects. Despite its power, however, it is never lethal.";
		case 159: return " protects the caster from any kind of projectile attack, although particular powerful attacks (lightning bolts, etcetera) are deflected less than lighter missiles. ";
		case SPELL_ORB_OF_FRAGMENTATION: return " throws a heavy sphere of metal which explodes on impact into a blast of deadly jagged fragments. It can rip a creature to shreds, but is ineffective against heavily armoured targets.";
		case SPELL_ICE_BOLT: return " throws a chunk of ice. It is particularly effective against those creatures not immune to the effects of freezing temperatures, but half of its destructive potential comes from its weight and sharp edges and cannot be ignored even by cold-resistant creatures.";
		case SPELL_ICE_STORM: return " conjures forth a raging blizzard of ice, sleet and freezing gasses.";
		case SPELL_ARC: return " zaps a nearby creature with a powerful electrical current.";
		case SPELL_AIRSTRIKE: return " causes the air around a creature to twist itself into a whirling vortex and attack.";
		case SPELL_SHADOW_CREATURES: return " weaves a creature from shadows and threads of Abyssal matter. The creature thus brought into existence will be a recreation of some type of creature found in the caster's immediate vicinity. The spell even creates appropriate pieces of equipment for the creature, which are given a lasting substance by their contact with firm reality.";
		default: return "This program apparently does not exist. Casting it may therefore be unwise. ";
	}
}

void describe_spell(int spelled)
{
	std::string description = "This program" + program_description(spelled);

	clrscr();
	print_description(description);
	if (getkey() == 0) getkey();
}

std::string corporation_description(int corporation_id)
{
	bool kiku_protects = (you[0].piety >= 50 && you[0].religion == GOD_KIKUBAAQUDGHA);
	bool vehu_sacrifice = (you[0].piety >= 30 && you[0].religion == GOD_VEHUMET);
	bool vehu_not_fail = (you[0].piety >= 50 && you[0].religion == GOD_VEHUMET);
	bool vehu_protects = (you[0].piety >= 75 && you[0].religion == GOD_VEHUMET);
	bool makh_sacrifice = (you[0].piety >= 30 && you[0].religion == GOD_MAKHLEB);
	bool sif_protects = (you[0].piety >= 100 && you[0].religion == GOD_SIF_MUNA);
	switch(corporation_id) {
		case 1: return "Zin is an ancient and revered God, dedicated to the "
				"establishment of order and the destruction of the forces of chaos and night. "
					"Valued "
					"worshippers can gain a variety of powers useful in the fight against evil, "
					"but must abstain from the use of necromancy and other forms of unholy magic. "
					"Zin appreciates long-standing faith as well as sacrifices of valued objects "
					"and the slaying of demons and the undead.";
		case 2: return "The Shining One is a powerful crusading diety, allied with "
				"Zin in the fight against evil. Followers may be granted with the ability to "
					"summarily dispense the wrath of heaven, but must never use any form of evil "
					"magic and should fight honourably. The Shining One appreciates long-standing persistence in the endless "
					"crusade, as well as the dedicated destruction of unholy creatures.";
		case 3: return "Kikubaaqudgha is a terrible Demon-God, served by those who "
				"seek knowledge of the powers of death. Followers gain special powers "
					"over the undead, and especially favoured servants can call on mighty demons "
					"to slay their foes. $Kikubaaqudgha requires the deaths of living creatures "
					"as often as possible, but is not interested in the offering of corpses except "
					"at an appropriate altar." + kiku_protects ? "$Kikubaaqudgha is protecting you from the side-effects of death magic." : "";
		case 4: return "Yredelemnul is worshipped by those who seek powers over "
				"death and the undead without having to learn to use necromancy. Followers "
					"can raise legions of servile undead and gain a number of other useful (if "
					"unpleasant) powers. Yredelemnul appreciates killing, but prefers corpses "
					"to be put to use rather than sacrificed.";
		case 5: return "Xom is a wild and unpredictable God of chaos, who seeks not "
				"worshippers but playthings to toy with. Many choose to follow Xom in the "
					"hope of receiving fabulous rewards and mighty powers, but Xom is nothing if "
					"not capricious.";
		case 6: return "Vehumet is a God of the destructive powers of magic. "
				"Followers gain various useful powers to enhance their command of "
					"the hermetic arts, and the most favoured stand to gain access to "
					"some of the fearsome spells in Vehumet's library. One's devotion to Vehumet "
					"can be proved by the causing of as much carnage and destruction as possible."
					+ vehu_sacrifice ? "$You can gain power from the those you kill in Vehumet's name, or those slain by your servants." : ""
					+ vehu_not_fail ? "$Your conjurations and summonings cast in Vehumet's name rarely fail." : ""
					+ vehu_protects ? "$During prayer you are protected from summoned creatures." : "";
		case 7: return "Okawaru is a dangerous and powerful God of battle. Followers "
				"can gain a number of powers useful in combat as well as various rewards, "
					"but must constantly prove "
					"themselves through battle and the sacrifice of corpses and valuable items.";
		case 8: return "Makhleb the Destroyer is a fearsome God of chaos and violent "
				"death. Followers, who must constantly appease Makhleb with blood, stand to "
					"gain various powers of death and destruction. The Destroyer appreciates "
					"sacrifices of corpses and valuable items."
					+ makh_sacrifice ? "$You can gain power from the deaths of those killed in Makhleb's name." : "";
		case 9: return "Sif Muna is a contemplative but powerful deity, served by "
				"those who seek magical knowledge. Sif Muna appreciates sacrifices of valuable "
					"items, and the casting of spells as often as possible."
					+ sif_protects ? "$Sif Muna is protecting you from some of the side-effects of magic." : "";
		case 10: return "Trog is an ancient God of anger and violence. Followers are "
				 "expected to kill in Trog's name and sacrifice the dead, and in return gain "
					 "power in battle and occasional rewards. Trog hates wizards, and followers "
					 "are forbidden the use of spell magic.";
		case 11: return "Nemelex is a strange and unpredictable trickster God, whose "
				 "powers can be invoked through the magical packs of cards which Nemelex "
					 "paints in the ichor of demons. Followers receive occasional gifts, and "
					 "should use these gifts as much as possible. Offerings of items are also "
					 "appreciated.";
		case 12: return "Elyvilon the Healer is worshipped by the healers (among "
				 "others), who gain their healing powers by long worship and devotion. Although Elyvilon "
					 "prefers a creed of pacifism, those who crusade against evil are not "
					 "excluded. Elyvilon appreciates the offering of weapons.";

	}
	return "";
}

void describe_god(int which_god)
{
	clrscr();
	print_description(corporation_description(which_god));
	if (getkey() == 0) getkey();
}

/*
Takes a descpr string filled up with stuff from other functions, and
displays it with minor formatting to avoid cut-offs in mid word and such.
The character $ is interpreted as a CR.
*/
void print_description(const std::string & description)
{
	int j = 0;
	textcolor(7);
	for(size_t i = 0; i < description.size(); i ++) {
		if(description[i] == '$') {
			gotoxy(1, wherey() + 1);
			j = 0;
		} else {
			putch(description[i]);
		}
		if(j % 70 >= 58 && description[i] == 32 && description[i + 1] != '$') {
			gotoxy(1, wherey() + 1);
			j = 0;
		}
		j ++;
	}
}

class LocalRandSeed {
public:
	LocalRandSeed(const std::string & name)
		: old_seed(random())
	{
		srandom(get_seed_from_string(name));
	}
	~LocalRandSeed()
	{
		srandom(old_seed);
	}
private:
	int old_seed;
	int get_seed_from_string(const std::string & str)
	{
		int result;
		for(std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
			result += *i;
		}
		return result * str.size();
	}
};

/* Describes the random demons you find in Pandemonium */
std::string describe_demon()
{
	LocalRandSeed rand_seed(ghost.gname);
	std::string result = "A powerful demon, ";
	result += ghost.gname;
	result += " has a";
	switch(random2(31)) {
		case 0: result += " huge, barrel-shaped "; break;
		case 1: result += " wispy, insubstantial "; break;
		case 2: result += " spindly "; break;
		case 3: result += " skeletal "; break;
		case 4: result += " horribly deformed "; break;
		case 5: result += " spiny "; break;
		case 6: result += " waif-like "; break;
		case 7: result += " scaly "; break;
		case 8: result += " sickeningly deformed "; break;
		case 9: result += " bruised and bleeding "; break;
		case 10: result += " sickly "; break;
		case 11: result += " mass of writhing tentacles for a "; break;
		case 12: result += " mass of ropey tendrils for a "; break;
		case 13: result += " tree trunk-like "; break;
		case 14: result += " hairy "; break;
		case 15: result += " furry "; break;
		case 16: result += " fuzzy "; break;
		case 17: result += "n obese "; break;
		case 18: result += " fat "; break;
		case 19: result += " slimey "; break;
		case 20: result += " wrinkled "; break;
		case 21: result += " metallic "; break;
		case 22: result += " glassy "; break;
		case 23: result += " crystalline "; break;
		case 24: result += " muscular "; break;
		case 25: result += "n icky "; break;
		case 26: result += " swollen "; break;
		case 27: result += " lumpy "; break;
		case 28: result += " armoured "; break;
		case 29: result += " carapaced "; break;
		case 30: result += " slender "; break;
	}
	result += "body";
	switch(random2(40)) {
		case 0: if (ghost.ghs [10] == 1) result += " with small insectoid wings"; break;
		case 1: if (ghost.ghs [10] == 1) result += " with large insectoid wings"; break;
		case 2: if (ghost.ghs [10] == 1) result += " with moth-like wings"; break;
		case 3: if (ghost.ghs [10] == 1) result += " with buttefly wings"; break;
		case 4: if (ghost.ghs [10] == 1) result += " with huge, bat-like wings"; break;
		case 5: if (ghost.ghs [10] == 1) result += " with fleshy wings"; break;
		case 6: if (ghost.ghs [10] == 1) result += " with small, bat-like wings"; break;
		case 7: if (ghost.ghs [10] == 1) result += " with hairy wings"; break;
		case 8: if (ghost.ghs [10] == 1) result += " with great feathered wings"; break;
		case 9: if (ghost.ghs [10] == 1) result += " with shiny metal wings"; break;
		case 10: if (ghost.ghs [10] == 2) result += " which hovers in mid-air"; break;
		case 11: if (ghost.ghs [10] == 2) result += " with sacs of gas hanging from its back"; break;
		case 12: result += " covered in tiny crawling spiders"; break;
		case 13: result += " covered in tiny crawling insects"; break;
		case 14: result += " and the head of a crocodile"; break;
		case 15: result += " and the head of a hippopotamous"; break;
		case 16: result += " and a cruel curved beak for a mouth"; break;
		case 17: result += " and a straight sharp beak for a mouth"; break;
		case 18: result += " and no head at all"; break;
		case 19: result += " and a hideous tangle of tentacles for a mouth"; break;
		case 20: result += " and an elephantine trunk"; break;
		case 21: result += " and an evil-looking proboscis"; break;
		case 22: result += " and dozens of eyes"; break;
		case 23: result += " and two ugly heads"; break;
		case 24: result += " and a long serpentine tail"; break;
		case 25: result += " and a pair of huge tusks growing from its jaw"; break;
		case 26: result += " and a single huge eye, in the centre of its forehead"; break;
		case 27: result += " and spikes of black metal for teeth"; break;
		case 28: result += " and a disc-shaped sucker for a head"; break;
		case 29: result += " and huge, flapping ears"; break;
		case 30: result += " and a huge, toothy maw in the centre of its chest"; break;
		case 31: result += " and a giant snailshell on its back"; break;
		case 32: result += " and a dozen heads"; break;
		case 33: result += " and the head of a jackal"; break;
		case 34: result += " and the head of a baboon"; break;
		case 35: result += " and a huge, slobbery tongue"; break;
		case 36: result += " which is covered in oozing lacerations"; break;
		case 37: result += " and the head of a frog"; break;
		case 38: result += " and the head of a yak"; break;
		case 39: result += " and eyes out on stalks"; break;
	}
	result += ".";
	switch(random2(40)) {
		case 0: result += " It radiates an aura of extreme power."; break;
		case 1: result += " It is surrounded by a sickening stench."; break;
		case 2: result += " It is surrounded by a brilliant glow"; break;
		case 3: result += " It radiates an aura of extreme hatred for all that lives."; break;
		case 4: result += " Tiny orange flames dance around it."; break;
		case 5: result += " Tiny purple flames dance around it."; break;
		case 6: result += " It is surrounded by a weird haze."; break;
		case 7: result += " It glows with a malevolent light."; break;
		case 8: result += " It looks incredibly angry."; break;
		case 9: result += " It oozes with slime."; break;
		case 10: result += " It dribbles constantly."; break;
		case 11: result += " Mould grows all over it."; break;
		case 12: result += " It looks diseased."; break;
		case 13: result += " It looks as frightened of you as you are of it."; break;
		case 14: result += " It stinks."; break;
		case 15: result += " It moves in a serious of hideous convulsions."; break;
		case 16: result += " It moves with an unearthly grace."; break;
		case 17: result += " It hungers for your soul!"; break;
		case 18: result += " Its joints are swollen as if afflicted by arthritis."; break;
		case 19: result += " smells like rotting flesh."; break;
		case 20: result += " shimmers before your eyes."; break;
	}
	return result;
}

