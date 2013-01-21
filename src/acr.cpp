#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "ability.h"
#include "chardump.h"
#include "command.h"
#include "debug.h"
#include "describe.h"
#include "direct.h"
#include "dungeon.h"
#include "effects.h"
#include "enum.h"
#include "externs.h"
#include "fight.h"
#include "files.h"
#include "food.h"
#include "invent.h"
#include "it_use2.h"
#include "it_use3.h"
#include "item_use.h"
#include "itemname.h"
#include "items.h"
#include "levels.h"
#include "linuxlib.h"
#include "message.h"
#include "misc.h"
#include "monplace.h"
#include "monstuff.h"
#include "mstruct.h"
#include "mutation.h"
#include "newgame.h"
#include "ouch.h"
#include "output.h"
#include "player.h"
#include "priest.h"
#include "randart.h"
#include "religion.h"
#include "skills.h"
#include "skills2.h"
#include "spell.h"
#include "spells.h"
#include "spells2.h"
#include "spells3.h"
#include "stuff.h"
#include "transfor.h"
#include "view.h"

/* Functions in main module */
void input(void);
void open_door(int move_x, int move_y);
void close_door(int move_x, int move_y);
void move(int move_x, int move_y);
void initialise(void);

struct environ env [1];
struct player you [1];
int gmon_use [1000];
int stealth; /* externed in view.h */
int visible [10];

extern int your_sign; /* these two are defined in view.cc. What does the player look like? (changed for shapechanging */
extern int your_colour;

extern int wield_change; /* defined in output.cc */

/*
It all starts here. Some initialisations are run first, then straight to
new_game and then input.
*/
int main()
{
	lincurses_startup();
	initial();
	initialise();
	while(true) {
		input();
	}
	lincurses_shutdown();
	return 0;
}

void run_delay()
{
	if (you[0].delay_t > 1) {
		you[0].delay_t--;
	}
	if (you[0].delay_t == 1) {
		switch(you[0].delay_doing) {
			case 0: mpr("You finish eating."); break;
			case 1: mpr("You finish putting on your armour."); break;
			case 2: mpr("You finish taking off your armour."); break;
			case 3: mpr("You finish installing."); break;
			case 4:
					if (you[0].species == SP_TROLL || you[0].species == SP_GHOUL) {
						mpr("You finish ripping the corpse into pieces.");
					} else {
						mpr("You finish chopping the corpse into pieces.");
					}
					break;
		}
		you[0].delay_t = 0;
	}
}

int get_key_while_running(int & move_x, int & move_y) // FIXME References are bad!
{
	int keyin = 125;
	move_x = you[0].run_x;
	move_y = you[0].run_y;

	if (kbhit()) {
		you[0].running = 0;
		return 0;
	}

	if (you[0].run_x == 0 && you[0].run_y == 0) {
		you[0].running--;
		keyin = '.';
	}
	return keyin;
}

int get_control(int & move_x, int & move_y) // FIXME References are bad!
{
	int keyin = '.';
	if (you[0].paralysis == 0) {
		run_delay();

		gotoxy(18,9);

		if (you[0].delay_t == 0) {
			_setcursortype(_NORMALCURSOR);

			bool is_running = (you[0].running > 0);
			if (is_running) {
				keyin = get_key_while_running(move_x, move_y);
				if(keyin == 0) {
					keyin = getkey();
				}
			} else {
				keyin = getkey();
			}
			mesclr();
		}
	}
	return keyin;
}

void print_experience_info()
{
	msg("You are a level @1 @2 @3.") << you[0].xl << species_name(you[0].species) << you[0].clasnam;
	if (you[0].xl == 27) {
		msg("I'm sorry, level 27 is as high as you can go.");
		msg("With the way you've been playing, I'm surprised you got this far.");
		return;
	}
	msg("Your next level is at @1 experience points.") << (exp_needed(you[0].xl + 2, you[0].species) + 1);
}

void process_control(int keyin, int & move_x, int & move_y) // FIXME References are bad!
{
	if (keyin != 125) {
		move_x = 0;
		move_y = 0;
		you[0].turnover = 0;
	}

	int plox [2];
	switch(keyin) {
		case 25: open_door(-1, -1); move_x = 0; move_y = 0; break;
		case 11: open_door(0, -1); move_x = 0; move_y = 0; break;
		case 21: open_door(1, -1); move_x = 0; move_y = 0; break;
		case 12: open_door(1, 0); move_x = 0; move_y = 0; break;
		case 14: open_door(1, 1); move_x = 0; move_y = 0; break;
		case 10: open_door(0, 1); move_x = 0; move_y = 0; break;
		case 2: open_door(-1, 1); move_x = 0; move_y = 0; break;
		case 8: open_door(-1, 0); move_x = 0; move_y = 0; break;
		case 'b': move_x = -1; move_y = 1; break;
		case 'j': move_y = 1; move_x = 0; break;
		case 'u': move_x = 1; move_y = -1; break;
		case 'k': move_y = -1; move_x = 0; break;
		case 'y': move_y = -1; move_x = -1; break;
		case 'h': move_x = -1; move_y = 0; break;
		case 'n': move_y = 1; move_x = 1; break;
		case 'l': move_x = 1; move_y = 0; break;

		case 'B': you[0].run_x = -1; you[0].run_y = 1; you[0].running = 2; break;
		case 'J': you[0].run_y = 1; you[0].run_x = 0; you[0].running = 2; break;
		case 'U': you[0].run_x = 1; you[0].run_y = -1; you[0].running = 2; break;
		case 'K': you[0].run_y = -1; you[0].run_x = 0; you[0].running = 2; break;
		case 'Y': you[0].run_y = -1; you[0].run_x = -1; you[0].running = 2; break;
		case 'H': you[0].run_x = -1; you[0].run_y = 0; you[0].running = 2; break;
		case 'N': you[0].run_y = 1; you[0].run_x = 1; you[0].running = 2; break;
		case 'L': you[0].run_x = 1; you[0].run_y = 0; you[0].running = 2; break;

		case '1': you[0].run_x = -1; you[0].run_y = 1; you[0].running = 2; break;
		case '2': you[0].run_y = 1; you[0].run_x = 0; you[0].running = 2; break;
		case '9': you[0].run_x = 1; you[0].run_y = -1; you[0].running = 2; break;
		case '8': you[0].run_y = -1; you[0].run_x = 0; you[0].running = 2; break;
		case '7': you[0].run_y = -1; you[0].run_x = -1; you[0].running = 2; break;
		case '4': you[0].run_x = -1; you[0].run_y = 0; you[0].running = 2; break;
		case '3': you[0].run_y = 1; you[0].run_x = 1; you[0].running = 2; break;
		case '6': you[0].run_x = 1; you[0].run_y = 0; you[0].running = 2; break;
		case '5': you[0].run_x = 0; you[0].run_y = 0; you[0].running = 100; break;

		case '<': up_stairs(); break;
		case '>': down_stairs(0, you[0].your_level); break;
		case 'o': open_door(100, 100); break;
		case 'c': close_door(100, 100); break;
		case 'd': drop(); break;
		case 'D': butchery(); break;
		case 'i': get_invent(-1); break;
		case 'I': invoke_wielded(); break;
		case 'g':
		case ',': pickup(); break;
		case ';': item_check(';'); break;
		case 'w': wield_weapon(0); break;
		case 't': throw_anything(); break;
		case 'f': shoot_thing(); break;
		case 'T': takeoff_armour(); break;
		case 'R': remove_ring(); break;
		case 'P': puton_ring(); break;
		case 'W': wear_armour(); break;
		case '=': adjust(); return;
		case 'M': which_spell(); break;
		case 'z': zap_wand(); break;
		case 'e': eat_food(); break;
		case 'a': species_ability();
				  redraw_screen();
				  break;
		case 'A': display_mutations();
				  redraw_screen();
				  break;
		case 'V': original_name(); break;
		case 'p': pray(); break;
		case '^':
				  if (you[0].religion == GOD_NO_GOD)
				  {
					  mpr("You aren't employed.");
					  break;
				  }
				  describe_god(you[0].religion);
				  redraw_screen();
				  break;
		case '.':
				  search_around();
				  move_x = 0;
				  move_y = 0;
				  you[0].turnover = 1; break;
		case 'q': drink(); break;
		case 'r': read_scroll(); break;
		case 'x': struct dist lmove [1];
				  look_around(lmove);
				  //		 move_x = 0; move_y = 0;
				  break;
		case 's':
#ifdef DEBUG
				  stethoscope(250); break;
#endif
				  search_around();
				  you[0].turnover = 1; break;
		case 'Z':
				  /* randart wpns */
				  if (you[0].equip [EQ_WEAPON] != -1 && you[0].inv_class [you[0].equip [EQ_WEAPON]] == 0 && you[0].inv_dam [you[0].equip [EQ_WEAPON]] % 30 >= 25)
					  if (randart_wpn_properties(you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], 0, RAP_PREVENT_SPELLCASTING) == 1)
					  {
						  mpr("Something is interfering with your hacking!");
						  break;
					  }
				  cast_a_spell(); break;
				  //		case 'M': which_spell(); break;      //memorise_spell(); break;
		case '\'': wield_weapon(1); break;
		case 'X':
				   if (you[0].level_type == 1 || you[0].level_type == 2)
				   {
					   mpr("You have no idea where you are!");
					   break;
				   }
				   plox [0] = 0;
				   show_map(plox);
				   redraw_screen();
				   break;
		case '\\': check_item_knowledge(); //nothing = check_item_knowledge();
				   redraw_screen();
				   break;
		case 16: replay_messages();
				 redraw_screen();
				 break;
		case '?': list_commands();
				  redraw_screen();
				  break;
		case 'C':
				  print_experience_info();
				  break;
		case '!':
				  yell(); /* in effects.cc */
				  break;
		case '@': display_char_status(); break;


		case 'm':
				  show_skills();
				  redraw_screen();
				  break;

				  //  case '^': disarm_trap(); break;

		case '#':
				  if (dump_char(0, substring(you[0].your_name, 6).c_str()) == 1)
					  mpr("Char dumped successfully.");
				  else
					  mpr("Char dump unsuccessful! Sorry about that.");
				  break;



#ifdef DEBUG
		case ')': you[0].xp += 500; you[0].exp_available += 500; you[0].xp_ch = 1;
				  level_change();
				  break;
		case '$': you[0].xp += 500; you[0].hp += 50; you[0].hp_max += 50; you[0].xp_ch = 1; you[0].hp_ch = 1; you[0].gp += 50;
				  you[0].exp_available += 500;
				  level_change();
				  break;
				  //  case '^': shop(); //in_a_shop(-1, you[0].inv_quant, you[0].inv_dam, you[0].inv_class, you[0].inv_type, you[0].inv_plus, you[0].inv_ident, you[0].equip [0], you[0].armour [0], you[0].armour [5], you[0].armour [2], you[0].armour [1], you[0].armour [3], you[0].armour [4], you[0].ring, it, igrid, you);
				  //  break;
		case '|': acquirement(250); break; //animate_dead(5, 7, you[0].pet_target, 1); break;
		case '-': you[0].hp = you[0].hp_max; you[0].hp_ch = 1; break;
		case '~': level_travel(); break;
		case '%': create_spec_object2(); break;
		case '+': create_spec_monster(); break;
		case '*': grd [you[0].x_pos] [you[0].y_pos] = 82; break;
				  //  case '#': grid [you[0].x_pos] [you[0].y_pos] = 100; break;
		case '(': grd [you[0].x_pos] [you[0].y_pos] = 99; break;
		case '_':
				  j = 0;
				  for (i = 0; i < 50; i ++)
				  {
					  j += you[0].skill_points [i];
				  }
				  msg("You have a total of @1 skill points.") << j;
				  break;
		case '`':
				  msg("@1") << OUTPUT_NO;
				  move_x = 0;
				  move_y = 0;
				  break;

		case '\'':
				  for (i = 0; i < ITEMS; i ++)
				  {
					  if (it[0].ilink [i] == ING) continue;
					  msg("@1 is linked to @2 c:@3 t:@4 p:@5 p2:@6 d:@7 q: @8") << i << it[0].ilink [i] << it[0].iclass [i] << it[0].itype [i] << it[0].iplus [i] << it[0].iplus2 [i] << it[0].idam [i] << it[0].iquant [i];
				  }
				  mpr("igrid:");
				  for (i = 0; i < GXM; i ++)
				  {
					  for (j = 0; j < GYM; j ++)
					  {
						  if (igrid [i] [j] != ING)
						  {
							  msg("@1 at @, @ c:@ t:@ p:@ p2:@ d:@ q: @") << igrid [i] [j] << i << j << it[0].iclass [igrid [i] [j]] << it[0].itype [igrid [i] [j]] << it[0].iplus [igrid [i] [j]] << it[0].iplus2 [igrid [i] [j]] << it[0].idam [igrid [i] [j]] << it[0].iquant [igrid [i] [j]];
						  }
					  }
				  }

				  break;

#endif

		case 'S': save_game(1); break;
		case 'Q': quit_game(); break;
		case 'v': version(); break;
		case '}':
				  //        Can't use this character
				  break;
		default: mpr("Unknown command.");

	}
}

/*
This function handles the player's input. It's called from main(), from inside
an endless loop.
*/
void input(void)
{
	int move_x = 0;
	int move_y = 0;

	you[0].time_taken = player_speed();

	window(1,1,80,25);
	textcolor(7);
	print_stats();

	int keyin = get_control(move_x, move_y);
	process_control(keyin, move_x, move_y);

	if (move_x != 0 || move_y != 0) {
		move(move_x, move_y);
	}
	if (you[0].turnover == 0) {
		viewwindow(1);
		return;
	}
	stealth = check_stealth();
	if (you[0].special_wield != 0) {
		special_wielded();
	}
	if (random2(10) == 0) {
		if ((player_teleport() > 0 && random2(100 / player_teleport()) == 0) || (you[0].level_type == 2 && random() % 30 == 0)) {
			you_teleport2(1); // this is instantaneous
		}
	}
	if (env[0].cgrid [you[0].x_pos] [you[0].y_pos] != CNG) {
		in_a_cloud();
	}
	if (you[0].duration [DUR_LIQUID_FLAMES] > 0) {
		you[0].duration [DUR_LIQUID_FLAMES] --;
	}
	if (you[0].duration [DUR_LIQUID_FLAMES] != 0)
	{
		mpr("You are covered in liquid flames!");
		scrolls_burn(8, 6);

		if (player_res_fire() > 100) {
			ouch((((random2(5) + random2(5) + 1) / 2 + (player_res_fire() - 100) * (player_res_fire() - 100)) * you[0].time_taken) / 10, 0, 17);
		}
		if (player_res_fire() <= 100) {
			ouch(((random2(5) + random2(5) + 1) * you[0].time_taken) / 10, 0, 17);
		}
		if (player_res_fire() < 100) {
			ouch(((random2(5) + random2(5) + 1) * you[0].time_taken) / 10, 0, 17);
		}
	}
	if (you[0].duration [DUR_ICY_ARMOUR] > 1) {
		you[0].duration [DUR_ICY_ARMOUR] --;
	}
	if (you[0].duration [DUR_ICY_ARMOUR] == 1) {
		mpr("Your icy armour evaporates.");
		you[0].AC_ch = 1;
		you[0].duration [DUR_ICY_ARMOUR] = 0;
	}
	if (you[0].duration [DUR_REPEL_MISSILES] > 0) {
		you[0].duration [DUR_REPEL_MISSILES] --;
		if (you[0].duration [DUR_REPEL_MISSILES] == 6) {
			mpr("Your repel missiles program is about to finish...");
			you[0].duration [DUR_REPEL_MISSILES] -= random2(2);
		}
	}
	if (you[0].duration [DUR_REPEL_MISSILES] == 1) {
		mpr("You feel less protected from missiles.");
		you[0].duration [DUR_REPEL_MISSILES] = 0;
	}
	if (you[0].duration [DUR_REPEL_MISSILES] > 0) {
		you[0].duration [DUR_DEFLECT_MISSILES] --;
		if (you[0].duration [DUR_DEFLECT_MISSILES] == 6) {
			mpr("Your deflect missiles program is about to finish...");
			you[0].duration [DUR_DEFLECT_MISSILES] -= random2(2);
		}
	}
	if (you[0].duration [DUR_DEFLECT_MISSILES] == 1) {
		mpr("You feel less protected from missiles.");
		you[0].duration [DUR_DEFLECT_MISSILES] = 0;
	}
	if (you[0].duration [DUR_REGENERATION] > 0) {
		you[0].duration [DUR_REGENERATION] --;
		if (you[0].duration [DUR_REGENERATION] == 6) {
			mpr("Your skin is crawling a little less now.");
			you[0].duration [DUR_REGENERATION] -= random2(2);
		}
		if (you[0].duration [DUR_REGENERATION] == 1) {
			mpr("Your skin stops crawling.");
			you[0].duration [DUR_REGENERATION] = 0;
			you[0].hunger_inc -= 4;
		}
	}
	if (you[0].duration [DUR_PRAYER] > 0) {
		you[0].duration [DUR_PRAYER] --;
	}
	if (you[0].duration [DUR_PRAYER] == 1) {
		mpr("Your communication is over.");
		you[0].duration [DUR_PRAYER] = 0;
	}
	if (you[0].duration [DUR_VORPAL_BLADE] > 0) {
		you[0].duration [DUR_VORPAL_BLADE] --;
	}
	if (you[0].duration [DUR_VORPAL_BLADE] == 1) {
		msg("@1 seems blunter.") << in_name(you[0].equip [EQ_WEAPON], 4);
		you[0].duration [DUR_VORPAL_BLADE] = 0;
		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 10;
		wield_change = 1;
	}
	if (you[0].duration [DUR_FIRE_BRAND] > 0) {
		you[0].duration [DUR_FIRE_BRAND] --;
	}
	if (you[0].duration [DUR_FIRE_BRAND] == 1) {
		msg("@1 goes out.") << in_name(you[0].equip [EQ_WEAPON], 4);
		you[0].duration [DUR_FIRE_BRAND] = 0;
		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 1;
		wield_change = 1;
	}
	if (you[0].duration [DUR_ICE_BRAND] > 0) {
		you[0].duration [DUR_ICE_BRAND] --;
	}
	if (you[0].duration [DUR_ICE_BRAND] == 1) {
		msg("@1 stops glowing.") << in_name(you[0].equip [EQ_WEAPON], 4);
		you[0].duration [DUR_ICE_BRAND] = 0;
		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 2;
		wield_change = 1;
	}
	if (you[0].duration [DUR_LETHAL_INFUSION] > 0) {
		you[0].duration [DUR_LETHAL_INFUSION] --;
	}
	if (you[0].duration [DUR_LETHAL_INFUSION] == 1) {
		msg("@1 stops crackling.") << in_name(you[0].equip [EQ_WEAPON], 4);
		you[0].duration [DUR_LETHAL_INFUSION] = 0;
		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 8;
		wield_change = 1;
	}
	if (you[0].duration [DUR_POISON_WEAPON] > 0) {
		you[0].duration [DUR_POISON_WEAPON] --;
	}
	if (you[0].duration [DUR_POISON_WEAPON] == 1) {
		msg("@1 stops dripping with poison.") << in_name(you[0].equip [EQ_WEAPON], 4);
		you[0].duration [DUR_POISON_WEAPON] = 0;
		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 6;
		wield_change = 1;
	}

	if (you[0].duration [DUR_BREATH_WEAPON] > 0) {
		you[0].duration [DUR_BREATH_WEAPON] --;
	}
	if (you[0].duration [DUR_BREATH_WEAPON] == 1) {
		mpr("You have got your breath back.");
		you[0].duration [DUR_BREATH_WEAPON] = 0;
	}

	if (you[0].duration [DUR_TRANSFORMATION] > 0) {
		you[0].duration [DUR_TRANSFORMATION] --;
		if (you[0].duration [DUR_TRANSFORMATION] == 10) {
			mpr("Your transformation is almost over.");
			you[0].duration [DUR_TRANSFORMATION] -= random2(3);
		}
	}
	if (you[0].duration [DUR_TRANSFORMATION] == 1) {
		untransform();
		you[0].duration [DUR_BREATH_WEAPON] = 0;
	}
	if (you[0].duration [DUR_SWIFTNESS] > 0) {
		you[0].duration [DUR_SWIFTNESS] --;
		if (you[0].duration [DUR_SWIFTNESS] == 6) {
			mpr("You start to feel a little slower.");
			you[0].duration [DUR_SWIFTNESS] -= random2(2);
		}
	}
	if (you[0].duration [DUR_SWIFTNESS] == 1) {
		mpr("You feel sluggish.");
		you[0].duration [DUR_SWIFTNESS] = 0;
	}
	if (you[0].duration [DUR_INSULATION] > 0) {
		you[0].duration [DUR_INSULATION] --;
		if (you[0].duration [DUR_INSULATION] == 6) {
			mpr("You start to feel a little less insulated.");
			you[0].duration [DUR_INSULATION] -= random2(2);
		}
	}
	if (you[0].duration [DUR_INSULATION] == 1) {
		mpr("You feel conductive.");
		you[0].duration [DUR_INSULATION] = 0;
		you[0].attribute [ATTR_RESIST_LIGHTNING] --;
	}
	if (you[0].duration [DUR_STONEMAIL] > 1) {
		you[0].duration [DUR_STONEMAIL] --;
		if (you[0].duration [DUR_STONEMAIL] == 6) {
			mpr("Your scaley stone armour is starting to flake away.");
			you[0].duration [DUR_STONEMAIL] -= random2(2);
		}
	}
	if (you[0].duration [DUR_STONEMAIL] == 1) {
		mpr("Your scaley stone armour disappears.");
		you[0].duration [DUR_STONEMAIL] = 0;
		burden_change();
	}
	if (you[0].duration [DUR_TELEPORT] > 1) {
		you[0].duration [DUR_TELEPORT] --;
	}
	if (you[0].duration [DUR_TELEPORT] == 1) {
		you_teleport2(1);
		you[0].duration [DUR_TELEPORT] = 0;
	}
	if (you[0].duration [DUR_CONTROL_TELEPORT] > 1) {
		you[0].duration [DUR_CONTROL_TELEPORT] --;
		if (you[0].duration [DUR_CONTROL_TELEPORT] == 6) {
			mpr("You start to feel a little uncertain.");
			you[0].duration [DUR_CONTROL_TELEPORT] -= random2(2);
		}
	}
	if (you[0].duration [DUR_CONTROL_TELEPORT] == 1) {
		mpr("You feel uncertain.");
		you[0].duration [DUR_CONTROL_TELEPORT] = 0;
		you[0].attribute [ATTR_CONTROL_TELEPORT] --;
	}
	if (you[0].duration [DUR_RESIST_POISON] > 1) {
		you[0].duration [DUR_RESIST_POISON] --;
		if (you[0].duration [DUR_RESIST_POISON] == 6) {
			mpr("Your poison resistance is about to expire.");
			you[0].duration [DUR_RESIST_POISON] -= random2(2);
		}
	}
	if (you[0].duration [DUR_RESIST_POISON] == 1) {
		mpr("Your poison resistance expires.");
		you[0].duration [DUR_RESIST_POISON] = 0;
	}
	if (you[0].duration [DUR_DEATH_CHANNEL] > 1) {
		you[0].duration [DUR_DEATH_CHANNEL] --;
		if (you[0].duration [DUR_DEATH_CHANNEL] == 6) {
			mpr("Your unholy channel is weakening.");
			you[0].duration [DUR_DEATH_CHANNEL] -= random2(2);
		}
	}
	if (you[0].duration [DUR_DEATH_CHANNEL] == 1) {
		mpr("Your unholy channel expires.");
		you[0].duration [DUR_DEATH_CHANNEL] = 0;
	}
	if (you[0].duration [DUR_LIQUID_FLAMES] > 0) {
		you[0].duration [DUR_LIQUID_FLAMES] --;
	}
	if (you[0].duration [DUR_TELEPORT] > 1) {
		you[0].duration [DUR_TELEPORT] --;
	}
	if (you[0].duration [DUR_TELEPORT] == 1) {
		you_teleport2(1);
		you[0].duration [DUR_TELEPORT] = 0;
	}
	if (you[0].invis > 1) {
		you[0].invis --;
		if (you[0].hunger >= 40 && you[0].is_undead != 2) {
			you[0].hunger -= 5;
		}
		if (you[0].invis == 6) {
			mpr("You flicker for a moment.");
			you[0].invis -= random2(2);
		}
	}
	if (you[0].invis == 1) {
		mpr("You flicker back into view.");
		you[0].invis = 0;
	}
	if (you[0].conf > 1) {
		you[0].conf --;
	}
	if (you[0].conf == 1) {
		mpr("You feel steadier.");
		you[0].conf = 0;
	}
	if (you[0].paralysis > 1) {
		you[0].paralysis--;
	}
	if (you[0].paralysis == 1) {
		mpr("You can move again.");
		you[0].paralysis = 0;
	}
	if (you[0].slow > 1) {
		you[0].slow--;
	}
	if (you[0].slow == 1) {
		mpr("You feel yourself speed up.");
		you[0].slow = 0;
	}
	if (you[0].haste > 1) {
		you[0].haste--;
		if (you[0].haste == 6) {
			mpr("Your extra speed is starting to run out.");
			you[0].haste -= random2(2);
		}
	}
	if (you[0].haste == 1) {
		mpr("You feel yourself slow down.");
		you[0].haste = 0;
	}
	if (you[0].might > 1) {
		you[0].might--;
	}
	if (you[0].might == 1) {
		mpr("You feel a little less mighty now.");
		you[0].might = 0;
		you[0].strength -= 5;
		you[0].max_strength -= 5;
		you[0].strength_ch = 1;
	}
	if (you[0].berserker > 1) {
		you[0].berserker--;
	}
	if (you[0].berserker == 1) {
		mpr("You went out battle mode.");
		mpr("You feel exhausted.");
		you[0].berserker = 0;
		you[0].slow += 4 + random2(4) + random2(4);
		you[0].hunger -= 700;
		if (you[0].hunger <= 50) {
			you[0].hunger = 50;
		}
		calc_hp();
		you[0].hp_ch = 1;
	}
	if (you[0].lev > 1) {
		if (you[0].species != SP_KENKU || you[0].xl < 15) {
			you[0].lev--;
		}
		if (you[0].lev == 10) {
			mpr("You are starting to lose your buoyancy!");
			you[0].lev -= random2(6);
			if (you[0].duration [DUR_CONTROLLED_FLIGHT] > 0) you[0].duration [DUR_CONTROLLED_FLIGHT] = you[0].lev;
		}
	}
	if (you[0].lev == 1) {
		mpr("You float gracefully downwards.");
		you[0].lev = 0;
		burden_change();
		you[0].duration [DUR_CONTROLLED_FLIGHT] = 0;
		if (grd [you[0].x_pos] [you[0].y_pos] == 61 || grd [you[0].x_pos] [you[0].y_pos] == 62) fall_into_a_pool(1, grd [you[0].x_pos] [you[0].y_pos]);
	}
	if (you[0].rotting > 0 || (you[0].species == SP_GHOUL && random2(2) == 0)) {
		if (you[0].species == SP_MUMMY) {
			you[0].rotting = 0;
		} else if (random2(20) <= (you[0].rotting - 1) || (you[0].species == SP_GHOUL && random2(200) == 0)) {
			ouch(1, 0, 21);
			you[0].hp_max --;
			you[0].base_hp --;
			you[0].hp_ch = 1;
			mpr("You feel your flesh rotting away.");
			you[0].rotting--;
		}
	}
	if (you[0].disease > 0) {
		you[0].disease --;
		if (you[0].species == SP_KOBOLD && you[0].disease > 5) {
			you[0].disease -= 2;
		}
		if (you[0].disease == 0) {
			mpr("You feel your health improve.");
		}
	}
	if (you[0].poison > 0) {
		if (random2(5) <= (you[0].poison - 1)) {
			if (you[0].poison > 10 && random2(you[0].poison) >= 8) {
				ouch(random2(10) + 5, 0, 1);
				you[0].hp_ch = 1;
				mpr("You feel extremely sick.");
			} else if (you[0].poison > 5 && random2(2) == 0) {
				ouch(random2(2) + 2, 0, 1);
				you[0].hp_ch = 1;
				mpr("You feel very sick.");
			} else {
				ouch(1, 0, 1);
				you[0].hp_ch = 1;
				mpr("You feel sick.");
			}
			if (random2(8) == 0 || (you[0].hp == 1 && random2(3) == 0)) {
				mpr("You feel a little better.");
				you[0].poison--;
			}
		}
	}
	if (you[0].deaths_door > 0) {
		if (you[0].hp > you[0].skills [SK_NECROMANCY] + (you[0].religion == GOD_KIKUBAAQUDGHA) * 13) {
			mpr("Your life is in your own hands once again.");
			you[0].paralysis += 5 + random2(5);
			you[0].conf += 10 + random2(10);
			you[0].hp_max --;
			if (you[0].hp > you[0].hp_max) {
				you[0].hp = you[0].hp_max;
			}
			you[0].hp_ch = 1;
			you[0].deaths_door = 0;
		} else {
			you[0].deaths_door --;
		}
		if (you[0].deaths_door == 10) {
			mpr("Your time is quickly running out!");
			you[0].deaths_door -= random2(6);
		}
		if (you[0].deaths_door == 1) {
			mpr("Your life is in your own hands again!");
			more();
			you[0].hp_ch = 1;
		}
	}
	if (you[0].is_undead != 2) {
		if (you[0].hunger_inc > 0 && you[0].hunger >= 40) {
			you[0].hunger -= you[0].hunger_inc;
			you[0].hunger -= you[0].burden_state;
		}
	} else {
		you[0].hunger = 6000;
	}

	if (you[0].hp < you[0].hp_max && you[0].disease == 0 && you[0].deaths_door == 0) {
		you[0].incr_regen += player_regen();
	}
	if (you[0].ep < you[0].ep_max) {
		you[0].ep_incr_regen += 7 + you[0].ep_max / 2;
	}
	while(you[0].incr_regen >= 100) {
		if (you[0].hp == you[0].hp_max - 1 && you[0].running != 0 && you[0].run_x == 0 && you[0].run_y == 0) you[0].running = 0;
		you[0].hp ++;
		if (you[0].hp > you[0].hp_max) {
			you[0].hp = you[0].hp_max;
		}
		you[0].incr_regen -= 100;
		you[0].hp_ch = 1;
	}

	while(you[0].ep_incr_regen >= 100) {
		if (you[0].ep == you[0].ep_max - 1 && you[0].running != 0 && you[0].run_x == 0 && you[0].run_y == 0) {
			you[0].running = 0;
		}
		you[0].ep ++;
		if (you[0].ep > you[0].ep_max) {
			you[0].ep = you[0].ep_max;
		}
		you[0].ep_incr_regen -= 100;
		you[0].ep_ch = 1;
	}

	viewwindow(0);

	monster();

	if (you[0].corpse_count <= you[0].time_taken) {
		manage_corpses();
		you[0].corpse_count = 200;
		if (random2(50) == 0) {
			cull_items();
		}
	} else {
		you[0].corpse_count -= you[0].time_taken;
	}

	env[0].cloud_no = manage_clouds();
	if (you[0].shock_shield > 0) {
		manage_shock_shield();
	}

	/*
	   If visible [0] is 1, at least one statue is visible.
	   This was supposed to be more complex (with values of 2 etc), but I
	   couldn't get it to work.
	   */
	if (visible [0] != 0) {
		if (visible [1] != 0) {
			switch(visible [1]) {
				case 0: break;
				case 2:
						if (random2(4) == 0) {
							msg("The silver statue's eyes glow a @1 colour.") << weird_colours(random2(200));
							create_monster(summon_any_demon(random2(2)), 25, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
						}
						break;
			}
			visible [1] --;
		}
		if (visible [2] != 0) {
			if (random2(3) == 0) {
				mpr("A hostile presence attacks your cyberbrain!");
				miscast_effect(SPTYP_DIVINATION, random2(15), random2(150), 100);
			}
			visible [2] --;
		}
		visible [0] --;
	}
	if (you[0].hunger <= 500) {
		if (you[0].paralysis == 0 && random2(40) == 0) {
			mpr("You lose consciousness!");
			you[0].paralysis += random2(8) + 5;
			if (you[0].paralysis > 13) {
				you[0].paralysis = 13;
			}
		}
		if (you[0].hunger <= 100) {
			mpr("You have starved to death.");
			ouch(-9999, 0, 15);
		}
	}
	if (you[0].hung_state == 3 && you[0].hunger <= 2600) {
		mpr("You are feeling hungry.");
		you[0].hung_state = 2;
		you[0].hung_ch = 1;
	}
	if (you[0].hung_state == 2 && you[0].hunger <= 1000) {
		mpr("You are starving!");
		you[0].hung_state = 1;
		you[0].hung_ch = 1;
	}
	if (you[0].hung_state == 4 && you[0].hunger < 7000) {
		you[0].hung_state = 3;
		you[0].hung_ch = 1;
	}
	if (you[0].hung_state == 5 && you[0].hunger < 11000) {
		you[0].hung_state = 4;
		you[0].hung_ch = 1;
	}

	_setcursortype(_NOCURSOR);
	viewwindow(1);
	_setcursortype(_NORMALCURSOR);
	if (you[0].paralysis > 0) {
		more();
	}
	if (you[0].level_type != 0) {
		switch(you[0].level_type) {
			case 1: break; /* labyrinth */
			case 2: if (random2(5) == 0) mons_place(2500, 0, 50, 50, 1, MHITNOT, 250, 51); break; /* Abyss  */
			case 3: if (random2(50) == 0) pandemonium_mons(); break; /* Pandemonium */
		}
	} else if (random2(240) == 0 && you[0].level_type != 1 && you[0].where_are_you != 18) {
		mons_place(2500, 0, 50, 50, 1, MHITNOT, 250, you[0].your_level);
	}
}

/*
Opens doors and handles some aspects of untrapping. If move_x != 100, it
carries a specific direction for the door to be opened (eg if you type
ctrl - dir).
*/
void open_door(int move_x, int move_y)
{
	struct dist door_move [1];

	door_move[0].move_x = move_x;
	door_move[0].move_y = move_y;


	if(move_x != 100) {
		int new_pos_x = you[0].x_pos + door_move[0].move_x;
		int new_pos_y = you[0].y_pos + door_move[0].move_y;
		int dest_cell = env[0].mgrid [new_pos_x] [new_pos_y];

		if (dest_cell != MNG && (menv[dest_cell].m_class < MLAVA0 || menv [dest_cell].m_sec == 0)) {
			you_attack(mgrd [new_pos_x] [new_pos_y]);
			you[0].turnover = 1;
			return;
		}
		if (grd [new_pos_x] [new_pos_y] >= 75 && grd [new_pos_x] [new_pos_y] <= 77) {
			if (env[0].cgrid [new_pos_x] [new_pos_y] != CNG) {
			 mpr("You can't get to that trap right now.");
			 return;
			}
			disarm_trap(door_move);
			return;
		}
	}

	if (move_x == 100) {
 		door_move[0].move_x = 0;
 		door_move[0].move_y = 0;
 		mpr("Which direction?");
 		direction(0, door_move);
	}
	if (door_move[0].nothing == -1) return;

	if (door_move[0].move_x > 1 || door_move[0].move_y > 1 || door_move[0].move_x < -1 || door_move[0].move_y < -1) {
		mpr("I'm afraid your arm isn't that long.");
		return;
	}

	int new_pos_x = you[0].x_pos + door_move[0].move_x;
	int new_pos_y = you[0].y_pos + door_move[0].move_y;

	if (grd [new_pos_x] [new_pos_y] == 3) {
		if (you[0].lev != 0) {
			mpr("You reach down and open the door.");
		} else {
			if (random2(25) == 0) {
				mpr("As you open the door, it creaks loudly!");
                noisy(15, you[0].x_pos, you[0].y_pos);
			} else {
				mpr("You open the door.");
			}
		}
		grd [new_pos_x] [new_pos_y] = 70;
	} else {
		mpr("You swing at nothing.");
	 	if (you[0].is_undead != 2) you[0].hunger -= 3;
	}
	you[0].turnover = 1;
} // end of void open_door()

/*
Similar to open_door. Can you spot the difference?
*/
void close_door(int door_x, int door_y)
{
	struct dist door_move [1];
	door_move[0].move_x = door_x;
	door_move[0].move_y = door_y;

	if (door_move[0].move_x == 100) {
		door_move[0].move_x = 0;
		door_move[0].move_y = 0;
		mpr("Which direction?");
		direction(0, door_move);
	}

	if (door_move[0].move_x > 1 || door_move[0].move_y > 1) {
		mpr("I'm afraid your arm isn't that long.");
		return;
	}

	if (door_move[0].move_x == 0 && door_move[0].move_y == 0) {
		mpr("You can't close doors on yourself!");
		return;
	}

	int new_pos_x = you[0].x_pos + door_move[0].move_x;
	int new_pos_y = you[0].y_pos + door_move[0].move_y;

	if (env[0].mgrid [new_pos_x] [new_pos_y] != MNG) {
		mpr("There's a creature in the doorway!");
		return;
	}

	if (grd [new_pos_x] [new_pos_y] == 70) {
		if (env[0].igrid [new_pos_x] [new_pos_y] != 501) {
			mpr("There's something blocking the doorway.");
			return;
		}

		if (you[0].lev != 0) {
			mpr("You reach down and close the door.");
		} else {
			if (random2(25) == 0) {
				mpr("As you close the door, it creaks loudly!");
				noisy(15, you[0].x_pos, you[0].y_pos);
			} else {
				mpr("You close the door.");
			}
		}
		grd [new_pos_x] [new_pos_y] = 3;
		you[0].turnover = 1;
	} else {
		mpr("There isn't anything that you can close there!");
	}
} // end of void open_door()

void init_system()
{
	textbackground(0);
	you[0].your_level = 0;
	srandom(time(NULL));
	clrscr();
}

void init_structures()
{
	your_sign = '@';
	your_colour = LIGHTGREY;

	for(int i = 0; i < NO_EQUIP; i++) {
		you[0].equip [NO_EQUIP] = -1;
	}

	for(int i = 1; i < ITEMS; i++) {
		env[0].it[0].iclass [i] = 0;
		env[0].it[0].itype [i] = 0;
		env[0].it[0].ix [i] = 1;
		env[0].it[0].iy [i] = 1;
		env[0].it[0].iquant [i] = 0;
		env[0].it[0].idam [i] = 0;
		env[0].it[0].iplus [i] = 0;
		env[0].it[0].iplus2 [i] = 0;
		env[0].it[0].ilink [i] = ING;
	}

	for(int i = 0; i < MNST; i++) {
		env[0].mons[i].m_class = -1;
		env[0].mons[i].m_speed_inc = 10;
		env[0].mons[i].m_targ_1_x = 155;
		env[0].mons[i].m_ench_1 = 0;
		env[0].mons[i].m_beh = 0; // sleeping
		env[0].mons[i].m_hit = MNG; // nothing
		for(int j = 0; j < 3; j++) {
			env[0].mons[i].m_ench [j] = 0;
		}
		for(int j = 0; j < 8; j++) {
			env[0].mons[i].m_inv [j] = ING;
		}
		env[0].mons[i].m_sec = 0;
	}

	for(int i = 0; i < GXM; i++) {
		for(int j = 0; j < GYM; j++) {
			env[0].igrid [i] [j] = ING;
			env[0].mgrid [i] [j] = MNG;
			env[0].map [i] [j] = 0;
		}
	}

	for(int i = 0; i < 52; i++) {
		you[0].inv_quant [i] = 0;
	}

	for(int i = 0; i < 25; i++) {
		you[0].spells [i] = 210;
	}

	for(int i = 0; i < 10; i ++) {
		visible [i] = 0;
	}
	you[0].prev_targ = MHITNOT;

}

void setup_game()
{
	int newc = new_game();
	if(newc == 0) restore_game();

	calc_hp();
	calc_ep();

	you[0].inv_no = 0;
	for(int i = 0; i < 52; i ++) {
	 	if (you[0].inv_quant [i] != 0) you[0].inv_no ++;
	}

	int just_made_new_lev = (newc == 0) ? 1 : 0;
	int moving_level = (newc == 1) ? 1 : 0;

	load(82, moving_level, 0, 0, 0, just_made_new_lev, you[0].where_are_you);

	moving_level = 0;
	just_made_new_lev = 0;
	newc = 0;

	mon_init(gmon_use, mcolour);
	init_properties();
	if(newc == 1) {
		for(int i = 0; i < GXM; i ++) {
			int j;
			for(j = 0; j < GYM; j ++) {
				if (grd [i] [j] == 68) {
					you[0].x_pos = i;
					you[0].y_pos = j;
				}
			}
			if(grd [i] [j] == 67) break;
		}

		burden_change();
		food_change();
		new_level(); // - must come after mon_init
	} // end if newc

	you[0].hp_ch = 1;
	you[0].ep_ch = 1;
	you[0].strength_ch = 1;
	you[0].intel_ch = 1;
	you[0].dex_ch = 1;
	you[0].AC_ch =1;
	you[0].evasion_ch = 1;
	you[0].xp_ch = 1;
	you[0].gp_ch = 1;
	you[0].hung_ch = 1;
	wield_change = 1;

	draw_border(BROWN, you[0].your_name, skill_title(best_skill(0, 50, 99), you[0].skills [best_skill(0, 50, 99)], you[0].clas, you[0].xl).c_str(), you[0].species);

	new_level();
	viewwindow(1); // This just puts the view up for the first turn.
	item();
}

void initialise()
{
	init_system();
	init_structures();
	setup_game();
}

bool act_confused(int move_x, int move_y)
{
	if (random2(3) != 0) {
		move_x = random2(3) - 1;
		move_y = random2(3) - 1;
	}
	
	int new_pos_x = you[0].x_pos + move_x;
	int new_pos_y = you[0].y_pos + move_y;

	if (grd [new_pos_x] [new_pos_y] < MINMOVE) {
		you[0].turnover = 1;
		mpr("Ouch!");
		return true;
	}

	if ((grd [new_pos_x] [new_pos_y] == 61 || grd [new_pos_x] [new_pos_y] == 62) && you[0].lev == 0) {
		fall_into_a_pool(0, grd [new_pos_x] [new_pos_y]);
		you[0].turnover = 1;
		return true;
	}
	return false;
}

int mng_attack(int cell)
{
	if (cell != MNG) {
		if (menv[cell].m_class >= MLAVA0 && menv[cell].m_sec == 1) {
			return 0;
		}
		if (menv[cell].m_beh == 7 && menv[cell].m_ench [2] != 6 && you[0].conf == 0) {
			swap_places(cell);
			return 0;
		}
		you_attack(cell);
		you[0].turnover = 1;
		return 1;
	}
	return 0;
}

bool move_to_a_pool(int new_pos_x, int new_pos_y, int attacking)
{
	if ((grd [new_pos_x] [new_pos_y] == 61 || grd [new_pos_x] [new_pos_y] == 62) && attacking == 0 && you[0].lev == 0) {
		mpr("Do you really want to step there?");
		int stepping = get_ch();
		if (stepping == 'y' || stepping == 'Y') {
			fall_into_a_pool(0, grd [new_pos_x] [new_pos_y]);
			you[0].turnover = 1;
			return true;
		}
		mpr("Okay, then.");
		return true;
	}
	return false;
}

bool move_to_a_trap(int new_pos_x, int new_pos_y)
{
	if (grd [new_pos_x] [new_pos_y] == 78 && random() % (you[0].skills [SK_TRAPS_DOORS] + 1) > 3) {
		msg("Wait a moment, @1! Do you really want to step there?") << you[0].your_name;
		more();
		you[0].turnover = 0;
		int i;
		for (i = 0; i < NTRAPS; i ++) {
			if (env[0].trap_x [i] == new_pos_x && env[0].trap_y [i] == new_pos_y) break;
		}
		if (env[0].trap_type [i] < 4 || env[0].trap_type [i] == 6 || env[0].trap_type [i] == 7) grd [new_pos_x] [new_pos_y] = 75;
		if (env[0].trap_type [i] == 4 || env[0].trap_type [i] == 5 || env[0].trap_type [i] == 8) grd [new_pos_x] [new_pos_y] = 76;
		return true;
	}
	return false;
}

void move_in_water(int move_x, int move_y)
{
	if (grd [you[0].x_pos] [you[0].y_pos] == 65 && you[0].lev == 0) {
		if (random2(3) == 0) {
			mpr("Splash!");
			noisy(10, you[0].x_pos, you[0].y_pos);
		}
		you[0].time_taken *= 13 + random2(8);
		you[0].time_taken /= 10;
		if (grd [you[0].x_pos - move_x] [you[0].y_pos - move_y] != 65) {
			mpr("You enter the shallow water. Moving in this stuff is going to be slow.");
			if (you[0].invis != 0) {
				mpr("And don't expect to remain undetected.");
			}
		}
	}
}

void step_into_a_trap()
{
	int trap_known;
	struct bolt beam [1];

	if (grd [you[0].x_pos] [you[0].y_pos] > 74 && grd [you[0].x_pos] [you[0].y_pos] < 79) {
		int trap_index;
		for (trap_index = 0; trap_index < NTRAPS; trap_index ++) {
			if (env[0].trap_x [trap_index] == you[0].x_pos && env[0].trap_y [trap_index] == you[0].y_pos) break;
		}
		int trap_type = env[0].trap_type [trap_index];
		if (grd [you[0].x_pos] [you[0].y_pos] == 78) {
			if (trap_type < 4 || trap_type == 6 || trap_type == 7) {
				grd [you[0].x_pos] [you[0].y_pos] = 75;
			}
			if (trap_type == 4 || trap_type == 5 || trap_type == 8) {
				grd [you[0].x_pos] [you[0].y_pos] = 76;
			}
			trap_known = 0;
		} else {
			trap_known = 1;
		}

		if (you[0].lev != 0 && (trap_type < 4 || trap_type == 6 || trap_type == 7)) {
			return;
		}

		int weapon;
		switch(trap_type) {
			case 0:
				beam[0].beam_name = " dart";
				beam[0].damage = 4;
				dart_trap(trap_known, trap_index, beam);
				break;
			case 1:
				beam[0].beam_name = "n arrow";
				beam[0].damage = 7;
				dart_trap(trap_known, trap_index, beam);
				break;
			case 2:
				beam[0].beam_name = " spear";
				beam[0].damage = 10;
				dart_trap(trap_known, trap_index, beam);
				break;
			case 3:
				beam[0].beam_name = "n axe";
				beam[0].damage = 15;
				dart_trap(trap_known, trap_index, beam);
				break;
			case 4:
				mpr("You enter a ventilation shaft trap!");
				weapon = you[0].equip [EQ_WEAPON];
				if (weapon != -1 && you[0].inv_class [weapon] == 0 && you[0].inv_dam [weapon] % 30 >= 25) {
					if (randart_wpn_properties(you[0].inv_class [weapon], you[0].inv_type [weapon], you[0].inv_dam [weapon], you[0].inv_plus [weapon], you[0].inv_plus2 [weapon], 0, RAP_PREVENT_TELEPORTATION) > 0) {
						mpr("You feel a weird sense of stasis.");
						break;
					}
				}
				you_teleport2(1);
				break;
			case 5:
				mpr("You feel momentarily disoriented.");
				forget_map(random2(50) + random2(50) + 2);
				break;
			case 7:
				beam[0].beam_name = " bolt";
				beam[0].damage = 13;
				dart_trap(trap_known, trap_index, beam);
				break;
			default: handle_traps(trap_type, trap_known); break;
		}
	}
}

/*
Called when the player moves by walking/running. Also calls attack function
and trap function etc when necessary.
*/
void move(int move_x, int move_y)
{
	//int i;
	if (you[0].conf > 0) {
		if(act_confused(move_x, move_y)) {
			return;
		}
	}

	int new_pos_x = you[0].x_pos + move_x;
	int new_pos_y = you[0].y_pos + move_y;

	if (you[0].running > 0 && you[0].running != 2 && grd [new_pos_x] [new_pos_y] != 67 && grd [new_pos_x] [new_pos_y] != 78) {
		you[0].running = 0;
		you[0].turnover = 0;
		return;
	}

	int attacking = mng_attack(mgrd [new_pos_x] [new_pos_y]);

	if(move_to_a_pool(new_pos_x, new_pos_y, attacking)) {
		return;
	}

	if (attacking == 0 && (grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] >= MINMOVE)) {
		if(move_to_a_trap(new_pos_x, new_pos_y)) {
			return;
		}

		you[0].x_pos += move_x;
		you[0].y_pos += move_y;

		move_in_water(move_x, move_y);

		move_x = 0;
		move_y = 0;
		if (player_fast_run() != 0) {
			you[0].time_taken *= 6;
			you[0].time_taken /= 10;
		}
		if (you[0].attribute [ATTR_WALK_SLOWLY] != 0) {
			you[0].time_taken *= 14;
			you[0].time_taken /= 10;
		}
		you[0].turnover = 1;
		item_check(0);

		step_into_a_trap();
	}
	if (grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] <= MINMOVE) {
		move_x = 0;
		move_y = 0;
		you[0].running = 0;
		you[0].turnover = 0;
	}


	if (you[0].running == 2) {
		you[0].running = 1;
	}

	if (you[0].level_type == 2 && (you[0].x_pos <= 21 || you[0].x_pos >= 61 || you[0].y_pos <= 15 || you[0].y_pos >= 54)) {
		env[0].cloud_no = area_shift();
		you[0].pet_target = MHITNOT;
	}
}

