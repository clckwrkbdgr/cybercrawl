#pragma once
#include "defines.h"
#include "message.h"

extern char info [200];
extern char st_prn [20];

extern char str_pass [80];

extern char gmon_use [1000];
extern char mcolour [1000];
extern int show_green;


struct dist
{
 int move_x;
 int move_y;
 int target_x;
 int target_y;
 int prev_targ;
 int nothing;
};

struct bolt
{
 int range, type, colour, flavour, source_x, source_y;
 int damage, ench_power, hit, bx, by, target_x;
 int target_y;
/* char beam_flavour, wand_id;*/
 int wand_id;
 int aim_down;
 int thing_thrown;
/*, beam_type;*/
 int move_x, move_y;
 int trac_targ;
 int tracer_mons;
 int trac_hit_tamed;
 int trac_hit_mons;
 int tracer;
 int trac_targ_x, trac_targ_y;
/* int beam_hit;*/

 /* If a monster fired it, which monster? */
 int beam_source;

 char beam_name [40];

};

struct player
{
int turnover;
int prev_targ;

char your_name [30];

int species;
int run_x;
int run_y;
int running;

/*
Make these all functions:
int teleport;
int see_invis;
int res_fire;
int res_poison;
int res_cold;
int fast_run;
int sust_abil;
int deaths_door;
int magic_battery;
int spec_fire;
int spec_cold;
int spec_poison;
int prot_life;
int berserker;
int energy;*/
int special_wield;
int deaths_door;
int shock_shield;

int corpse_count;
int disease;

/* Function
int res_magic ;
*/
int max_level;

int x_pos;
int y_pos;

int hunger;
int hunger_inc;
int equip [NO_EQUIP];
/* list in player.cc */

int hp;
int hp_max;
int base_hp; // this is what is affected by necrophages etc
int base_hp2;
int ep;
int ep_max;
int base_ep;
int base_ep2;

int strength;
int intel;
int dex;
int max_strength;
int max_intel;
int max_dex;


/* xxx_ch variables : determine whether new value is printed next turn.
Set to 1 if value is changed. */
int hung_ch;
int hung_state; /* normal */
int burden_ch;
int hp_ch;
int ep_ch;
int strength_ch;
int intel_ch;
int dex_ch;
int xp_ch;
int AC_ch; /* remember that the AC shown = 10 - AC */
int gp_ch;
int evasion_ch;

/* Functions:
int AC;
int evasion;
int shield_class;
int rate_regen;
int ep_rate_regen;
*/

int incr_regen;
int ep_incr_regen;

int xp;
int xl;
int gp;
int clas;
char clasnam [30];
int speed;
int time_taken;

	int inv_class [52];
	int inv_type [52];
	int inv_plus [52];
	int inv_plus2 [52];
	int inv_dam [52];
	int inv_ident [52];
	int inv_col [52];
	int inv_quant [52];
	int inv_no; // number of items carried.

 int burden;
 int burden_state;
int spells [25];
int spell_no;
int spell_levels;
int char_direction ;
// 0 = going down
// 1 = going up!
int pet_target;

int your_level;

int duration [30]; // lots of durational things. Why didn't I do this for haste etc right from the start? Oh well.
/* There's a list of these in player.cc */

int invis ;
int conf;
int paralysis;
int slow ;
int haste ; // haste
int might ; // might
int lev ; // levitation
int poison ; // poison!!
int rotting ;
int berserker;

int attribute [30]; // various attributes, eg resist lightning
/* list in player.cc */

int is_undead;
/* 0 - is alive
   1 - is undead, but can still eat/drink etc (eg vampire)
   2 - is undead (mummy etc)*/

int delay_doing;
int delay_t;

int skills [50];
int practise_skill [50];
int skill_points [50];
int exp_available;

int item_description [5] [50];
int unique_items [50];
int unique_creatures [50];
int level_type;



int where_are_you;

int branch_stairs [30];

int religion;
int piety;
int gods [100];

int mutation [100];
int demon_pow [100];
int mpower;

int had_item [50];

int betrayal;

};

extern struct player you [1];


struct monsters
{
 int m_class;
 int m_hp;
 int m_hp_max;
 int m_HD;
 int m_AC;
 int m_ev;
 int m_speed;
 int m_speed_inc;
 int m_x;
 int m_y;
 int m_targ_1_x;
 int m_targ_1_y;
 int m_inv [8];
 int m_beh;
 int m_sec;
 int m_hit;
 int m_ench [3];
 int m_ench_1;
};

/* extern struct monsters mons [MNST]; */

struct item_struct
{
 int iclass [ITEMS]; /* basic class (eg weapon) */
 int itype [ITEMS]; /* type within that class (eg dagger) */
 int iplus [ITEMS]; /* hit+, charges */
 int iplus2 [ITEMS]; /* dam+ etc */
 int idam [ITEMS]; /* special stuff */
 int iquant [ITEMS]; /* multiple items */
 int ix [ITEMS]; /*  x-location */
 int iy [ITEMS]; /* y-location */
 int icol [ITEMS]; /* colour */
 int iid [ITEMS]; /* identification */
 int ilink [ITEMS]; /* next item in stack */
};

/*extern struct item_struct it [1]; */


struct environ
{
 item_struct it [1];
 monsters mons [MNST];
 int grid [GXM] [GYM];
 int mgrid [GXM] [GYM];
 int igrid [GXM] [GYM];
 int map [GXM] [GYM];
 int cgrid [GXM] [GYM];
 int show [19] [19];
 int show_col [19] [19];
 int rock_colour;
 int floor_colour;

 int cloud_x [CLOUDS];
 int cloud_y [CLOUDS];
 int cloud_type [CLOUDS];
 int cloud_decay [CLOUDS];
 int cloud_no;

 int keeper_name [5] [3];
 int sh_x [5];
 int sh_y [5];
 int sh_greed [5];
 int sh_type [5];
 int sh_level [5];

 int trap_type [NTRAPS];
 int trap_x [NTRAPS];
 int trap_y [NTRAPS];
 int mons_alloc [20];
 int trap_known;

};

extern struct environ env [1]; // make this global to main module

struct ghost_struct
{
 char gname [20];
 int ghs [20];
};

extern struct ghost_struct ghost;

