#pragma once
#include <string>
#include "defines.h"
// ($pellbinder) (c) D.G.S.E. 1998

// ****remeber***** must make an hardcopy of this sometime
//#define 
// bitfield
// leaves no skeleton? ("blob" monsters?)
// if weight=0 or zombie_size=0, this is always true
#define M_NO_FLAGS 0 // clear
#define M_NO_SKELETON (1<<0)
// resistances
#define M_RES_ELEC (1<<1)
#define M_RES_POISON (1<<2)
#define M_RES_FIRE (1<<3)
#define M_RES_HELLFIRE (1<<4)
#define M_RES_COLD (1<<5)
// invisible
#define M_INVIS (1<<6) // is created with invis enchantment set, and never runs out
// vulnerabilities
//#define M_ED_ELEC (1<<6) // never used
#define M_ED_POISON (1<<7) // ???  - - This flag is now (2.50) set for insects (LH)
#define M_ED_FIRE (1<<8)
#define M_ED_COLD (1<<9)
// casts spells?
#define M_SPELLCASTER (1<<10)
// hovering?
#define M_FLIES (1<<11) // will crash to ground if paralysed (wings)
#define M_LEVITATE (1<<12) // not if this is set
// infrared/radar/whatever?
#define M_SEE_INVIS (1<<13)
// killing this beast only gives 10 experience (makes sense for plants/fungi)
// must do this manually
#define M_NO_EXP_GAIN (1<<14)
#define M_SPEAKS (1<<15)

// holiness
#define H_HOLY -1
#define H_NORMAL 0
#define H_UNDEAD 1
#define H_DEMONIC 2
// corpse_thingy
#define C_NOCORPSE 0
#define C_CLEAN 1
#define C_CONTAMINATED 2
#define C_POISON 3
#define C_HCL 4 // your flesh starts to rot away...
// well, HCl will burn your flesh away. Anyway, couldn't find a better name for now
#define C_MUTATE 5 // makes you mutate, of course

// zombie size
#define Z_NOZOMBIE 0 // no zombie (and no skeleton)
#define Z_SMALL 1
#define Z_BIG 2
// Note - this should be set to 0 for classed monsters, eg orc wizard

// shout
#define S_RANDOM -1
#define S_SILENT 0
#define S_SHOUT 1 //1=shout
#define S_BARK 2 //2=bark
#define S_SHOUT2 3 //3=shout twice
#define S_ROAR 4 //4=roar
#define S_SCREAM 5 //5=scream
#define S_BELLOW 6 //6=bellow (?)
#define S_SCREECH 7 //7=screech
#define S_BUZZ 8 //8=buzz
#define S_MOAN 9 //9=moan
#define S_WHINE 10 //10=irritating whine (mosquito)
#define S_CROAK 11 //11=frog croak
// ai
// So far this only affects a) chance to see stealthy player and b) chance to
//  walk through damaging clouds (LH)
#define I_PLANT 0
#define I_INSECT 1
#define I_ANIMAL 2
#define I_NORMAL 3
#define I_HIGH 4

extern int mondatasize;
extern struct monsterentry {
	int mc; // monster number 
	int showchar,colour;
	const char *name/*[32]*/; // longest is 23 till now (31 is max alowed here)
	int bitfields;
	int weight ;
	// experience is calculated like this:
	// ((((max_hp / 7) + 1) * (mHD * mHD) + 1) * exp_mod) / 10
	//     ^^^^^^ see below at hpdice
        //   Note that this may make draining attacks less attractive (LH)
	int exp_mod ;
	
	int charclass ; //
	int holiness ; // -1=holy,0=normal,1=undead,2=very very evil
	
	int resist_magic ; // (positive is ??)
	// max damage in a turn is total of these four?
	int damage[4] ;
	
	// hpdice[4]: [0]=HD [1]=min_hp [2]=rand_hp [3]=add_hp
	// min hp = [0]*[1]+[3] & max hp = [0]*([1]+[2])+[3]
	// example: the Iron Golem, hpdice={15,7,4,0}
	// 	15*7 < hp < 15*(7+4), 
	//       105 < hp < 165
	// hp will be around 135 each time. (assuming an good random number generator)
	// !!!!!!! The system is exactly the same as before, only the way of writing changed !!!!
	int hpdice[4] ; // until we have monsters with 32767 hp,this is easily possible
	int AC ; // armour class
	int ev ; // evasion
	int speed ,speed_inc ; // duh!
	int sec ; // not used (250) most o/t time
	
	// eating the corpse: 1=clean,2=might be contaminated,3=poison,4=very bad
	int corpse_thingy ;
	// 0=no zombie, 1=small zombie (z) 107, 2=_BIG_ zombie (Z) 108
	int zombie_size ;
	// 0=silent, 1=shout, 2=bark (makes only sense for dogs), 
	// 3=shout twice, 4=rour, 5=scream, 6=bellow (?), 7=screech,
	// 8=buzz, 9=moan, -1=random one of (0-7)
	int shouts ;
	// AI things?
	int intel ; // 0=none, 1=worst...4=best
	int gmon_use ;
} mondata[];
// wow. this struct is only about 48 bytes, (excluding the name)


int mondamage(int mc, int rt);
void mon_init(int gmon_use [1000], int mcolour [1000]);
void def_letters(char letters [52] [1]);
int mon_resist_mag(int mc, int mhd);
int mons_res_fire(int mclass);
int mons_intel(int mclass);
int mons_res_poison(int mclass);
int mons_zombie_size(int mclass);
int mons_weight(int mclass);
int mons_corpse_thingy(int mclass);
int mons_charclass(int mcls);
int mons_res_cold(int mclass);
int mons_holiness(int mclass);
int mons_shouts(int mclass);
int mons_flies(int mclass);
int mons_see_invis(int mclass);
void define_monster(int k, struct monsters mns [MNST]);
int mons_exp_mod(int mclass);
int mons_skeleton(int mcls);
int mons_res_elec(int mclass);
int hit_points(int hit_dice, int min_hp, int rand_hp);
void mons_spell_list(int sec, int splist [6]);
int mons_char(int mc);
std::string moname(int mcl, int mench, int see_inv, int descrip);
int exper_value(int mclass, int mHD, int maxhp);

std::string monam(int mons_cla, int mons_e, int desc, int see_invis);


int mons_pan(int mcls); // is the monster to be found in pandemonium

int mons_flag(int mc,int bf);

