#include "config.h"

//#define DEBUG

/*
Sub-Crawl 1.0
*/
/* contains the player struct: */
//#include "struct.h"

#ifdef DOS
#include <conio.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <string.h>

#ifdef DOS
#include <file.h>
#endif

#ifdef USE_EMX
#include <sys/types.h>
#endif

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include "externs.h"
#include "enum.h"


#include "ability.h"
//#include "beam.h"
#include "chardump.h"
#include "command.h"
#include "debug.h"
//#include "decks.h"
#include "describe.h"
#include "direct.h"
#include "dungeon.h"
#include "effects.h"
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
//#include "maps.h"
#include "macro.h"

#include "message.h"
#include "misc.h"
#include "monplace.h"
#include "monstuff.h"
#include "mstruct.h"
//#include "mstuff2.h"
#include "mutation.h"
#include "newgame.h"
#include "ouch.h"
#include "output.h"
#include "player.h"
#include "priest.h"
#include "randart.h"
#include "religion.h"
//#include "shopping.h"
#include "skills.h"
#include "skills2.h"
#include "spell.h"
#include "spells.h"
//#include "spells1.h"
#include "spells2.h"
#include "spells3.h"
#include "stuff.h"
#include "transfor.h"
#include "view.h"

/*
#include "player.h"
#include "message.h"
#include "output.h"
#include "view.h"
#include "newgame.h"
#include "items.h"
#include "files.h"
#include "itemname.h"
#include "fight.h"
#include "mstruct.h"
#include "item_use.h"
#include "it_use2.h"
#include "ouch.h"
#include "spells1.h"
#include "invent.h"
#include "direct.h"
#include "skills.h"
#include "skills2.h"
#include "player.h"
#include "chardump.h"
#include "misc.h"
#include "newgame.h"
*/
/* Functions in main module */
void input(void);
void open_door(char move_x, char move_y);
void close_door(char move_x, char move_y);
void move(char move_x, char move_y);

//void missile(struct player you [1], struct environ env [1], struct bolt beam [1], int throw_2);
//void beam(struct player you [1], struct environ env [1], struct bolt beam [1]);


void mpr(char inf [200]);
void more(void);
void initialise(void);



struct environ env [1];
struct player you [1];
char info [200];
char st_prn [20];

char str_pass [80];
char gmon_use [1000];

int stealth; /* externed in view.h */

char use_colour = 1;

char visible [10];

/*

Functions needed:
New:
int player_speed(player you);
mpr(char message []);
hit_player(damage, flavour, then last two ouch values, env);


Old:
print_stats(player you);
wield(player you);
show_map
noisy()
losight

*/

void (*viewwindow)(char);
/* these are all defined in view.cc: */
extern unsigned char (*mapch)(unsigned char);
extern unsigned char (*mapch2)(unsigned char);
unsigned char mapchar(unsigned char ldfk);
unsigned char mapchar2(unsigned char ldfk);
unsigned char mapchar3(unsigned char ldfk);
unsigned char mapchar4(unsigned char ldfk);
/*
Function pointers are used to make switching between Linux and DOS char sets
possible as a runtime option (command-line -c)
*/

extern unsigned char your_sign; /* these two are defined in view.cc. What does the player look like? (changed for shapechanging */
extern unsigned char your_colour;

extern char wield_change; /* defined in output.cc */

/*
It all starts here. Some initialisations are run first, then straight to
new_game and then input.
*/
int main(int argc, char *argv[])
{
#ifdef LINUX
	lincurses_startup();
#endif

#ifdef MACROS
	macro_init();
#endif
	

	viewwindow = &viewwindow2;
	mapch = &mapchar;
	mapch2 = &mapchar2;

	if (argc > 1)
	{
 		if (stricmp(argv [1], "-c") == 0 || stricmp(argv [1], "-nc") == 0)
 		{
  			viewwindow = &viewwindow3;
  			mapch = &mapchar3;
  			mapch2 = &mapchar4;
  			if (stricmp(argv [1], "-nc") == 0)
  			{
   				use_colour = 0; /* this is global to this function, so can either be
	 		      passed eg to lincurses_startup or defined as an
			      extern in another module */
  			}
 		}
  		else
  		{
   			cprintf(EOL"Crawl accepts the following arguments only:"EOL);
   			cprintf(" -c   Use non-ibm character set"EOL);
   			cprintf(" -nc  Use non-ibm character set, but no colour"EOL);
   			cprintf(EOL"Any others will cause this message to be printed again."EOL);
   			end(0);
  		}
	}

	{ // TODO This block is a workaround to display correct symbols. Works like '-c' arg.
		viewwindow = &viewwindow3;
		mapch = &mapchar3;
		mapch2 = &mapchar4;
	}

//new_game();

	initial();
	initialise();

	while (TRUE)
	{
		input();
//	cprintf("x");
	}
/* Should never reach this stage, right? */
#ifdef LINUX
    lincurses_shutdown();
#endif

#ifdef USE_EMX
  	deinit_emx();
#endif


	return 0;
}



/*
This function handles the player's input. It's called from main(), from inside
an endless loop.
*/
void input(void)
{

	int plox [2];
	char move_x = 0;
	char move_y = 0;
//	unsigned char keyin = 0;
	char keyin = 0;
	char str_pass [50];

	you[0].time_taken = player_speed();


	window(1,1,80,25);
	textcolor(7);

    print_stats();



 	if (you[0].paralysis == 0)
	{

		if (you[0].delay_t > 1) you[0].delay_t--;
		if (you[0].delay_t == 1)
		{
			switch(you[0].delay_doing)
			{
			 	case 0: mpr("You finish eating."); break;
			 	case 1: mpr("You finish putting on your armour."); break;
		 		case 2: mpr("You finish taking off your armour."); break;
	         	case 3: mpr("You finish installing."); break;
		 		case 4: if (you[0].species == SP_TROLL || you[0].species == SP_GHOUL) mpr("You finish ripping the corpse into pieces.");
				else mpr("You finish chopping the corpse into pieces.");
				break;
			}
			you[0].delay_t = 0;
		}
//        } else keyin = '.';

		gotoxy(18,9);

	   	if (you[0].delay_t == 0)
		{
			_setcursortype(_NORMALCURSOR);

			if (you[0].running > 0)
			{
				keyin = 125;
				move_x = you[0].run_x;
				move_y = you[0].run_y;

      			if (kbhit())
      			{
         			you[0].running = 0;
         			goto gutch;
      			}

				if (you[0].run_x == 0 && you[0].run_y == 0)
				{
					you[0].running--;
					keyin = '.';
				}
			} else
	       		gutch: keyin = getch();

 			mesclr();

			if (keyin == 0) // alternate ("alt") also works - see ..\KEYTEST.CPP
			{
				keyin = getch();
				switch(keyin)
				{
					case 'O': move_x = -1; move_y = 1; break;
					case 'P': move_y = 1; move_x = 0; break;
					case 'I': move_x = 1; move_y = -1; break;
					case 'H': move_y = -1; move_x = 0; break;
					case 'G': move_y = -1; move_x = -1; break;
					case 'K': move_x = -1; move_y = 0; break;
					case 'Q': move_y = 1; move_x = 1; break;
					case 'M': move_x = 1; move_y = 0; break;

	                case 119: open_door(-1, -1); move_x = 0; move_y = 0; break;
	                case -115: open_door(0, -1); move_x = 0; move_y = 0; break;
	                case -124: open_door(1, -1); move_x = 0; move_y = 0; break;
	                case 116: open_door(1, 0); move_x = 0; move_y = 0; break;
	                case 118: open_door(1, 1); move_x = 0; move_y = 0; break;
	                case -111: open_door(0, 1); move_x = 0; move_y = 0; break;
	                case 117: open_door(-1, 1); move_x = 0; move_y = 0; break;
	                case 115: open_door(-1, 0); move_x = 0; move_y = 0; break;

	                case 76:
					case 'S': keyin = '.'; goto get_keyin_again;

				}

				keyin = 125;
			}
//	}
//	else keyin = '\\'; // end of if delay_t > 1
		}
		else keyin = '.';
	} else keyin = '.'; // end of if you[0].paralysis == 0

	if (keyin != 125)
	{
		move_x = 0;
		move_y = 0;
		you[0].turnover = 0;
	}

	get_keyin_again :
//putch(keyin);
//getch();


	switch(keyin)
	{
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
#ifdef PLAIN_TERM
			redraw_screen();
#endif
			break;
		case 'A': display_mutations();
#ifdef PLAIN_TERM
			redraw_screen();
#endif
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
#ifdef PLAIN_TERM
			redraw_screen();
#endif
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
   				strcpy(info, "You have no idea where you are!");
   				mpr(info);
   				break;
  			}
			plox [0] = 0;
			show_map(plox);
#ifdef PLAIN_TERM
			redraw_screen();
#endif
			break;
		case '\\': check_item_knowledge(); //nothing = check_item_knowledge();
#ifdef PLAIN_TERM
			redraw_screen();
#endif
			break;
  		case 16: replay_messages();
#ifdef PLAIN_TERM
			redraw_screen();
#endif
			break;
	  	case '?': list_commands();
#ifdef PLAIN_TERM
			redraw_screen();
#endif
			break;
  		case 'C':
  			strcpy(info, "You are a level ");
  			itoa(you[0].xl, st_prn, 10);
  			strcat(info, st_prn);
  			strcat(info, " ");
  			strcat(info, species_name(you[0].species));
  			strcat(info, " ");
  			strcat(info, you[0].clasnam);
  			strcat(info, ".");
  			mpr(info);

  			if (you[0].xl == 27)
  			{
 				strcpy(info, "I'm sorry, level 27 is as high as you can go.");
                mpr(info);
				strcpy(info, "With the way you've been playing, I'm surprised you got this far.");
			   	mpr(info);
			   	break;
	  		}
			strcpy(info, "Your next level is at ");
  			itoa(exp_needed(you[0].xl + 2, you[0].species) + 1, st_prn, 10);
			strcat(info, st_prn);
		  	strcat(info, " experience points.");
  			mpr(info);
			break;
  		case '!':
  			yell(); /* in effects.cc */
  			break;
  		case '@': display_char_status(); break;


  		case 'm':
  			show_skills();
#ifdef PLAIN_TERM
			redraw_screen();
#endif
  			break;

//  case '^': disarm_trap(); break;

  		case '#':
  			char name_your [30];
  			strncpy(name_your, you[0].your_name, 6);
  			name_your [6] = 0;
  			if (dump_char(0, name_your) == 1)
   				strcpy(info, "Char dumped successfully.");
    		else
    			strcpy(info, "Char dump unsuccessful! Sorry about that.");
		    mpr(info);
		  	break;

  		case '`': macro_add_query(); break;
  		case '~': mpr("Saving macros."); macro_save(); break;



/* Debug starts: * /


 case '&': cast_spec_spell(); break;
  case '%': create_spec_object2(); break;
  case '*': grd [you[0].x_pos] [you[0].y_pos] = 82; break;
  case '(':
	char specs [3];
	strcpy(info, "Create which feature? ");
	mpr(info);
	specs [0] = getche();
	specs [1] = getche();
	specs [2] = getche();
    grd [you[0].x_pos] [you[0].y_pos] = atoi(specs); break;
  case 'G': debug_add_skills(); break;
//  case '\"': grd [you[0].x_pos] [you[0].y_pos] = 99; break;
//  case '\'': grd [you[0].x_pos] [you[0].y_pos] = 101; break;
  case '+': create_spec_monster(); break;

//case '`':
//   itoa(grd [you[0].x_pos] [you[0].y_pos], st_prn, 10);
//   itoa(you[0].mpower, st_prn, 10);
//			strcpy(info, st_prn);
//   mpr(info);
//break;

case '[':
   itoa(you[0].x_pos, st_prn, 10);
   strcpy(info, st_prn);
   strcat(info, "/");
   itoa(you[0].y_pos, st_prn, 10);
   strcat(info, st_prn);
   mpr(info);
   itoa(player_res_fire(), st_prn, 10);
   strcpy(info, st_prn);
   mpr(info);
   itoa(player_res_cold(), st_prn, 10);
   strcpy(info, st_prn);
   mpr(info);
   itoa(player_res_poison(), st_prn, 10);
   strcpy(info, st_prn);
   mpr(info);
   itoa(player_prot_life(), st_prn, 10);
   strcpy(info, st_prn);
   mpr(info);
//   acquirement();
break;

case ']':
	char specx [2];
	strcpy(info, "Gain which mutation? ");
	mpr(info);
	specx [0] = getche();
	specx [1] = getche();
    mutate(atoi(specx));
break;

  case ')': you[0].xp += 5000; you[0].exp_available += 500; you[0].xp_ch = 1;
  level_change();
  break;
  case '\"': level_travel(); break;
  case '-': you[0].hp = you[0].hp_max; you[0].hp_ch = 1; break;
  case '$': you[0].gp += 500; you[0].gp_ch = 1; break;

  case 'F':	stethoscope(250); break;

  case ':':
  int i, j;
  j = 0;
  for (i = 0; i < 20; i ++)
  {
   if (you[0].branch_stairs [i] == 0) continue;
   strcpy(info, "Branch ");
   itoa(i, st_prn, 10);
   strcat(info, st_prn);
   strcat(info, " is on level ");
   itoa(you[0].branch_stairs [i], st_prn, 10);
   strcat(info, st_prn);
   strcat(info, ".");
   mpr(info);
  }
  break;
  case '{':
  magic_mapping(99, 100);
  break;
  case '_':
  mpr("You feel more pious! Well done.");
  gain_piety(10);
  break;
//  case '}': Xom_acts(1, 50, 1); break;
  case '|': acquirement(250); break;
*/
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
  			strcpy(info, "You have a total of ");
  			itoa(j, st_prn, 10);
  			strcat(info, st_prn);
  			strcat(info, " skill points.");
  			mpr(info);
  			break;
/*  case '=':
  new_level();
  strcpy(info, "");
  for (i = 0; i < 20; i ++)
  {
    itoa(mons_alloc [i], st_prn, 10);
    strcat(info, st_prn);
    strcat(info, ",");
  }
  mpr(info);
  break;*/
#endif

#ifdef DEBUG
		case '`':
   			itoa(OUTPUT_NO, st_prn, 10);
			strcpy(info, st_prn);
   			mpr(info);
			move_x = 0;
			move_y = 0;
			break;

        case '\'':
            for (i = 0; i < ITEMS; i ++)
            {
	            if (it[0].ilink [i] == ING) continue;
                itoa(i, st_prn, 10);
                strcpy(info, st_prn);
                strcat(info, " is linked to ");
                itoa(it[0].ilink [i], st_prn, 10);
                strcat(info, st_prn);
                strcat(info, " c:");
                itoa(it[0].iclass [i], st_prn, 10);
                strcat(info, st_prn);
                strcat(info, " t:");
                itoa(it[0].itype [i], st_prn, 10);
                strcat(info, st_prn);
                strcat(info, " p:");
                itoa(it[0].iplus [i], st_prn, 10);
                strcat(info, st_prn);
                strcat(info, " p2:");
                itoa(it[0].iplus2 [i], st_prn, 10);
                strcat(info, st_prn);
                strcat(info, " d:");
                itoa(it[0].idam [i], st_prn, 10);
                strcat(info, st_prn);
                strcat(info, " q: ");
                itoa(it[0].iquant [i], st_prn, 10);
                strcat(info, st_prn);
                mpr(info);
    		}
            strcpy(info, "igrid:");
            mpr(info);
            for (i = 0; i < GXM; i ++)
            {
            	for (j = 0; j < GYM; j ++)
                {
                	if (igrid [i] [j] != ING)
                  	{
                   		itoa(igrid [i] [j], st_prn, 10);
                   		strcpy(info, st_prn);
                   		strcat(info, " at ");
                   		itoa(i, st_prn, 10);
                   		strcat(info, st_prn);
                   		strcat(info, ", ");
                   		itoa(j, st_prn, 10);
                   		strcat(info, st_prn);
                 		strcat(info, " c:");
                 		itoa(it[0].iclass [igrid [i] [j]], st_prn, 10);
                 		strcat(info, st_prn);
                 		strcat(info, " t:");
                	 	itoa(it[0].itype [igrid [i] [j]], st_prn, 10);
                 		strcat(info, st_prn);
                 		strcat(info, " p:");
                 		itoa(it[0].iplus [igrid [i] [j]], st_prn, 10);
                 		strcat(info, st_prn);
                 		strcat(info, " p2:");
                 		itoa(it[0].iplus2 [igrid [i] [j]], st_prn, 10);
                 		strcat(info, st_prn);
                 		strcat(info, " d:");
                 		itoa(it[0].idam [igrid [i] [j]], st_prn, 10);
                 		strcat(info, st_prn);
                 		strcat(info, " q: ");
	                 	itoa(it[0].iquant [igrid [i] [j]], st_prn, 10);
                 		strcat(info, st_prn);
	                   	mpr(info);
                  	}
              	}
          	}

			break;

#endif

		case 'S': save_game(1); break;
		case 'Q': quit_game(); break;
		case 'v': version(); break;
	    case '}':
//        Can't use this char
		break;
		default: strcpy(info, "Unknown command.");
			mpr(info); break;

	}



	if (move_x != 0 || move_y != 0) move(move_x, move_y);


	if (you[0].turnover == 0)
	{
		viewwindow(1);
		return;
	}

//if (random2(10) < you[0].skills [SK_TRAPS_DOORS] + 2) search_around();

	stealth = check_stealth();

	if (you[0].special_wield != 0)
	{
 		special_wielded();
	}


	if (random2(10) == 0)
	{
 		if ((player_teleport() > 0 && random2(100 / player_teleport()) == 0) || (you[0].level_type == 2 && random() % 30 == 0))
 		{
			you_teleport2(1); // this is instantaneous
		}
	}


	if (env[0].cgrid [you[0].x_pos] [you[0].y_pos] != CNG) in_a_cloud();



	if (you[0].duration [DUR_LIQUID_FLAMES] > 0) you[0].duration [DUR_LIQUID_FLAMES] --;
// paradox: it both lasts longer & does more damage overall if you're moving slower.
// rationalisation: I guess it gets rubbed off/falls off/etc if you move around more.
	if (you[0].duration [DUR_LIQUID_FLAMES] != 0)
	{
 		mpr("You are covered in liquid flames!");
 		scrolls_burn(8, 6);
/* hit_player((random2(5) + random2(5) + 1) * you[0].time_taken / 10, 1, 0, 17, env);*/


 		if (player_res_fire() > 100)
 		{
  			ouch((((random2(5) + random2(5) + 1) / 2 + (player_res_fire() - 100) * (player_res_fire() - 100)) * you[0].time_taken) / 10, 0, 17);
 		}
 		if (player_res_fire() <= 100)
 		{
  			ouch(((random2(5) + random2(5) + 1) * you[0].time_taken) / 10, 0, 17);
 		}
 		if (player_res_fire() < 100)
 		{
  			ouch(((random2(5) + random2(5) + 1) * you[0].time_taken) / 10, 0, 17);
 		}

	}
	if (you[0].duration [DUR_ICY_ARMOUR] > 1)
	{
	 	you[0].duration [DUR_ICY_ARMOUR] --;
// 		scrolls_burn(4, 8);
	}
	if (you[0].duration [DUR_ICY_ARMOUR] == 1)
	{
 		mpr("Your icy armour evaporates.");
/* you[0].AC -= 6; */
 		you[0].AC_ch = 1;
	 	you[0].duration [DUR_ICY_ARMOUR] = 0;
	}

	if (you[0].duration [DUR_REPEL_MISSILES] > 0)
    {
      you[0].duration [DUR_REPEL_MISSILES] --;
      if (you[0].duration [DUR_REPEL_MISSILES] == 6)
      {
       mpr("Your repel missiles program is about to finish...");
       you[0].duration [DUR_REPEL_MISSILES] -= random2(2);
      }
    }
	if (you[0].duration [DUR_REPEL_MISSILES] == 1)
	{
 		mpr("You feel less protected from missiles.");
 		you[0].duration [DUR_REPEL_MISSILES] = 0;
	}

	if (you[0].duration [DUR_REPEL_MISSILES] > 0)
    {
      you[0].duration [DUR_DEFLECT_MISSILES] --;
      if (you[0].duration [DUR_DEFLECT_MISSILES] == 6)
      {
       mpr("Your deflect missiles program is about to finish...");
       you[0].duration [DUR_DEFLECT_MISSILES] -= random2(2);
      }
    }
	if (you[0].duration [DUR_DEFLECT_MISSILES] == 1)
	{
 		mpr("You feel less protected from missiles.");
 		you[0].duration [DUR_DEFLECT_MISSILES] = 0;
	}

	if (you[0].duration [DUR_REGENERATION] > 0)
	{
		you[0].duration [DUR_REGENERATION] --;
        if (you[0].duration [DUR_REGENERATION] == 6)
        {
         mpr("Your skin is crawling a little less now.");
         you[0].duration [DUR_REGENERATION] -= random2(2);
        }
		if (you[0].duration [DUR_REGENERATION] == 1)
		{
 			mpr("Your skin stops crawling.");
 			you[0].duration [DUR_REGENERATION] = 0;
/* you[0].rate_regen -= 100;*/
	 		you[0].hunger_inc -= 4;
		}
	}

	if (you[0].duration [DUR_PRAYER] > 0) you[0].duration [DUR_PRAYER] --;
	if (you[0].duration [DUR_PRAYER] == 1)
	{
 		mpr("Your communication is over.");
	 	you[0].duration [DUR_PRAYER] = 0;
	}

	if (you[0].duration [DUR_VORPAL_BLADE] > 0) you[0].duration [DUR_VORPAL_BLADE] --;
	if (you[0].duration [DUR_VORPAL_BLADE] == 1)
	{
 		item_name(you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_quant [you[0].equip [EQ_WEAPON]], you[0].inv_ident [you[0].equip [EQ_WEAPON]], 4, str_pass);
 		strcpy(info, str_pass);
 		strcat(info, " seems blunter.");
 		mpr(info);
 		you[0].duration [DUR_VORPAL_BLADE] = 0;
 		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 10;
 		wield_change = 1;
	}

	if (you[0].duration [DUR_FIRE_BRAND] > 0) you[0].duration [DUR_FIRE_BRAND] --;
	if (you[0].duration [DUR_FIRE_BRAND] == 1)
	{
 		item_name(you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_quant [you[0].equip [EQ_WEAPON]], you[0].inv_ident [you[0].equip [EQ_WEAPON]], 4, str_pass);
 		strcpy(info, str_pass);
 		strcat(info, " goes out.");
 		mpr(info);
 		you[0].duration [DUR_FIRE_BRAND] = 0;
 		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 1;
 		wield_change = 1;
	}

	if (you[0].duration [DUR_ICE_BRAND] > 0) you[0].duration [DUR_ICE_BRAND] --;
	if (you[0].duration [DUR_ICE_BRAND] == 1)
	{
	 	item_name(you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_quant [you[0].equip [EQ_WEAPON]], you[0].inv_ident [you[0].equip [EQ_WEAPON]], 4, str_pass);
 		strcpy(info, str_pass);
 		strcat(info, " stops glowing.");
 		mpr(info);
 		you[0].duration [DUR_ICE_BRAND] = 0;
 		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 2;
 		wield_change = 1;

	}


	if (you[0].duration [DUR_LETHAL_INFUSION] > 0) you[0].duration [DUR_LETHAL_INFUSION] --;
	if (you[0].duration [DUR_LETHAL_INFUSION] == 1)
	{
 		item_name(you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_quant [you[0].equip [EQ_WEAPON]], you[0].inv_ident [you[0].equip [EQ_WEAPON]], 4, str_pass);
 		strcpy(info, str_pass);
 		strcat(info, " stops crackling.");
 		mpr(info);
 		you[0].duration [DUR_LETHAL_INFUSION] = 0;
 		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 8;
 		wield_change = 1;
	}

	if (you[0].duration [DUR_POISON_WEAPON] > 0) you[0].duration [DUR_POISON_WEAPON] --;
	if (you[0].duration [DUR_POISON_WEAPON] == 1)
	{
 		item_name(you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_quant [you[0].equip [EQ_WEAPON]], you[0].inv_ident [you[0].equip [EQ_WEAPON]], 4, str_pass);
 		strcpy(info, str_pass);
 		strcat(info, " stops dripping with poison.");
 		mpr(info);
 		you[0].duration [DUR_POISON_WEAPON] = 0;
 		you[0].inv_dam [you[0].equip [EQ_WEAPON]] -= 6;
	 	wield_change = 1;
	}

	if (you[0].duration [DUR_BREATH_WEAPON] > 0) you[0].duration [DUR_BREATH_WEAPON] --;
	if (you[0].duration [DUR_BREATH_WEAPON] == 1)
	{
 		mpr("You have got your breath back.");
 		you[0].duration [DUR_BREATH_WEAPON] = 0;
	}

	if (you[0].duration [DUR_TRANSFORMATION] > 0)
	{
 		you[0].duration [DUR_TRANSFORMATION] --;
 		if (you[0].duration [DUR_TRANSFORMATION] == 10)
 		{
  			mpr("Your transformation is almost over.");
  			you[0].duration [DUR_TRANSFORMATION] -= random2(3);
 		}
	}
	if (you[0].duration [DUR_TRANSFORMATION] == 1)
	{
 		untransform();
 		you[0].duration [DUR_BREATH_WEAPON] = 0;
	}


	if (you[0].duration [DUR_SWIFTNESS] > 0)
    {
     you[0].duration [DUR_SWIFTNESS] --;
     if (you[0].duration [DUR_SWIFTNESS] == 6)
     {
        mpr("You start to feel a little slower.");
        you[0].duration [DUR_SWIFTNESS] -= random2(2);
     }
    }
	if (you[0].duration [DUR_SWIFTNESS] == 1)
	{
	 	mpr("You feel sluggish."); // swiftness runs out
 		you[0].duration [DUR_SWIFTNESS] = 0;
/* you[0].fast_you[0].run --;*/
	}

	if (you[0].duration [DUR_INSULATION] > 0)
    {
      you[0].duration [DUR_INSULATION] --;
      if (you[0].duration [DUR_INSULATION] == 6)
      {
        mpr("You start to feel a little less insulated.");
        you[0].duration [DUR_INSULATION] -= random2(2);
      }
    }
	if (you[0].duration [DUR_INSULATION] == 1)
	{
 		mpr("You feel conductive."); // insulation (lightning resistance) wore off
	 	you[0].duration [DUR_INSULATION] = 0;
 		you[0].attribute [ATTR_RESIST_LIGHTNING] --;
	}

	if (you[0].duration [DUR_STONEMAIL] > 1)
    {
     you[0].duration [DUR_STONEMAIL] --;
     if (you[0].duration [DUR_STONEMAIL] == 6)
     {
        mpr("Your scaley stone armour is starting to flake away.");
        you[0].duration [DUR_STONEMAIL] -= random2(2);
     }
    }
	if (you[0].duration [DUR_STONEMAIL] == 1)
	{
 		mpr("Your scaley stone armour disappears.");
/* you[0].AC -= 7;
 you[0].evasion += 2;
 you[0].evasion_ch = 1;
 you[0].AC_ch = 1;*/
 		you[0].duration [DUR_STONEMAIL] = 0;
 		burden_change();
	}

	if (you[0].duration [DUR_TELEPORT] > 1) you[0].duration [DUR_TELEPORT] --;
	if (you[0].duration [DUR_TELEPORT] == 1)
	{
 		you_teleport2(1);
 		you[0].duration [DUR_TELEPORT] = 0;
	}

	if (you[0].duration [DUR_CONTROL_TELEPORT] > 1)
    {
      you[0].duration [DUR_CONTROL_TELEPORT] --;
      if (you[0].duration [DUR_CONTROL_TELEPORT] == 6)
      {
        mpr("You start to feel a little uncertain.");
        you[0].duration [DUR_CONTROL_TELEPORT] -= random2(2);
      }
    }
	if (you[0].duration [DUR_CONTROL_TELEPORT] == 1)
	{
 		mpr("You feel uncertain."); // teleport control
 		you[0].duration [DUR_CONTROL_TELEPORT] = 0;
 		you[0].attribute [ATTR_CONTROL_TELEPORT] --;
	}

	if (you[0].duration [DUR_RESIST_POISON] > 1)
    {
      you[0].duration [DUR_RESIST_POISON] --;
      if (you[0].duration [DUR_RESIST_POISON] == 6)
      {
        mpr("Your poison resistance is about to expire.");
        you[0].duration [DUR_RESIST_POISON] -= random2(2);
      }
    }
	if (you[0].duration [DUR_RESIST_POISON] == 1)
	{
 		mpr("Your poison resistance expires."); // poison resistance wore off
 		you[0].duration [DUR_RESIST_POISON] = 0;
	}

	if (you[0].duration [DUR_DEATH_CHANNEL] > 1)
    {
      you[0].duration [DUR_DEATH_CHANNEL] --;
      if (you[0].duration [DUR_DEATH_CHANNEL] == 6)
      {
        mpr("Your unholy channel is weakening.");
        you[0].duration [DUR_DEATH_CHANNEL] -= random2(2);
      }
    }
	if (you[0].duration [DUR_DEATH_CHANNEL] == 1)
	{
 		mpr("Your unholy channel expires."); // Death channel wore off
 		you[0].duration [DUR_DEATH_CHANNEL] = 0;
	}




	if (you[0].duration [DUR_LIQUID_FLAMES] > 0) you[0].duration [DUR_LIQUID_FLAMES] --;

	if (you[0].duration [DUR_TELEPORT] > 1) you[0].duration [DUR_TELEPORT] --;
	if (you[0].duration [DUR_TELEPORT] == 1)
	{
 		you_teleport2(1);
 		you[0].duration [DUR_TELEPORT] = 0;
	}

	if (you[0].invis > 1)
	{
	 	you[0].invis --;
	 	if (you[0].hunger >= 40 && you[0].is_undead != 2) you[0].hunger -= 5;
        if (you[0].invis == 6)
        {
         mpr("You flicker for a moment.");
         you[0].invis -= random2(2);
        }
	}
	if (you[0].invis == 1)
	{
		strcpy(info, "You flicker back into view.");
		mpr(info);
		you[0].invis = 0;
	}



	if (you[0].conf > 1) you[0].conf --;
	if (you[0].conf == 1)
	{
		strcpy(info, "You feel steadier.");
		mpr(info);
		you[0].conf = 0;
	}


	if (you[0].paralysis > 1) you[0].paralysis--;
	if (you[0].paralysis == 1)
	{
		strcpy(info, "You can move again.");
		mpr(info);
		you[0].paralysis = 0;
	}



	if (you[0].slow > 1) you[0].slow--;
	if (you[0].slow == 1)
	{
		strcpy(info, "You feel yourself speed up.");
		mpr(info);
		you[0].slow = 0;
	}

	if (you[0].haste > 1)
	{
	 	you[0].haste--;
        if (you[0].haste == 6)
        {
          mpr("Your extra speed is starting to run out.");
          you[0].haste -= random2(2);
        }
	}

	if (you[0].haste == 1)
	{
		strcpy(info, "You feel yourself slow down.");
		mpr(info);
		you[0].haste = 0;
	}

	if (you[0].might > 1) you[0].might--;
	if (you[0].might == 1)
	{
		strcpy(info, "You feel a little less mighty now.");
		mpr(info);
		you[0].might = 0;
		you[0].strength -= 5;
		you[0].max_strength -= 5;
		you[0].strength_ch = 1;
	}

	if (you[0].berserker > 1) you[0].berserker--;
	if (you[0].berserker == 1)
	{
		strcpy(info, "You went out battle mode.");
		mpr(info);
		strcpy(info, "You feel exhausted.");
		mpr(info);
		you[0].berserker = 0;
 		you[0].slow += 4 + random2(4) + random2(4);
 		you[0].hunger -= 700;
 		if (you[0].hunger <= 50) you[0].hunger = 50;
 		calc_hp();
 		you[0].hp_ch = 1;
	}

	if (you[0].lev > 1)
	{
        if (you[0].species != SP_KENKU || you[0].xl < 15)
 		 you[0].lev--;
 		if (you[0].lev == 10)
 		{
  			strcpy(info, "You are starting to lose your buoyancy!");
  			mpr(info);
  			you[0].lev -= random2(6); // so you never know how much time you have left!
  			if (you[0].duration [DUR_CONTROLLED_FLIGHT] > 0) you[0].duration [DUR_CONTROLLED_FLIGHT] = you[0].lev;
 		}
	}

	if (you[0].lev == 1)
	{
		strcpy(info, "You float gracefully downwards.");
		mpr(info);
		you[0].lev = 0;
	    burden_change();
        you[0].duration [DUR_CONTROLLED_FLIGHT] = 0;
		if (grd [you[0].x_pos] [you[0].y_pos] == 61 || grd [you[0].x_pos] [you[0].y_pos] == 62) fall_into_a_pool(1, grd [you[0].x_pos] [you[0].y_pos]);
	}


	if (you[0].rotting > 0 || (you[0].species == SP_GHOUL && random2(2) == 0))
	{
     	if (you[0].species == SP_MUMMY)
          you[0].rotting = 0;
			else
				if (random2(20) <= (you[0].rotting - 1) || (you[0].species == SP_GHOUL && random2(200) == 0))
				{
					ouch(1, 0, 21);
					you[0].hp_max --;
			        you[0].base_hp --;
					you[0].hp_ch = 1;
					mpr("You feel your flesh rotting away.");
					you[0].rotting--;
				}
	}

	if (you[0].disease > 0)
	{
 		you[0].disease --;
 		if (you[0].species == SP_KOBOLD && you[0].disease > 5) you[0].disease -= 2;
 		if (you[0].disease == 0)
 		{
  			strcpy(info, "You feel your health improve.");
  			mpr(info);
 		}
	}

	if (you[0].poison > 0)
	{
		if (random2(5) <= (you[0].poison - 1))
		{
	  		if (you[0].poison > 10 && random2(you[0].poison) >= 8)
			{
	 			ouch(random2(10) + 5, 0, 1);
	 			you[0].hp_ch = 1;
	 			strcpy(info, "You feel extremely sick.");
	 			mpr(info);
			} else
	 		if (you[0].poison > 5 && random2(2) == 0)
	 		{
	 			ouch(random2(2) + 2, 0, 1);
	 			you[0].hp_ch = 1;
	 			strcpy(info, "You feel very sick.");
	 			mpr(info);

	 		} else
	      	{
				ouch(1, 0, 1);
				you[0].hp_ch = 1;
				strcpy(info, "You feel sick.");// //the poison running through your veins.");
				mpr(info);
	      	}

			if (random2(8) == 0 || (you[0].hp == 1 && random2(3) == 0))
			{
				strcpy(info, "You feel a little better.");
				mpr(info);
				you[0].poison--;
			}

		}

	}

	if (you[0].deaths_door > 0)
	{
	 	if (you[0].hp > you[0].skills [SK_NECROMANCY] + (you[0].religion == GOD_KIKUBAAQUDGHA) * 13)
 		{
  			strcpy(info, "Your life is in your own hands once again.");
  			mpr(info);
	  		you[0].paralysis += 5 + random2(5);
  			you[0].conf += 10 + random2(10);
  			you[0].hp_max --;
  			if (you[0].hp > you[0].hp_max) you[0].hp = you[0].hp_max;
  			you[0].hp_ch = 1;
  			you[0].deaths_door = 0;
		}
   		else you[0].deaths_door --;

 		if (you[0].deaths_door == 10)
 		{
  			strcpy(info, "Your time is quickly running out!");
 	 		mpr(info);
  			you[0].deaths_door -= random2(6); // so that you never know how many turns you have left. Evil, huh?
 		}

 		if (you[0].deaths_door == 1)
 		{
  			strcpy(info, "Your life is in your own hands again!");
  			mpr(info);
//  relay_message();
  			more();
/*  you[0].hp = 1;*/
  			you[0].hp_ch = 1;
 		}

	}

	if (you[0].is_undead != 2)
	{
 		if (you[0].hunger_inc > 0 && you[0].hunger >= 40)
 		{
  			you[0].hunger -= you[0].hunger_inc;
  			you[0].hunger -= you[0].burden_state;
 		}
	} else you[0].hunger = 6000;

	if (you[0].hp < you[0].hp_max && you[0].disease == 0 && you[0].deaths_door == 0) you[0].incr_regen += player_regen();
	if (you[0].ep < you[0].ep_max) you[0].ep_incr_regen += 7 + you[0].ep_max / 2;

	while(you[0].incr_regen >= 100)
	{
 		if (you[0].hp == you[0].hp_max - 1 && you[0].running != 0 && you[0].run_x == 0 && you[0].run_y == 0) you[0].running = 0;
		you[0].hp ++; if (you[0].hp > you[0].hp_max) you[0].hp = you[0].hp_max;
		you[0].incr_regen -= 100;
		you[0].hp_ch = 1;
	}

	while(you[0].ep_incr_regen >= 100)
	{
 		if (you[0].ep == you[0].ep_max - 1 && you[0].running != 0 && you[0].run_x == 0 && you[0].run_y == 0) you[0].running = 0;
		you[0].ep ++; if (you[0].ep > you[0].ep_max) you[0].ep = you[0].ep_max;
		you[0].ep_incr_regen -= 100;
		you[0].ep_ch = 1;
	}



//losight(show, grid, you[0].x_pos, you[0].y_pos);
	viewwindow(0);

	monster();

	if (you[0].corpse_count <= you[0].time_taken)
	{
 		manage_corpses();
 		you[0].corpse_count = 200;
 		if (random2(50) == 0) cull_items();
	} else you[0].corpse_count -= you[0].time_taken;




	env[0].cloud_no = manage_clouds(); //cloud_x, cloud_y, cloud_type, cloud_decay, cloud_no, grid, time_taken, you[0].haste, you[0].slow, cgrid);


	if (you[0].shock_shield > 0)
	{
 		manage_shock_shield();
	}

    /*
    If visible [0] is 1, at least one statue is visible.
    This was supposed to be more complex (with values of 2 etc), but I
     couldn't get it to work.
    */
    if (visible [0] != 0)
    {
     char wc [30];
     if (visible [1] != 0)
     {
      switch(visible [1])
      {
       case 0: break;
//       case 1: mpr("You feel a pleasing absence."); break;
       case 2:
            if (random2(4) == 0)
            {
             strcpy(info, "The silver statue's eyes glow a ");
             weird_colours(random2(200), wc);
             strcat(info, wc);
             strcat(info, " colour.");
             mpr(info);
			 create_monster(summon_any_demon(random2(2)), 25, 1, you[0].x_pos, you[0].y_pos, MHITYOU, 250);
            }
       break;
//       case 3: mpr("You feel a terrible presence observing you."); break;
      }
      visible [1] --;
     }
     if (visible [2] != 0)
     {
      if (random2(3) == 0)
      {
       mpr("A hostile presence attacks your cyberbrain!");
       miscast_effect(SPTYP_DIVINATION, random2(15), random2(150), 100);
      }
      visible [2] --;
     }
    visible [0] --;
    }



	if (you[0].hunger <= 500)
	{
		if (you[0].paralysis == 0 && random2(40) == 0)
		{
	 		strcpy(info, "You lose consciousness!");
	 		mpr(info);
			you[0].paralysis += random2(8) + 5;
 			if (you[0].paralysis > 13) you[0].paralysis = 13;
		}
		if (you[0].hunger <= 100)
		{
 			strcpy(info, "You have starved to death.");
 			mpr(info);
 			ouch(-9999, 0, 15);
		}
	}

	if (you[0].hung_state == 3 && you[0].hunger <= 2600) // && you[0].hunger_inc > 0)
	{
		strcpy(info, "You are feeling hungry.");
		mpr(info);
		you[0].hung_state = 2;
		you[0].hung_ch = 1;
	}


	if (you[0].hung_state == 2 && you[0].hunger <= 1000) // && you[0].hunger_inc > 0)
	{
		strcpy(info, "You are starving!");
		mpr(info);
		you[0].hung_state = 1;
		you[0].hung_ch = 1;
	}


	if (you[0].hung_state == 4 && you[0].hunger < 7000) // && you[0].hunger_inc > 0)
	{
	 	you[0].hung_state = 3;
 		you[0].hung_ch = 1;
	}

	if (you[0].hung_state == 5 && you[0].hunger < 11000) // && you[0].hunger_inc > 0)
	{
	 	you[0].hung_state = 4;
 		you[0].hung_ch = 1;
	}



	_setcursortype(_NOCURSOR);

	viewwindow(1);

	_setcursortype(_NORMALCURSOR);

	if (you[0].paralysis > 0)
	{
		more();
	}



	if (you[0].level_type != 0) /* No monsters in labyrinths */
    {
	    switch(you[0].level_type)
        {
	        case 1: break; /* labyrinth */
    	    case 2: if (random2(5) == 0) mons_place(2500, 0, 50, 50, 1, MHITNOT, 250, 51);
           		break; /* Abyss  */
           	case 3: if (random2(50) == 0) pandemonium_mons(); //mons_place(250, 0, 50, 50, 0, MHITNOT, 250, 52);
           		break; /* Pandemonium */
      	}
  	} else if (random2(240) == 0 && you[0].level_type != 1 && you[0].where_are_you != 18) mons_place(2500, 0, 50, 50, 1, MHITNOT, 250, you[0].your_level);

	return;

}


/*
Opens doors and handles some aspects of untrapping. If move_x != 100, it
carries a specific direction for the door to be opened (eg if you type
ctrl - dir).
*/
void open_door(char move_x, char move_y)
{
/*move_x = door_x;
move_y = door_y;*/
//int nothing = 0;
	struct dist door_move [1];

	door_move[0].move_x = move_x;
	door_move[0].move_y = move_y;

	if (move_x != 100 && env[0].mgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] != MNG && (menv[env[0].mgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y]].m_class < MLAVA0 || menv [env[0].mgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y]].m_sec == 0))
	{
/* if (menv [env[0].mgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y]].m_ench [2] == 6 && player_see_invis() == 0)
 {
  strcpy(info, "Something seems to be in the way.");
  mpr(info);
  you[0].turnover = 1;
  return;
 }
 strcpy(info, "You might want to wait for the creature standing in your way to move.");
 mpr(info);
 return;*/
 		you_attack(mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]);
 		you[0].turnover = 1;
 		return;
	}

	if (move_x != 100 && grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] >= 75 && grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] <= 77)
	{
 	    if (env[0].cgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] != CNG)
 	    {
 		 mpr("You can't get to that trap right now.");
 		 return;
	    }
 		disarm_trap(door_move);
 		return;
	}

	if (move_x == 100)
	{
 		door_move[0].move_x = 0;
 		door_move[0].move_y = 0;
 		strcpy(info, "Which direction?");
 		mpr(info);
 		direction(0, door_move);
	}/* else
 {
  if (grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] == 70)
  {
   close_door(door_move[0].move_x, door_move[0].move_x);
   return;
  }
 }*/

	if (door_move[0].nothing == -1) return;

	if (door_move[0].move_x > 1 || door_move[0].move_y > 1 || door_move[0].move_x < -1 || door_move[0].move_y < -1)
	{
		strcpy(info, "I'm afraid your arm isn't that long.");
		mpr(info);
		return;
	}

	if (grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] == 3)
	{
		if (you[0].lev != 0)
		{
			strcpy(info, "You reach down and open the door.");
			mpr(info);
			grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] = 70;
			you[0].turnover = 1;
		} else
	    {
			if (random2(25) == 0)
			{
				strcpy(info, "As you open the door, it creaks loudly!");
                noisy(15, you[0].x_pos, you[0].y_pos);
			} else strcpy(info, "You open the door.");

			mpr(info);
			grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] = 70;
			you[0].turnover = 1;
		}
	} else
	{
		strcpy(info, "You swing at nothing.");
	 	mpr(info);
 	 	you[0].turnover = 1;
	 	if (you[0].is_undead != 2) you[0].hunger -= 3;
	}

} // end of void open_door()


/*
Similar to open_door. Can you spot the difference?
*/
void close_door(char door_x, char door_y)
{
	struct dist door_move [1];
	door_move[0].move_x = door_x;
	door_move[0].move_y = door_y;

	if (door_move[0].move_x == 100)
	{
 		door_move[0].move_x = 0;
 		door_move[0].move_y = 0;
 		strcpy(info, "Which direction?");
 		mpr(info);
 		direction(0, door_move);
	}

	if (door_move[0].move_x > 1 || door_move[0].move_y > 1)
	{
		strcpy(info, "I'm afraid your arm isn't that long.");
		mpr(info);
		return;
	}

	if (door_move[0].move_x == 0 && door_move[0].move_y == 0)
	{
		strcpy(info, "You can't close doors on yourself!");
		mpr(info);
		return;
	}

//if (env[0].eenv[0].mgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] != MNG)
	if (env[0].mgrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] != MNG)
	{
	// Need to make sure that turnover = 1 if creature is invisible
		strcpy(info, "There's a creature in the doorway!");
		mpr(info);
		door_move[0].move_x = 0; door_move[0].move_y = 0;
		return;
	}



	if (grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] == 70)
	{

		if (env[0].igrid [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] != 501)
		{
			strcpy(info, "There's something blocking the doorway.");
			mpr(info);
		 	door_move[0].move_x = 0;
			door_move[0].move_y = 0;
		 	return;
		}

		if (you[0].lev != 0)
		{
	 		strcpy(info, "You reach down and close the door.");
	 		mpr(info);
	 		grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] = 3;
	 		you[0].turnover = 1;
		} else
	    {
			if (random2(25) == 0)
			{
				strcpy(info, "As you close the door, it creaks loudly!");
                noisy(15, you[0].x_pos, you[0].y_pos);
			} else strcpy(info, "You close the door.");

			mpr(info);
			grd [you[0].x_pos + door_move[0].move_x] [you[0].y_pos + door_move[0].move_y] = 3;
			you[0].turnover = 1;
	  	}
	} else
	{
		strcpy(info, "There isn't anything that you can close there!");
		mpr(info);
	}

} // end of void open_door()




/*
Initialises a whole lot of stuff.
*/
void initialise(void)
{

	int i = 0;

	your_sign = '@';
	your_colour = LIGHTGREY;

/*for (i = 0; i < 10; i ++)
{
	func_pass [i] = 0;
}*/

	for (i = 0; i < NO_EQUIP; i++)
	{
		you[0].equip [NO_EQUIP] = -1;
	}
	/*for (i = 0; i < 20; i++)
{
	mons_alloc [i] = 250;
}*/


/* system initialisation stuff */
	textbackground(0);
	you[0].your_level = 0;

#ifdef DOS
	directvideo = 1;
#endif

#ifdef USE_EMX
  	init_emx();
#endif

	srandom(time(NULL));
	clrscr();




/* init item array */
	for (i = 1; i < ITEMS; i++)
	{
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


	strcpy(info, "");

	int j = 0;


//env[0].it [0] = &it[0];
//env[0].elvl [0] = &lvl[0];

	for (i = 0; i < MNST; i++)
	{
//        env[0].mons [i] = &mons [i];
		env[0].mons[i].m_class = -1;
		env[0].mons[i].m_speed_inc = 10;
		env[0].mons[i].m_targ_1_x = 155;
		env[0].mons[i].m_ench_1 = 0;
		env[0].mons[i].m_beh = 0; // sleeping
		env[0].mons[i].m_hit = MNG; // nothing
		for (j = 0; j < 3; j++)
		{
			env[0].mons[i].m_ench [j] = 0;
		}
		for (j = 0; j < 8; j++)
		{
			env[0].mons[i].m_inv [j] = ING;
		}
		env[0].mons[i].m_sec = 0;
	}

	for (i = 0; i < GXM; i ++)
	{
		for (j = 0; j < GYM; j ++)
		{
			env[0].igrid [i] [j] = ING;
			env[0].mgrid [i] [j] = MNG;
			env[0].map [i] [j] = 0;
		}
	}

	for (i = 0; i < 52; i++)
	{
		you[0].inv_quant [i] = 0;
	}

/*for (i = 0; i < 50; i++)
{
	unique_creatures [i] = 0;
}*/


	for (i = 0; i < 25; i ++)
	{
		you[0].spells [i] = 210;
	}

	for (i = 0; i < 10; i ++)
	{
		visible [i] = 0;
	}

	you[0].prev_targ = MHITNOT;

/*for (i = 0; i < 19; i ++)
{
 for (j = 0; j < 19; j ++)
 {
 show [i] [j] = 0;
 show_col [i] [j] = LIGHTGRAY;
 }
}*/


/* sets up a new game*/
	int newc = new_game();

	if (newc == 0) restore_game();

	calc_hp();
	calc_ep();

//if (you[0].species == 12) you[0].is_undead = 2; else you[0].is_undead = 0;
/*switch(you[0].species)
{
 case 10: your_sign = 'o'; break;
 case 11: your_sign = 'K'; break;
 case 12: your_sign = 'M'; break;
 default: your_sign = '@'; break;
}*/

/*if (newc == 1) stair_taken = 82;*/

	you[0].inv_no = 0;
	for (i = 0; i < 52; i ++)
	{
	 	if (you[0].inv_quant [i] != 0) you[0].inv_no ++;
	}
	char just_made_new_lev;

	if (newc == 0) just_made_new_lev = 1; else just_made_new_lev = 0;

	char moving_level = 0;
	if (newc == 1) moving_level = 1;

/*load(82, moving_level, level_saved, was_a_labyrinth, old_level, just_made_new_lev);*/
//load(82, moving_level, 0, 0, 0, 0, just_made_new_lev);
	load(82, moving_level, 0, 0, 0, just_made_new_lev, you[0].where_are_you);

	moving_level = 0;
	just_made_new_lev = 0;
	newc = 0;

//new_level();



	mon_init(gmon_use, mcolour); //, mcolour);
//new_level(); // - must come after mon_init

//def_letters(letters);

/*def_properties(property, mass);*/

	init_properties();


	if (newc == 1)
	{

		for (i = 0; i < GXM; i ++)
		{
			for (j = 0; j < GYM; j ++)
			{
				if (grd [i] [j] == 68)
				{
					you[0].x_pos = i;
					you[0].y_pos = j;
				}
			}
			if (grd [i] [j] == 67) break;
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
	char title [40];

	strcpy(title, skill_title(best_skill(0, 50, 99), you[0].skills [best_skill(0, 50, 99)], you[0].clas, you[0].xl));
	draw_border(BROWN, you[0].your_name, title, you[0].species);

	new_level();

	viewwindow(1); // This just puts the view up for the first turn.

	item();


}

/*
Called when the player moves by walking/running. Also calls attack function
and trap function etc when necessary.
*/
void move(char move_x, char move_y)
{
	char attacking = 0;
	char stepping = 0;
//char move_x, move_y;
	char info [200];
	int i;
	int trap_known, trapped;
	struct bolt beam [1];

	if (you[0].conf > 0)
	{
  		if (random2(3) != 0)
  		{
        	move_x = random2(3) - 1;
			move_y = random2(3) - 1;
  		}

		if (grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] < MINMOVE)
		{
			you[0].turnover = 1;
			mpr("Ouch!");
			return;
		}

  		if ((grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] == 61 || grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] == 62) && you[0].lev == 0)
		{
		  	fall_into_a_pool(0, grd [you[0].x_pos + move_x] [you[0].y_pos + move_y]);
			you[0].turnover = 1;
			return;
		}
	} // end of if you[0].conf

	if (you[0].running > 0 && you[0].running != 2 && grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] != 67 && grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] != 78)
	{
		you[0].running = 0;
		move_x = 0;
		move_y = 0;
		you[0].turnover = 0;
	    return;
	}

	if (mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y] != MNG)
	{
	    if (menv[mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]].m_class >= MLAVA0 && menv[mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]].m_sec == 1) goto break_out;
	    if (menv[mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]].m_beh == 7 && menv[mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]].m_ench [2] != 6 && you[0].conf == 0)
	    {
	      	swap_places(mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]);
	      	goto break_out;
	    }
		you_attack(mgrd [you[0].x_pos + move_x] [you[0].y_pos + move_y]);
		you[0].turnover = 1;
		attacking = 1;
	}

 	break_out :
	if ((grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] == 61 || grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] == 62) && attacking == 0 && you[0].lev == 0)
	{
    	mpr("Do you really want to step there?");
		stepping = get_ch();
  		if (stepping == 'y' || stepping == 'Y')
  		{
			fall_into_a_pool(0, grd [you[0].x_pos + move_x] [you[0].y_pos + move_y]);
 			you[0].turnover = 1;
	 		return;
 	 	}
  		mpr("Okay, then.");
		return;
	}

	if (attacking == 0 && (grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] >= MINMOVE))
	{
     	if (grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] == 78 && random() % (you[0].skills [SK_TRAPS_DOORS] + 1) > 3)
        {
        	strcpy(info, "Wait a moment, ");
            strcat(info, you[0].your_name);
            strcat(info, "! Do you really want to step there?");
            mpr(info);
            more();
            you[0].turnover = 0;
			for (i = 0; i < NTRAPS; i ++)
			{
				if (env[0].trap_x [i] == you[0].x_pos + move_x && env[0].trap_y [i] == you[0].y_pos + move_y) break;
			}
			if (env[0].trap_type [i] < 4 || env[0].trap_type [i] == 6 || env[0].trap_type [i] == 7) grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] = 75;
            if (env[0].trap_type [i] == 4 || env[0].trap_type [i] == 5 || env[0].trap_type [i] == 8) grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] = 76;
            return;
		}
		you[0].x_pos += move_x;
		you[0].y_pos += move_y;
        if (grd [you[0].x_pos] [you[0].y_pos] == 65 && you[0].lev == 0)
        {
         if (random2(3) == 0)
         {
          mpr("Splash!");
          noisy(10, you[0].x_pos, you[0].y_pos);
         }
		 you[0].time_taken *= 13 + random2(8);
		 you[0].time_taken /= 10;
         if (grd [you[0].x_pos - move_x] [you[0].y_pos - move_y] != 65)
         {
          mpr("You enter the shallow water. Moving in this stuff is going to be slow.");
          if (you[0].invis != 0)
          {
           mpr("And don't expect to remain undetected.");
          }
         }
        }
		move_x = 0;
		move_y = 0;
  		if (player_fast_run() != 0)
 	 	{
   			you[0].time_taken *= 6;
   			you[0].time_taken /= 10;
		}
 		if (you[0].attribute [ATTR_WALK_SLOWLY] != 0)
		{
   			you[0].time_taken *= 14;
   			you[0].time_taken /= 10;
  		}
		you[0].turnover = 1;
        item_check(0);

		if (grd [you[0].x_pos] [you[0].y_pos] > 74 && grd [you[0].x_pos] [you[0].y_pos] < 79)
		{

			if (grd [you[0].x_pos] [you[0].y_pos] == 78)
			{
				//abort();
				for (i = 0; i < NTRAPS; i ++)
				{
					if (env[0].trap_x [i] == you[0].x_pos && env[0].trap_y [i] == you[0].y_pos) break;
				}
				if (env[0].trap_type [i] < 4 || env[0].trap_type [i] == 6 || env[0].trap_type [i] == 7) grd [you[0].x_pos] [you[0].y_pos] = 75;
			   	if (env[0].trap_type [i] == 4 || env[0].trap_type [i] == 5 || env[0].trap_type [i] == 8) grd [you[0].x_pos] [you[0].y_pos] = 76;
	           	trap_known = 0;

				// else if (trap_type is magic etc
			} else trap_known = 1;


			for (i = 0; i < NTRAPS; i ++)
			{
				if (env[0].trap_x [i] == you[0].x_pos && env[0].trap_y [i] == you[0].y_pos) break;
			}

        	if (you[0].lev != 0 && (env[0].trap_type [i] < 4 || env[0].trap_type [i] == 6 || env[0].trap_type [i] == 7))
                goto out_of_traps; /* Can fly over mechanical traps */

			switch(env[0].trap_type [i])
			{

				case 0:
					strcpy(beam[0].beam_name, " dart");
					beam[0].damage = 4;
					trapped = i;
					dart_trap(trap_known, i, beam);
					break;

				case 1:
					strcpy(beam[0].beam_name, "n arrow");
					beam[0].damage = 7;
					trapped = i;
					dart_trap(trap_known, i, beam);
					break;

				case 2:
					strcpy(beam[0].beam_name, " spear");
					beam[0].damage = 10;
					trapped = i;
					dart_trap(trap_known, i, beam);
					break;

				case 3:
					strcpy(beam[0].beam_name, "n axe");
					beam[0].damage = 15;
					trapped = i;
					dart_trap(trap_known, i, beam);
					break;

   				case 4:
				   	mpr("You enter a ventilation shaft trap!");
	   				if (you[0].equip [EQ_WEAPON] != -1 && you[0].inv_class [you[0].equip [EQ_WEAPON]] == 0 && you[0].inv_dam [you[0].equip [EQ_WEAPON]] % 30 >= 25)
    				if (randart_wpn_properties(you[0].inv_class [you[0].equip [EQ_WEAPON]], you[0].inv_type [you[0].equip [EQ_WEAPON]], you[0].inv_dam [you[0].equip [EQ_WEAPON]], you[0].inv_plus [you[0].equip [EQ_WEAPON]], you[0].inv_plus2 [you[0].equip [EQ_WEAPON]], 0, RAP_PREVENT_TELEPORTATION) > 0)
				    {
     					mpr("You feel a weird sense of stasis.");
     					break;
   					}
	   				you_teleport2(1);
	   				break;

   				case 5:
   					mpr("You feel momentarily disoriented.");
   					forget_map(random2(50) + random2(50) + 2);
   					break;

				case 7:
					strcpy(beam[0].beam_name, " bolt");
					beam[0].damage = 13;
					trapped = i;
					dart_trap(trap_known, i, beam);
					break;



   				default: handle_traps(env[0].trap_type [i], trap_known); break;


			} // end of switch
		} // end of if another grd == trap

	}


	out_of_traps : if (grd [you[0].x_pos + move_x] [you[0].y_pos + move_y] <= MINMOVE)
	{
		move_x = 0;
		move_y = 0;
		you[0].running = 0;
		you[0].turnover = 0;
	}


 	if (you[0].running == 2) you[0].running = 1;

 	if (you[0].level_type == 2 && (you[0].x_pos <= 21 || you[0].x_pos >= 61 || you[0].y_pos <= 15 || you[0].y_pos >= 54))
   	{
	    env[0].cloud_no = area_shift();
    	you[0].pet_target = MHITNOT;
#ifdef DEBUG
	    mpr("Shifting.");
		int igly = 0;
		int ig2 = 0;
		for (igly = 0; igly < ITEMS; igly ++)
		{
			if (it[0].iquant [igly] != 0) ig2 ++;
		}
		strcpy(info, "No of items present: ");
		itoa(ig2, st_prn, 10);
		strcat(info, st_prn);
		mpr(info);
		ig2 = 0;
		for (igly = 0; igly < MNST; igly ++)
		{
	 		if (mons [igly].m_class != -1) ig2 ++;
		}
		strcpy(info, "No of monsters present: ");
		itoa(ig2, st_prn, 10);
		strcat(info, st_prn);
		mpr(info);
		strcpy(info, "No of clouds present: ");
		itoa(cloud_no, st_prn, 10);
		strcat(info, st_prn);
		mpr(info);

#endif
   }


} // end of void move()



/*
TO DO:

Stealth in check_awaken

*/



