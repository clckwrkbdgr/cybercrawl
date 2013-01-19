/* This file was optimized & heavily rewritten by Alexey Guzeev */
#include "linuxlib.h"

#include "externs.h"
#include "enum.h"

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "dungeon.h"
#include "files.h"
#include "itemname.h"
#include "message.h"
#include "misc.h"
#include "mstruct.h"
#include "mstuff2.h"
#include "output.h"
#include "player.h"
#include "randart.h"
#include "skills2.h"
#include "stuff.h"
#include "view.h"

enum { NO_SPELL_FOUND = 250 };

/*
Order for looking for conjurations for the 1st & 2nd spell slots,
when finding spells to be remembered by a player's ghost:
*/
int search_order_conj [] = {
/* 0 */
54, // crystal spear
53, // -ve energy
148, // agony
151, // disintegrate
17, // lightning
79, // sticky flame
60, // isk's blast
15, // firebolt
16, // cold bolt
6, // fireball
/* 10 */
35, // venom bolt
128, // iron
129, // stone arrow
26, // thr fl
27, // thr fr
67, // pain
115, // sting
5, // m dart
NO_SPELL_FOUND, // terminate search
};

/*
Order for looking for summonings and self-enchants for the 3rd spell slot:
*/
int search_order_third [] = {
/* 0 */
158, // symbol of torment
121, // summon gr dmn
72, // summon wraiths
62, // summon hor th
119, // summon demon
120, // demonic horde
22, // haste
28, // animate dead
25, // invis
82, // call imp
49, // summon small mammal
/* 10 */
28, // controlled blink
59, // blink
NO_SPELL_FOUND, // end search
};


/*
Order for looking for enchants for the 4th + 5th spell slot. If fails, will
go through conjs.
Note: Dig must be in misc2 (5th) position to work.
*/
int search_order_misc [] = {
/* 0 */
148, // agony - evil spell
113, // banishment
23, // paralyse
24, // confuse
21, // slow
20, // polymorph other
37, // teleport other
14, // dig
NO_SPELL_FOUND, // end search
};

class FileWriter {
public:
	FileWriter(const std::string & filename)
		: valid(false), file(NULL)
	{
		file = fopen(filename.c_str(), "wb");
		if(file == NULL) {
			return;
		}
		valid = true;
	}
	virtual ~FileWriter() {
		fclose(file);
	}
	bool is_valid() { return valid; }
	bool skip_bytes(size_t count) {
		char bytes[count];
		for(size_t i = 0; i < count; ++i) {
			bytes[i] = 0;
		}
		size_t written = fwrite(&bytes, 1, count, file);
		return (written == count);
	}
	bool str_value(std::string string, size_t count) {
		for(size_t i = 0; i < count; ++i) {
			if(i < string.size()) {
				char_value(string[i]);
			} else {
				char ch = 0;
				char_value(ch);
			}
		}
		return true;
	}
	bool char_value(char value) {
		size_t written = fwrite(&value, sizeof(value), 1, file);
		return (written == 1);
	}
	bool int_value(int value) {
		size_t written = fwrite(&value, sizeof(value), 1, file);
		return (written == 1);
	}
	bool unrandart(int value) {
		return int_value(does_unrandart_exist(value));
	}
	bool identity(int type, int value) {
		int identy[4][50];
		save_id(identy); // TODO Just awful.
		return int_value(identy[type][value]);
	}
private:
	FileWriter(FileWriter&) : valid(false), file(NULL) {}
	FileWriter & operator=(FileWriter&) { return * this; }
	bool valid;
	FILE * file;
};

class FileReader {
public:
	FileReader(const std::string & filename)
		: valid(false), file(NULL)
	{
		file = fopen(filename.c_str(), "rb");
		if(file == NULL) {
			return;
		}
		valid = true;
	}
	virtual ~FileReader() {
		fclose(file);
	}
	bool is_valid() { return valid; }
	bool skip_bytes(size_t count) {
		char zero = 0;
		size_t read = fread(&zero, 1, count, file);
		return (read == count);
	}
	bool str_value(std::string & string, int count) {
		for(int i = 0; i < count; ++i) {
			char ch;
			char_value(ch);
			if(ch) {
				string += ch;
			}
		}
		return true;
	}
	bool char_value(char & value) {
		size_t read = fread(&value, sizeof(value), 1, file);
		return (read == 1);
	}
	bool int_value(int & value) {
		size_t read = fread(&value, sizeof(value), 1, file);
		return (read == 1);
	}
	bool unrandart(int value) {
		int v;
		bool result = int_value(v);
		set_unrandart_exist(value, v);
		return result;
	}
	bool identity(int type, int value) {
		int ch;
		bool result = int_value(ch);
		switch (type) {
			case 0: set_id(OBJ_WANDS, value, ch); break;
			case 1: set_id(OBJ_SCROLLS, value, ch); break;
			case 2: set_id(OBJ_JEWELLERY, value, ch); break;
			case 3: set_id(OBJ_POTIONS, value, ch); break;
		}
		return result;
	}
private:
	FileReader(FileReader&) : valid(false), file(NULL) {}
	FileReader & operator=(FileReader&) { return * this; }
	bool valid;
	FILE * file;
};

int write2(FILE *file, char *buffer, int count);
int read2(FILE *file, char *buffer, int count);

static void reset_ch () {
  you[0].hp_ch = 1;
  you[0].ep_ch = 1;
  you[0].strength_ch = 1;
  you[0].intel_ch = 1;
  you[0].dex_ch = 1;
  you[0].AC_ch =  1;
  you[0].evasion_ch = 1;
  you[0].xp_ch = 1;
  you[0].gp_ch = 1;
  you[0].hung_ch = 1;
  you[0].hung_state = 3;
}


struct ghost_struct ghost;


extern int your_sign; /* these two are defined in view.cc */
extern int your_colour;

int translate_spell(int spel);
int search_spell_list(int * spell_list, int ignore_spell = NO_SPELL_FOUND, int * secondary_spell_list = NULL);
bool find_spell(int which_sp);
void add_spells(int buffer [40]);
void generate_random_demon(void);

static void save_int (char **pp, int val, int digits) {
  char *p=*pp;
  std::string thing_quant;
  int add=1;
  for (int i=1; i<digits; ++i) add*=10;
  thing_quant = to_string(val+add);
  for (int i=0; i<digits; ++i) *p++=thing_quant[i];
  *pp=p;
}


static int load_int (char **pp, int digits) {
  char *p=*pp;
  char thing_quant[8];
  int add=1;
  thing_quant[0]=*p++;
  for (int i=1; i<digits; ++i) {
    thing_quant[i]=*p++;
    add*=10;
  }
  *pp=p;
  thing_quant[digits]=0;
  return atoi(thing_quant)-add;
}


void load (int stair_taken, char moving_level, char was_a_labyrinth, char old_level, char want_followers, char just_made_new_lev, char where_were_you2) {
  int j = 0;
  int i, count_x, count_y;
  char cha_fil [80];

  char corr_level [4];

  char already_saved = 0;

  int foll_class [8];
  int foll_hp [8];
  int foll_hp_max [8];
  int foll_HD [8];
  int foll_AC [8];
  char foll_ev [8];
  int foll_speed [8];
  int foll_speed_inc [8];

  int foll_targ_1_x [8];
  int foll_targ_1_y [8];
  int foll_inv [8] [8];
  int foll_beh [8];
  int foll_sec [8];
  int foll_hit [8];

  int foll_ench [8] [3];
  int foll_ench_1 [8];

  int fit_iclass [8] [8];
  int fit_itype [8] [8];
  int fit_iplus [8] [8];
  int fit_iplus2 [8] [8];
  int fit_idam [8] [8];
  int fit_iquant [8] [8];
  int fit_icol [8] [8];
  char fit_iid [8] [8];

  int itmf = 0;
  int ic = 0;

  strcpy(corr_level, "");
  if (you[0].your_level<10) strcpy(corr_level, "0");
  std::string istr = to_string(you[0].your_level);
  strcat(corr_level, istr.c_str());
  corr_level [2] = you[0].where_are_you + 97;
  corr_level [3] = 0; /* null-terminate it */
  strncpy(cha_fil, you[0].your_name.c_str(), 6);
  cha_fil [6] = 0;
  strcat(cha_fil, ".");
  if (you[0].level_type!=0) strcat(cha_fil, "lab"); /* temporary level */
  else strcat(cha_fil, corr_level);

  you[0].prev_targ=MHITNOT;

  int following = -1;
  int fmenv = 0;

  int minvc = 0;

  if (moving_level==1) {
    for (int clouty=0; clouty<CLOUDS; ++clouty) {
      env[0].cloud_type[clouty]=0;
      env[0].cgrid[env[0].cloud_x [clouty]] [env[0].cloud_y [clouty]]=CNG;
      env[0].cloud_decay[clouty]=0;
      --env[0].cloud_no;
    }
    env[0].cloud_no=0;
  }

  if (want_followers == 1 && just_made_new_lev == 0) {
    for (count_x = you[0].x_pos - 1; count_x < you[0].x_pos + 2; count_x ++) {
      for (count_y = you[0].y_pos - 1; count_y < you[0].y_pos + 2; count_y ++) {
        if (count_x == you[0].x_pos && count_y == you[0].y_pos) continue;
        following ++;
        foll_class [following] = -1;
        if (mgrd [count_x] [count_y] == MNG) continue;
        fmenv = mgrd [count_x] [count_y];
        if ((menv[fmenv].m_class==400) || (menv[fmenv].m_class==19) || (menv[fmenv].m_class == 401) || (menv[fmenv].m_class==56) || (menv[fmenv].m_class==-1)) continue;
        if (menv [fmenv].m_class >= MLAVA0) continue;
        if (menv [fmenv].m_speed_inc < 50) continue;
        foll_class [following] = menv [fmenv].m_class;

        foll_hp [following] = menv [fmenv].m_hp;
        foll_hp_max [following] = menv [fmenv].m_hp_max;
        foll_HD [following] = menv [fmenv].m_HD;
        foll_AC [following] = menv [fmenv].m_AC;
        foll_ev [following] = menv [fmenv].m_ev;
        foll_speed [following] = menv [fmenv].m_speed;
        foll_speed_inc [following] = menv [fmenv].m_speed_inc;
        foll_targ_1_x [following] = menv [fmenv].m_targ_1_x;
        foll_targ_1_y [following] = menv [fmenv].m_targ_1_y;
        for (minvc=0; minvc<8; ++minvc) foll_inv[following][minvc]=501;
        for (minvc=0; minvc<8; ++minvc) {
          if (menv[fmenv].m_inv[minvc]==501) {
            fit_iquant[following][minvc]=0;
            continue;
          }
          fit_iclass [following] [minvc] = mitm.iclass [menv [fmenv].m_inv [minvc]];
          fit_itype [following] [minvc] = mitm.itype [menv [fmenv].m_inv [minvc]];
          fit_iplus [following] [minvc] = mitm.iplus [menv [fmenv].m_inv [minvc]];
          fit_iplus2 [following] [minvc] = mitm.iplus2 [menv [fmenv].m_inv [minvc]];
          fit_idam [following] [minvc] = mitm.idam [menv [fmenv].m_inv [minvc]];
          fit_iquant [following] [minvc] = mitm.iquant [menv [fmenv].m_inv [minvc]];
          fit_icol [following] [minvc] = mitm.icol [menv [fmenv].m_inv [minvc]];
          fit_iid [following] [minvc] = mitm.iid [menv [fmenv].m_inv [minvc]];
          destroy_item(menv [fmenv].m_inv [minvc]);
        }

        foll_beh [following] = menv [fmenv].m_beh;
        foll_sec [following] = menv [fmenv].m_sec;
        foll_hit [following] = menv [fmenv].m_hit;
        foll_ench_1 [following] = menv [fmenv].m_ench_1;
        foll_ench [following] [0] = menv [fmenv].m_ench [0];
        foll_ench [following] [1] = menv [fmenv].m_ench [1];
        foll_ench [following] [2] = menv [fmenv].m_ench [2];

        /* now for the icky part:*/
        for (j=0; j<3; ++j) menv [fmenv].m_ench[j]=0;
        menv [fmenv].m_ench_1 = 0;
        menv [fmenv].m_class = -1;
        menv [fmenv].m_hp = 0;
        menv [fmenv].m_hp_max = 0;
        menv [fmenv].m_HD = 0;
        menv [fmenv].m_AC = 0;
        menv [fmenv].m_ev = 0;
        mgrd [count_x] [count_y] = MNG;
      }
    }
    if (was_a_labyrinth == 0) save_level(old_level, 0, where_were_you2);
    already_saved = 1;
    was_a_labyrinth = 0;
  } /* end if level_type == 0*/

  strcpy(ghost.gname, "");
  for (ic=0; ic<20; ++ic) ghost.ghs [ic] = 0;

  FILE *handle;
  int handle2 = open(cha_fil, O_RDONLY, O_CREAT | O_TRUNC | O_BINARY, 0660);
  if (handle2 != -1)
  {
   	 close(handle2);
     handle = fopen(cha_fil, "rb");
  } else
//  if (handle2==-1)
    { /* generate new level */
    strcpy(ghost.gname, "");
    for (int imn=0; imn<20; ++imn) ghost.ghs[imn]=0;

    builder(you[0].your_level, you[0].level_type);

    if (you[0].level_type == 3) generate_random_demon();

    if (random2(3) == 0 && you[0].your_level > 1)
    {
      strcpy(corr_level, "");
      if (you[0].your_level<10) strcpy(corr_level, "0");
      strcat(corr_level, to_string(you[0].your_level).c_str());
      corr_level[2]=you[0].where_are_you+97;
      corr_level [3] = 0; /* null-terminate it */
      strcpy(cha_fil, "bones.");
      if (you[0].level_type!=0) strcat(cha_fil, "lab"); /* temporary level */
      else strcat(cha_fil, corr_level);

      int gfile2 = open(cha_fil, S_IWRITE, S_IREAD);

      if (gfile2!=-1) {
        close(gfile2);
//        gfile = open(cha_fil, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0660);
//        gfile = open(cha_fil, O_RDONLY, O_CREAT | O_TRUNC | O_BINARY, 0660);
       FILE *gfile = fopen(cha_fil, "rb");
        if (gfile==NULL) {
          msg("Error opening ghost file: @1") << cha_fil;
          more();
        } else {
          char buf1[40];
          read2(gfile, buf1, 40);
          fclose(gfile);
          for (int iiii=0; iiii<20; ++iiii) ghost.gname[iiii]=buf1[iiii];
          ghost.ghs[0]=buf1[20];
          ghost.ghs[1]=buf1[21];
          ghost.ghs[2]=buf1[22];
          ghost.ghs[3]=buf1[23];
          ghost.ghs[4]=buf1[24];
          ghost.ghs[5]=buf1[25];
          ghost.ghs[6]=buf1[26];
          ghost.ghs[7]=buf1[27];
          ghost.ghs[8]=buf1[28];
          /* note - as ghosts, automatically get res poison + prot_life */
          ghost.ghs[9]=buf1[29];
          ghost.ghs[10]=buf1[30];
          ghost.ghs[11]=buf1[31];
          ghost.ghs[12]=buf1[32];
          ghost.ghs[13]=buf1[33];

          ghost.ghs[14]=buf1[34];
          ghost.ghs[15]=buf1[35];
          ghost.ghs[16]=buf1[36];
          ghost.ghs[17]=buf1[37];
          ghost.ghs[18]=buf1[38];
          ghost.ghs[19]=buf1[39];
          unlink(cha_fil);
          for (int imn = 0; imn < MNST - 10; imn ++) {
            if (menv [imn].m_class!=-1) continue;
            menv [imn].m_class = 400;
            menv [imn].m_HD = ghost.ghs [12];
            menv [imn].m_hp = ghost.ghs [0];
            menv [imn].m_hp_max = ghost.ghs [0];
            menv [imn].m_AC = ghost.ghs [2];
            menv [imn].m_ev = ghost.ghs [1];
            menv [imn].m_speed = 10;
            menv [imn].m_speed_inc = 70;
            if (ghost.ghs [14] != 250 || ghost.ghs [15] != 250 || ghost.ghs [16] != 250 || ghost.ghs [17] != 250 || ghost.ghs [18] != 250 || ghost.ghs [19] != 250)
              menv [imn].m_sec = 119; else menv [imn].m_sec = 250;
            do {
              menv [imn].m_x = random2(60) + 10;
              menv [imn].m_y = random2(50) + 10;
            } while ((grd[menv[imn].m_x][menv[imn].m_y]!=67) || (mgrd[menv[imn].m_x][menv[imn].m_y]!=MNG));
            mgrd [menv [imn].m_x] [menv [imn].m_y] = imn;
            break;
          }
        }
      }
    }

    for (i = 0; i < GXM; i ++) {
      for (j = 0; j < GYM; j ++) {
        env[0].map [i] [j] = 0;
        if ((you[0].char_direction==1) && (you[0].level_type!=3)) {
          /* closes all the gates if you're on the way out */
          if ((grd[i][j]==69) || (grd[i][j]==96) || (grd[i][j]==99)) grd[i][j]=98;
        }
        env[0].cgrid [i] [j] = CNG;
      }
    }

    for (i = 0; i < MNST; i ++) {
      if (menv[i].m_class==255) menv[i].m_class=-1;
    }

    if (just_made_new_lev == 0) {
      if (stair_taken == 69 || stair_taken == 81) stair_taken = 86;
      else if (stair_taken < 86) stair_taken += 4;
      else if (stair_taken >= 130 && stair_taken < 150) stair_taken -= 20;
      else if (stair_taken >= 110 && stair_taken < 130) stair_taken += 20;
      else if (stair_taken > 90) stair_taken = 86;
      else if (stair_taken == 67) stair_taken = 67;
      else stair_taken -= 4;

      for (count_x = 0; count_x < GXM; count_x ++) {
        for (count_y = 0; count_y < GYM; count_y ++) {
          if (grd [count_x] [count_y] == stair_taken) goto found_stair;
        }
      }

      if (stair_taken < 86) stair_taken=82;
      else stair_taken=86;

      for (count_x = 0; count_x < GXM; count_x ++) {
        for (count_y = 0; count_y < GYM; count_y ++) {
          if (grd [count_x] [count_y] == stair_taken) goto found_stair;
        }
      }
      for (count_x = 0; count_x < GXM; count_x ++)
      {
        for (count_y = 0; count_y < GYM; count_y ++)
        {
          if (grd [count_x] [count_y] == 67) goto found_stair;
        }
      }
    }

found_stair :

    if (just_made_new_lev == 0) {
      you[0].x_pos = count_x;
      you[0].y_pos = count_y;
    }

    if ((you[0].level_type==1) || (you[0].level_type==2)) grd[you[0].x_pos][you[0].y_pos]=67;

    following = 0;
    fmenv = -1;

    if (((you[0].level_type==0) || (you[0].level_type==3)) && (want_followers==1) && (just_made_new_lev==0)) {
      for (ic = 0; ic < 2; ic ++) {
        for (count_x = you[0].x_pos - 6; count_x < you[0].x_pos + 7; count_x ++) {
          for (count_y = you[0].y_pos - 6; count_y < you[0].y_pos + 7; count_y ++) {
            if ((ic==0) &&
                (
                 (count_x<you[0].x_pos-1) ||
                 (count_x>you[0].x_pos+1) ||
                 (count_y<you[0].y_pos-1) ||
                 (count_y>you[0].y_pos+1)
                )
               ) continue;
            if ((count_x==you[0].x_pos) && (count_y==you[0].y_pos)) continue;
            if ((mgrd[count_x][count_y]!=MNG) || (grd[count_x][count_y]<67)) continue;
            while (menv [following].m_class != -1) {
             following ++;
             if (following >= MNST) goto out_of_foll;
            }

            while (fmenv < 7) {
              fmenv ++;
              if (foll_class [fmenv] == -1) continue;
              menv [following].m_class = foll_class [fmenv];
              menv [following].m_hp = foll_hp [fmenv];
              menv [following].m_hp_max = foll_hp_max [fmenv];
              menv [following].m_HD = foll_HD [fmenv];
              menv [following].m_AC = foll_AC [fmenv];
              menv [following].m_ev = foll_ev [fmenv];
              menv [following].m_speed = foll_speed [fmenv];
              menv [following].m_speed_inc = foll_speed_inc [fmenv];
              menv [following].m_x = count_x;
              menv [following].m_y = count_y;
              menv [following].m_targ_1_x = 0;
              menv [following].m_targ_1_y = 0;
              for (minvc = 0; minvc < 8; minvc ++) {
                if (fit_iquant[fmenv][minvc]==0) {
                  menv [following].m_inv [minvc] = 501;
                  continue;
                }
                itmf = 0;
                while (mitm.iquant[itmf]>0) ++itmf;
                menv [following].m_inv [minvc] = itmf;
                mitm.iclass [itmf] = fit_iclass [fmenv] [minvc];
                mitm.itype [itmf] = fit_itype [fmenv] [minvc];
                mitm.iplus [itmf] = fit_iplus [fmenv] [minvc];
                mitm.iplus2 [itmf] = fit_iplus2 [fmenv] [minvc];
                mitm.idam [itmf] = fit_idam [fmenv] [minvc];
                mitm.iquant [itmf] = fit_iquant [fmenv] [minvc];
                mitm.icol [itmf] = fit_icol [fmenv] [minvc];
                mitm.iid [itmf] = fit_iid [fmenv] [minvc];
                mitm.ilink [itmf] = 501;
              }
              menv [following].m_beh = foll_beh [fmenv];
              menv [following].m_sec = foll_sec [fmenv];
              menv [following].m_hit = foll_hit [fmenv];
              menv [following].m_ench_1 = foll_ench_1 [fmenv];
              menv [following].m_ench [0] = foll_ench [fmenv] [0];
              menv [following].m_ench [1] = foll_ench [fmenv] [1];
              menv [following].m_ench [2] = foll_ench [fmenv] [2];
              mgrd [count_x] [count_y] = following;
              break;
            }
          }
        }
      }
    } /* end if level_type == 0 */
out_of_foll :

    reset_ch();

    moving_level = 0;

    for (i = 0; i < MNST; i++) {
      if (menv [i].m_class == -1) continue;
      for (j = 0; j < 8; j ++) {
        if (menv [i].m_inv [j] == 501) continue;
        if (mitm.ilink [menv [i].m_inv [j]] != 501) {
          /* items carried by monsters shouldn't be linked */
          mitm.ilink [menv [i].m_inv [j]] = 501;
        }
      }
    }

    if (you[0].level_type == 3) {
      for (count_x = 0; count_x < GXM; count_x ++) {
        for (count_y = 0; count_y < GYM; count_y ++) {
          if ((grd[count_x][count_y]>=86) && (grd[count_x][count_y]<=89)) {
            grd [count_x] [count_y] = 67;
            if (random2(30) == 0) grd [count_x] [count_y] = 100;
          }
          if ((grd[count_x][count_y]>=81) && (grd[count_x][count_y]<=85)) {
            grd [count_x] [count_y] = 101;
          }
        }
      }
    }

    save_level(you[0].your_level, (you[0].level_type != 0), you[0].where_are_you);
    return;
  }

  moving_level = 0;

  for (count_x = 0; count_x < ITEMS; count_x ++) mitm.ilink[count_x]=501;

  for (i = 0; i < GXM; i ++) {
    for (j = 0; j < GYM; j ++) igrd [i] [j] = 501;
  }

  const int datalen=20+20+4*80*70+3*NTRAPS+25*ITEMS+1+9*CLOUDS+5*8+5*20+(18+5+5+5+5+8*5)*MNST;
  char *buf=(char*)malloc(datalen);
  char *p=buf;
  int retval=read2(handle, buf, datalen);
  if (datalen!=retval) {
    perror("opa (7)...");
	Format format(EOL"Wanted to read @1 bytes; could only read @2.");
	format << datalen << retval;
    cprintf(format.str().c_str());
    end(-1);
  }
  fclose(handle);

  for (i=0; i<20; ++i) ghost.gname[i]=*p++;
  for (i=0; i<20; ++i) ghost.ghs[i]=*p++;
//  for (j=0; j<20; ++j) ghost.ghs[j]-=30;

  for (count_x = 0; count_x < GXM; count_x ++) {
    for (count_y = 0; count_y < GYM; count_y ++) {
      grd[count_x][count_y]=*p++;
      env[0].map[count_x][count_y]=*p++;
      if (env[0].map [count_x] [count_y] == 201) env[0].map [count_x] [count_y] = 239;
      mgrd [count_x] [count_y] = MNG; ++p;
      if ((mgrd[count_x][count_y]!=MNG) &&
          (
           (menv[mgrd[count_x][count_y]].m_class==-1) ||
           (menv[mgrd[count_x][count_y]].m_x!=count_x) ||
           (menv[mgrd[count_x][count_y]].m_y!=count_y)
          )) {
        mgrd [count_x] [count_y] = MNG; /* This is one of the worst things I've ever done */
      }
      env[0].cgrid[count_x][count_y]=*p++;
    }
  }

  for (i = 0; i < NTRAPS; i ++) {
    env[0].trap_type[i]=*p++;
    env[0].trap_x[i]=*p++;
    env[0].trap_y[i]=*p++;
  }

  for (count_x = 0; count_x < GXM; count_x ++) {
    for (count_y = 0; count_y < GYM; count_y ++) {
      if ((igrd[count_x][count_y]<0) || (igrd[count_x][count_y]>501)) {
        igrd [count_x] [count_y] = 501;
      }
    }
  }

  for (i=0; i<ITEMS; ++i) {
    mitm.iclass[i]=*p++;
    mitm.itype[i]=*p++;
    mitm.iplus[i]=*p++;
    mitm.idam[i]=*p++;
    mitm.iquant[i]=load_int(&p, 6);
    mitm.icol[i]=*p++;
    mitm.ix[i]=*p++;
    mitm.iy[i]=*p++;
    mitm.iid[i]=*p++;
    mitm.ilink[i]=load_int(&p, 5)-40000;
    igrd[mitm.ix[i]][mitm.iy[i]]=load_int(&p, 5)-40000;
    mitm.iplus2[i]=*p++;
    if (mitm.iclass[i]==100) {
      mitm.iquant[i]=0;
      mitm.ilink[i]=501;
    }
  }

  env[0].cloud_no=*p++;

  for (i=0; i<CLOUDS; ++i) {
    env[0].cloud_x[i]=*p++;
    env[0].cloud_y[i]=*p++;
    env[0].cloud_type[i]=*p++;
    env[0].cloud_decay[i]=load_int(&p, 5);
    ++p;
  }

  for (i=0; i<5; ++i) {
    env[0].keeper_name[i][0]=*p++;
    env[0].keeper_name[i][1]=*p++;
    env[0].keeper_name[i][2]=*p++;
    env[0].sh_x[i]=*p++;
    env[0].sh_y[i]=*p++;
    env[0].sh_greed[i]=*p++;
    env[0].sh_type[i]=*p++;
    env[0].sh_level[i]=*p++;
    if (mgrd[env[0].sh_x[i]-1][env[0].sh_y[i]-1]==31) {
      cprintf("x");
      getkey();
    }
  }

  for (i = 0; i < 20; i ++) env[0].mons_alloc[i]=load_int(&p, 5)-10000;

  for (count_x=0; count_x<MNST; ++count_x) {
    p+=3;
    menv[count_x].m_AC=*p++;
    menv[count_x].m_ev=*p++;
    menv[count_x].m_HD=*p++;
    menv[count_x].m_speed=*p++;
    menv[count_x].m_speed_inc=*p++;
    menv[count_x].m_beh=*p++;
    menv[count_x].m_x=*p++;
    menv[count_x].m_y=*p++;
    menv[count_x].m_targ_1_x=*p++;
    menv[count_x].m_targ_1_y=*p++;
    ++p;
    menv[count_x].m_ench_1=*p++;
    for (j=0; j<3; ++j) menv[count_x].m_ench[j]=*p++;
    menv[count_x].m_class=load_int(&p, 5)-40080;
    menv[count_x].m_hp=load_int(&p, 5)-40000;
    menv[count_x].m_hp_max=load_int(&p, 5)-40000;
    menv[count_x].m_sec=load_int(&p, 5)-40000;
    for (j=0; j<8; ++j) menv[count_x].m_inv[j]=load_int(&p, 5)-40000;
    for (j=0; j<MNST; ++j) {
      if (menv[j].m_class!=-1) mgrd[menv[j].m_x][menv [j].m_y]=j;
    }
  }

  reset_ch();

  free(buf);
  if (p!=buf+datalen) {
    perror("opa (6)...");
    end(-1);
  }

  for (i = 0; i < GXM; i ++) {
    for (j = 0; j < GYM; j ++) {
      if (igrd [i] [j] < 0 || igrd [i] [j] > 501) igrd [i] [j] = 501;
    }
  }
  for (i = 0; i < MNST; i ++) {
    for (j = 0; j < 8; j ++) {
      if ((menv[i].m_inv[j]<0) || (menv[i].m_inv[j]>501)) menv[i].m_inv[j]=501;
      if (menv [i].m_inv [j] != 501) mitm.ilink [menv [i].m_inv [j]] = 501;
    }
  }
  for (i = 0; i < ITEMS; i ++) {
    if (mitm.ilink [i] > 501) mitm.ilink [i] = 501;
  }
  for (i = 0; i < MNST; i++) {
    if (menv [i].m_class == -1) continue;
    for (j = 0; j < 8; j ++) {
      if (menv [i].m_inv [j] == 501) continue;
      if (mitm.ilink [menv [i].m_inv [j]] != 501) {
        mitm.ilink [menv [i].m_inv [j]] = 501;
      }
    }
  }
  if (you[0].your_level == 35 && stair_taken >= 86) {
    do {
     you[0].x_pos = 10 + random2(GXM - 10);
     you[0].y_pos = 10 + random2(GYM - 10);
    } while ((grd[you[0].x_pos][you[0].y_pos]!=67) || (mgrd[you[0].x_pos][you[0].y_pos]!=MNG));
    count_x = you[0].x_pos;
    count_y = you[0].y_pos;
    goto found_stair;
  } else {
	if (stair_taken == 67)
     for (count_x = 0; count_x < GXM; count_x ++)
     {
       for (count_y = 0; count_y < GYM; count_y ++)
       {
         if (grd [count_x] [count_y] == stair_taken) goto found_stair;
       }
     }
    if (stair_taken >= 130 && stair_taken < 150) stair_taken -= 20;
    else if (stair_taken >= 110 && stair_taken < 130) stair_taken += 20;
    else if (stair_taken < 86) stair_taken += 4;
    else stair_taken -= 4;
    for (count_x = 0; count_x < GXM; count_x ++) {
      for (count_y = 0; count_y < GYM; count_y ++) {
        if (grd [count_x] [count_y] == stair_taken) goto found_stair;
      }
    }
    if (stair_taken < 86) stair_taken = 82;
    else stair_taken = 86;
    for (count_x = 0; count_x < GXM; count_x ++) {
      for (count_y = 0; count_y < GYM; count_y ++) {
        if (grd [count_x] [count_y] == stair_taken) goto found_stair;
      }
    }
  }
  for (count_x = 0; count_x < GXM; count_x ++) {
    for (count_y = 0; count_y < GYM; count_y ++) {
      if ((mgrd[count_x][count_y]!=MNG) &&
          (
           (menv[mgrd[count_x][count_y]].m_class==-1)  ||
           (menv[mgrd[count_x][count_y]].m_x!=count_x) ||
           (menv [mgrd [count_x] [count_y]].m_y != count_y)
          )
         ) {
        mgrd [count_x] [count_y] = MNG; /* This is one of the worst things I've ever done */
      }
    }
  }
} /* end of void load_level(); */


void save_level (int level_saved, char was_a_labyrinth, char where_were_you) {
  char cha_fil[20];
  char extens[5];
  int count_x, count_y;
  int i, j;

  strcpy(extens, "");
  if (level_saved < 10) strcpy(extens, "0");
  strcat(extens, to_string(level_saved).c_str());
  extens [2] = where_were_you + 97;
  extens [3] = 0; /* null-terminate it */
  strncpy(cha_fil, you[0].your_name.c_str(), 6);
  cha_fil [6] = 0;
  strcat(cha_fil, ".");
  if (was_a_labyrinth == 1) strcat(cha_fil, "lab"); /* temporary level */
  else strcat(cha_fil, extens);

  you[0].prev_targ=MHITNOT;

  int fry;
  int frx;

/* Setting up ix & iy, which aren't normally used: */

  for (frx = 0; frx < MNST; frx ++) {
    for (fry = 0; fry < 8; fry ++) {
      if (menv [frx].m_inv [fry] != 501) {
        mitm.ix [menv [frx].m_inv [fry]] = 2;
        mitm.iy [menv [frx].m_inv [fry]] = 2;
        mitm.ilink [menv [frx].m_inv [fry]] = 501;
      }
    }
  }

  for (count_x = 0; count_x < 80; count_x ++) {
    for (count_y = 0; count_y < 70; count_y ++) {

      int count_out = 0;

      if (igrd [count_x] [count_y] < 0 || igrd [count_x] [count_y] > 501) igrd [count_x] [count_y] = 501;
      if (igrd [count_x] [count_y] > 501) igrd [count_x] [count_y] = 501;
      if (igrd [count_x] [count_y] == 501) continue;

      frx = igrd [count_x] [count_y];

      while(frx != 501) {
        mitm.ix [frx] = count_x;
        mitm.iy [frx] = count_y;

        if (frx > 501 || frx < 0) {
			Format format("Error! Item out of bounds: @1");
			format << frx;
          cprintf(format.str().c_str());
          if (getkey() == 0) getkey();
          cprintf(EOL);
          break;
        }

        fry = mitm.ilink [frx];
        frx = fry;
        ++count_out;
        if (count_out > 1000) {
          count_out = 0;
          mitm.ilink [frx] = 501;
          mpr("Item link error.");
          break;
        }

        if (frx == 501) break;
      }
    }
  }

  const int datalen=20+20+4*80*70+3*NTRAPS+25*ITEMS+1+9*CLOUDS+5*8+5*20+(18+5+5+5+5+8*5)*MNST;
  char *buf=(char*)malloc(datalen);
  char *p=buf;

  int ghost_bak[20];
  for (j = 0; j < 20; j ++) ghost_bak [j] = ghost.ghs [j];
  for (i=0; i<20; ++i) *p++=ghost.gname[i];
  for (i=0; i<20; ++i) *p++=ghost_bak[i];

  for (count_x = 0; count_x < 80; count_x ++) {
    for (count_y = 0; count_y < 70; count_y++) {
      *p++=(grd [count_x] [count_y]);
      *p++=env[0].map [count_x] [count_y];
      *p++=mgrd [count_x] [count_y];
      *p++=env[0].cgrid [count_x] [count_y];
    }
  }

  for (i=0; i<NTRAPS; ++i) {
    *p++=env[0].trap_type [i];
    *p++=env[0].trap_x [i];
    *p++=env[0].trap_y [i];
  }

  for (i=0; i<ITEMS; ++i) {
    *p++=mitm.iclass[i];
    *p++=mitm.itype[i];
    *p++=mitm.iplus[i];
    *p++=mitm.idam[i];
    save_int(&p, mitm.iquant[i], 6);
    *p++=mitm.icol[i];
    *p++=mitm.ix[i];
    *p++=mitm.iy[i];
    *p++=mitm.iid[i];
    if (mitm.iquant[i]==0) mitm.ilink[i]=501;
    save_int(&p, mitm.ilink[i]+40000, 5);
    save_int(&p, igrd[mitm.ix[i]][mitm.iy[i]]+40000, 5);
    *p++=mitm.iplus2[i];
  }

  *p++=env[0].cloud_no;

  for (i = 0; i < CLOUDS; i ++) {
    *p++=env[0].cloud_x[i];
    *p++=env[0].cloud_y[i];
    *p++=env[0].cloud_type[i];
    save_int(&p, env[0].cloud_decay[i], 5);
    *p++=0;
  }

  for (i = 0; i < 5; i ++) {
    *p++=env[0].keeper_name [i] [0];
    *p++=env[0].keeper_name [i] [1];
    *p++=env[0].keeper_name [i] [2];
    *p++=env[0].sh_x [i];
    *p++=env[0].sh_y [i];
    *p++=env[0].sh_greed [i];
    *p++=env[0].sh_type [i];
    *p++=env[0].sh_level [i];
    if (mgrd [env[0].sh_x [i] - 1] [env[0].sh_y [i] - 1] == 31) {
      cprintf("y");
      getkey();
    }
  }

  for (i=0; i<20; ++i) save_int(&p, env[0].mons_alloc[i]+10000, 5);

  for (i = 0; i < MNST; i++) {
    *p++=5;
    *p++=5;
    *p++=5;
    *p++=menv [i].m_AC;
    *p++=menv [i].m_ev;
    *p++=menv [i].m_HD;
    *p++=(menv [i].m_speed);
    *p++=menv [i].m_speed_inc;
    *p++=menv [i].m_beh;
    *p++=menv [i].m_x;
    *p++=menv [i].m_y;
    *p++=menv [i].m_targ_1_x;
    *p++=menv [i].m_targ_1_y;
//    *p++=5;
    *p++=0;
    *p++=menv [i].m_ench_1;
    for (j = 0; j < 3; j++) *p++=menv [i].m_ench [j];
    save_int(&p, menv[i].m_class+40080, 5);
    save_int(&p, menv[i].m_hp+40000, 5);
    save_int(&p, menv[i].m_hp_max+40000, 5);
    save_int(&p, menv[i].m_sec+40000, 5);
    for (j = 0; j < 8; j ++) save_int(&p, menv[i].m_inv[j]+40000, 5);
  }

  if (p!=buf+datalen) {
    perror("opa (1)...");
    end(-1);
  }
  FILE *handle=fopen(cha_fil, "wb");
//open(cha_fil, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0660);
  if (handle==NULL) {
    perror("Oh dear... ");
    end(-1);
  }
  int retval=write2(handle, buf, datalen);
  free(buf);
  if (datalen!=retval) {
    perror("opa (2)...");
    end(-1);
  }
  fclose(handle);
}

template<class T>
void file_you(T & file)
{
	file.str_value(you[0].your_name, 30);
	file.int_value(you[0].religion);
	file.int_value(you[0].piety);
	file.int_value(you[0].invis);
	file.int_value(you[0].conf);
	file.int_value(you[0].paralysis);
	file.int_value(you[0].slow);
	file.int_value(you[0].shock_shield);
	file.int_value(you[0].rotting);
	file.skip_bytes(1);
	file.int_value(you[0].deaths_door);
	file.int_value(your_sign);
	file.int_value(your_colour);
	file.skip_bytes(1);
	file.int_value(you[0].pet_target);
	file.skip_bytes(1);
	file.skip_bytes(5);
	file.int_value(you[0].spell_levels);
	file.int_value(you[0].max_level);
	file.int_value(you[0].where_are_you);
	file.int_value(you[0].char_direction);
	file.int_value(you[0].your_level);
	file.int_value(you[0].is_undead);
	file.int_value(you[0].special_wield);
	file.int_value(you[0].berserker);
	file.skip_bytes(1);
	file.int_value(you[0].level_type);
	file.int_value(you[0].corpse_count);
	file.int_value(you[0].disease);
	file.skip_bytes(1);
	file.skip_bytes(1);
	file.int_value(you[0].species);
	file.int_value(you[0].hp);
	file.int_value(you[0].hp_max);
	file.int_value(you[0].haste);
	file.int_value(you[0].might);
	file.int_value(you[0].lev);
	file.int_value(you[0].poison);
	file.skip_bytes(1);
	file.int_value(you[0].hunger);
	file.skip_bytes(1);
	for(int i = 0; i < NO_EQUIP; ++i) {
		file.int_value(you[0].equip[i]);
	}
	file.int_value(you[0].ep);
	file.int_value(you[0].ep_max);
	file.int_value(you[0].strength);
	file.int_value(you[0].intel);
	file.int_value(you[0].dex);
	file.skip_bytes(1);
	file.skip_bytes(1);
	file.skip_bytes(1);
	file.skip_bytes(2);
	file.int_value(you[0].incr_regen);
	file.skip_bytes(3);
	file.skip_bytes(5);
	file.int_value(you[0].xp);
	file.int_value(you[0].gp);
	file.int_value(you[0].clas);
	file.int_value(you[0].xl);
	file.skip_bytes(14);
	file.int_value(you[0].exp_available);
	file.int_value(you[0].max_strength);
	file.int_value(you[0].max_intel);
	file.int_value(you[0].max_dex);
	file.int_value(you[0].hunger_inc);
	file.int_value(you[0].ep_incr_regen);
	file.skip_bytes(5);
	file.int_value(you[0].base_hp);
	file.int_value(you[0].base_hp2);
	file.int_value(you[0].base_ep);
	file.int_value(you[0].base_ep2);
	file.int_value(you[0].x_pos);
	file.int_value(you[0].y_pos);
	file.str_value(you[0].clasnam, 30);
	file.int_value(you[0].burden);

	you[0].spell_no = 0;
	for(int i = 0; i < 25; ++i) {
		file.int_value(you[0].spells[i]);
		if(you[0].spells[i] != 210) {
			++you[0].spell_no;
		}
	}

	you[0].inv_no = 0;
	for(int i = 0; i < 52; ++i) {
		file.int_value(you[0].inv_class[i]);
		file.int_value(you[0].inv_type[i]);
		file.int_value(you[0].inv_plus[i]);
		file.int_value(you[0].inv_dam[i]);
		file.int_value(you[0].inv_col[i]);
		file.int_value(you[0].inv_ident[i]);
		file.int_value(you[0].inv_quant[i]);
		file.int_value(you[0].inv_plus2[i]);
		if (you[0].inv_quant[i]>0) {
			++you[0].inv_no;
		}
	}

	for(int i = 0; i < 5; ++i) {
		for(int j = 0; j < 50; ++j) {
			file.int_value(you[0].item_description[i][j]);
		}
	}

	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 50; ++j) {
			file.identity(i, j);
		}
	}

	for(int j = 0; j < 50; ++j) {
		file.int_value(you[0].skills[j]);
	}
	for(int j = 0; j < 50; ++j) {
		file.int_value(you[0].practise_skill[j]);
	}
	for(int j = 0; j < 50; ++j) {
		file.int_value(you[0].skill_points[j]);
	}
	for(int j = 0; j < 50; ++j) {
		file.int_value(you[0].unique_items[j]);
	}
	for(int j = 0; j < 50; ++j) {
		file.int_value(you[0].unique_creatures[j]);
	}
	for(int j = 0; j < 30; ++j) {
		file.int_value(you[0].duration[j]);
	}
	for(int j = 0; j < 30; ++j) {
		file.int_value(you[0].attribute[j]);
	}
	for(int j = 0; j < 30; ++j) {
		file.int_value(you[0].branch_stairs[j]);
	}
	for(int j = 0; j < 100; ++j) {
		file.int_value(you[0].mutation[j]);
	}
	for(int j = 0; j < 50; ++j) {
		file.int_value(you[0].had_item[j]);
	}
	for(int j = 0; j < 100; ++j) {
		file.int_value(you[0].demon_pow[j]);
	}
	for(int j = 0; j < NO_UNRANDARTS; ++j) {
		file.unrandart(j);
	}
}

void save_game (bool leave_game)
{
	std::string filename = you[0].your_name;
	filename = (filename.size() > 6) ? std::string(filename, 0, 6) : filename;
	filename += ".sav";
	FileWriter file(filename);
	if(!file.is_valid()) {
		perror("Unable to open file for writing");
		end(-1);
	}

	file_you(file);

	if (!leave_game) return;

	if(you[0].level_type == 0) {
		save_level(you[0].your_level, 0, you[0].where_are_you);
	} else {
		save_level(you[0].your_level, 1, you[0].where_are_you);
	}

	clrscr();
	cprintf("See you soon!");
	end(0);
}

void restore_game()
{
	std::string filename = you[0].your_name;
	filename = (filename.size() > 6) ? std::string(filename, 0, 6) : filename;
	filename += ".sav";
	FileReader file(filename);
	if(!file.is_valid()) {
		perror("Unable to open file for reading");
		end(-1);
	}

	file_you(file);
}

int calculate_ghost_damage()
{
	int damage = you[0].skills[SK_UNARMED_COMBAT] + (you[0].species == SP_TROLL) ? (4 + you[0].xl) : 4;
	int weapon = you[0].equip[EQ_WEAPON];
	if (weapon!= -1) {
		if(you[0].inv_class[weapon] == 0) {
			damage = property(you[0].inv_class[weapon], you[0].inv_type[weapon], 0);
		}
		if(you[0].inv_class[weapon] == 11) {
			damage = 5;
		}
		if(you[0].inv_class[weapon] == 0 || you[0].inv_class[weapon] == 11) {
			damage *= 26 + you[0].skills[weapon_skill(you[0].inv_class[weapon], you[0].inv_type[weapon])];
			damage /= 25;
		}
	}
	damage *= 31 + you[0].skills[SK_FIGHTING];
	damage /= 30;
	damage += you[0].strength / 4;
	return (damage <= 50) ? damage : 50;
}

int calculate_ghost_evasion()
{
	int evasion = 4;
	int weapon = you[0].equip[EQ_WEAPON];
	if(weapon != -1) {
		if(you[0].inv_class[weapon] == 0) {
			if(you[0].inv_dam[weapon] < 180) {
				evasion=you[0].inv_dam[weapon] % 30;
			}
			if(you[0].inv_dam[weapon] % 30 >= 25) {
				evasion = randart_wpn_properties(you[0].inv_class[weapon], you[0].inv_type[weapon], you[0].inv_dam[weapon], you[0].inv_plus[weapon], you[0].inv_plus2[weapon], 0, RAP_BRAND);
			}
		}
	}
	return evasion;
}

bool file_exists(const std::string & filename)
{
	FILE *gfile = fopen(filename.c_str(), "rb");
	if(gfile != NULL) {
		fclose(gfile);
		return true;
	}
	return false;
}

bool save_int_array(const std::string & filename, int * array, int count)
{
	FILE * gfile = fopen(filename.c_str(), "wb");
	if(gfile == NULL) {
		return false;
	}
	fwrite((char*)array, 1, count, gfile);
	fclose(gfile);
	return true;
}

void save_ghost()
{
	if(you[0].your_level <= 1 || you[0].is_undead != 0)
		return;

	std::string file_ext = to_string(you[0].your_level, 2) + char(you[0].where_are_you + 'a');
	std::string filename = "bones." + ((you[0].level_type != 0) ? "lab" : file_ext);
	if(file_exists(filename)) {
		return;
	}

	FileWriter file(filename);
	if(!file.is_valid()) {
		msg("Error creating ghost file: @1") << filename;
		more();
		return;
	}

	int spells[6]; // Death is a traumatic experience, so ghosts only remember a few spells.
	spells[0] = translate_spell(search_spell_list(search_order_conj));
	spells[1] = translate_spell(search_spell_list(search_order_conj, spells[0]));
	spells[2] = translate_spell(search_spell_list(search_order_third));
	spells[3] = translate_spell(search_spell_list(search_order_misc, NO_SPELL_FOUND, search_order_conj));
	spells[4] = translate_spell(find_spell(14) ? 14 : search_spell_list(search_order_conj, spells[3], search_order_conj));
	spells[5] = translate_spell(find_spell(1) ? 1 : ((find_spell(28) || find_spell(59)) ? 28 : NO_SPELL_FOUND)); // Looks for blink/tport for emergency slot

	for(int i = 0; i < 20; ++i) {
		file.int_value(you[0].your_name[i]);
	}
	file.int_value((you[0].hp_max < 120) ? you[0].hp_max : 150);
	file.int_value(player_evasion());
	file.int_value(player_AC());
	file.int_value(player_see_invis());
	file.int_value(player_res_fire());
	file.int_value(player_res_cold()); // note - as ghosts, automatically get res poison + prot_life
	file.int_value(you[0].attribute[ATTR_RESIST_LIGHTNING]);
	file.int_value(calculate_ghost_damage());
	file.int_value(calculate_ghost_evasion());
	file.int_value(you[0].species);
	file.int_value(best_skill(0, 50, 99));
	file.int_value(you[0].skills[best_skill(0, 50, 99)]);
	file.int_value(you[0].xl);
	file.int_value(you[0].clas);
	for(int i = 0; i < 6; ++i) {
		file.int_value(spells[i]);
	}
}

int write2(FILE *file, char *buffer, int count)
{
	return fwrite(buffer, 1, count, file);
}

int read2(FILE *file, char *buffer, int count)
{
	return fread(buffer, 1, count, file);
}

int search_spell_list(int * spell_list, int ignore_spell, int * secondary_spell_list)
{
	for(int i = 0; spell_list[i] != NO_SPELL_FOUND; i ++) {
		if(spell_list[i] == ignore_spell) continue;
		if(find_spell(spell_list[i])) return spell_list[i];
	}
	if(secondary_spell_list) {
		return search_spell_list(secondary_spell_list, ignore_spell);
	}
	return NO_SPELL_FOUND;
}

/*
Searches for a specific spell, according to search order in the global arrays
defined start of file (top to bottom).
*/
bool find_spell(int which_sp)
{
	for(int i = 0; i < 21; i ++) {
		if(you[0].spells [i] == which_sp) {
			return true;
		}
	}
	return false;
}

// When passed the number for a player spell, returns the equivalent monster
//spell. Returns NO_SPELL_FOUND on failure (no equiv).
int translate_spell(int spel)
{
	switch(spel) {
		case 1: return 14;
		case 5: return 0;
		case 6: return 12;
		case 14: return 18;
		case 15: return 8;
		case 16: return 9;
		case 17: return 10;
		case 20: return 43;
		case 21: return 4;
		case 22: return 5;
		case 23: return 3;
		case 24: return 6;
		case 25: return 11;
		case 26: return 1;
		case 27: return 2;
		case 28: return 16; /* approximate */
		case 35: return 7;
		case 37: return 15;
		case 49: return 21; /* approximate */
		case 53: return 19;
		case 54: return 17;
		case 59: return 16;
		case 60: return 22;
		case 62: return 24; /* approximate */
		case 66: return 28;
		case 67: return 29;
		case 72: return 42; /* approximate */
		case 79: return 31;
		case 82: return 33;
		case 113: return 52;
		case 115: return 38;
		case 119: return 27;
		case 120: return 33;
		case 121: return 51;
		case 128: return 39;
		case 129: return 40;
		case 148: return 48; /* Too powerful to give ghosts Torment for Agony? Nah. */
		case 151: return 45;
		case 158: return 48;
		default: return NO_SPELL_FOUND;
	}
	return NO_SPELL_FOUND;
}

void generate_random_demon(void)
{
	int rdem = 0;
	int i = 0;

	for (rdem = 0; rdem < MNST + 1; rdem ++)
	{
		if (rdem == MNST) return; /* obviously no random demon */
		if (menv [rdem].m_class == 401) break; /* found one! */
	}

	strcpy(ghost.gname, make_name(random2(250), random2(250), random2(250), 3).c_str());

	ghost.ghs [0] = 50 + random2(50) + random2(50) + random2(50) + random2(50);
	if (random2(3) == 0) ghost.ghs [0] += random2(50) + random2(50);
	/* hp - could be defined below (as could ev, AC etc). Oh well, too late */

	ghost.ghs [1] = 5 + random2(10); /* evasion */
	ghost.ghs [2] = random2(0); /* AC */
	if (random2(3) == 0) ghost.ghs [3] = 1; else ghost.ghs [3] = 0; /* see inv */
	if (random2(3) != 0)
	{
		ghost.ghs [4] = 0; /* res_fire */
		if (random2(4) == 0) ghost.ghs [4] = 99;
		if (random2(4) == 0) ghost.ghs [4] = 102;
	} else ghost.ghs [4] = 101;
	if (random2(3) != 0)
	{
		ghost.ghs [5] = 0; /* res_cold */
		if (random2(4) == 0) ghost.ghs [5] = 99;
	} else ghost.ghs [5] = 101;
	/* demons, as ghosts, automatically get res poison + prot_life */
	if (random2(3) != 0)
	{
		ghost.ghs [6] = 0; /* res_elec */
	} else ghost.ghs [6] = 1;

	ghost.ghs [7] = 10 + random2(20) + random2(20) + random2(20); /* damage in combat */
	ghost.ghs [8] = 0; /* special attack type (uses weapon brand code) */
	if (random2(2) == 0)
	{
		ghost.ghs [8] = random2(17);
		if (ghost.ghs [8] == 3 || ghost.ghs [8] == 5 || ghost.ghs [8] == 7 || ghost.ghs [8] == 11 || ghost.ghs [8] == 12 || ghost.ghs [8] == 14)
			ghost.ghs [8] = 0; /* some brands inappropriate (eg holy wrath) */
	}
	ghost.ghs [9] = 0; /* ghost species - used for: is demon a spellcaster? */
	if (random2(3) != 0) ghost.ghs [9] = 1;
	ghost.ghs [10] = random2(3); /* ghost best skill - used for: does demon fly? */
	if (random2(3) == 0) ghost.ghs [10] = 0;
	ghost.ghs [11] = 0; /* vacant - ghost best skill level */
	ghost.ghs [12] = 10 + random2(10); /* Hit Dice */
	ghost.ghs [13] = 0; /* ghost class - used for: does demon cycle colours? */
	if (random2(10) == 0) ghost.ghs [13] = 1;

	menv [rdem].m_HD = ghost.ghs [12];
	menv [rdem].m_hp = ghost.ghs [0];
	menv [rdem].m_hp_max = ghost.ghs [0];
	menv [rdem].m_AC = ghost.ghs [2];
	menv [rdem].m_ev = ghost.ghs [1];
	menv [rdem].m_speed = 10;
	if (random2(3) == 0) menv [rdem].m_speed = 6 + random2(13);
	menv [rdem].m_speed_inc = 70;
	menv [rdem].m_sec = 1 + random2(15); /* demon's colour */

	ghost.ghs [14] = 250;
	ghost.ghs [15] = 250;
	ghost.ghs [16] = 250;
	ghost.ghs [17] = 250;
	ghost.ghs [18] = 250;
	ghost.ghs [19] = 250;

	/* This bit uses the list of player spells to find appropriate spells
	   for the demon, then converts those spells to the monster spell indices.
	   Some special monster-only spells are at the end. */
	if (ghost.ghs [9] == 1)
	{
		if (random2(2) == 0)
			do
			{
				if (random2(3) == 0) break;
				ghost.ghs [14] = search_order_conj [i];
				i ++;
				if (search_order_conj [i] == NO_SPELL_FOUND) break;
			} while(1);

		if (random2(2) == 0)
			do
			{
				if (random2(3) == 0) break;
				ghost.ghs [15] = search_order_conj [i];
				i ++;
				if (search_order_conj [i] == NO_SPELL_FOUND) break;
			} while(1);

		if (random2(4) != 0)
			do
			{
				if (random2(3) == 0) break;
				ghost.ghs [16] = search_order_third [i];
				i ++;
				if (search_order_third [i] == NO_SPELL_FOUND) break;
			} while(1);

		if (random2(2) == 0)
			do
			{
				if (random2(3) == 0) break;
				ghost.ghs [17] = search_order_misc [i];
				i ++;
				if (search_order_misc [i] == NO_SPELL_FOUND) break;
			} while(1);

		if (random2(2) == 0)
			do
			{
				if (random2(3) == 0) break;
				ghost.ghs [18] = search_order_misc [i];
				i ++;
				if (search_order_misc [i] == NO_SPELL_FOUND) break;
			} while(1);

		if (random2(2) == 0) ghost.ghs [19] = 59; /* blink */
		if (random2(2) == 0) ghost.ghs [19] = 1; /* teleport */

		for (i = 14; i < 20; i ++)
		{
			ghost.ghs [i] = translate_spell(ghost.ghs [i]);
		} /* Converts the player spell indices to monster spell ones */

		/* give demon a chance for some monster-only spells: */
		if (random2(25) == 0) ghost.ghs [14] = 50; /* metal splinters */
		if (random2(25) == 0) ghost.ghs [14] = 37; /* eye of devas */
		if (random2(25) == 0) ghost.ghs [15] = 26; /* steam */
		if (random2(25) == 0) ghost.ghs [15] = 35; /* blast */
		if (random2(25) == 0) ghost.ghs [15] = 49; /* fiend's hellfire */
		if (random2(25) == 0) ghost.ghs [16] = 30; /* smiting */
		if (random2(25) == 0) ghost.ghs [16] = 20; /* burst of hellfire */
		if (random2(25) == 0) ghost.ghs [16] = 20; /* burst of hellfire */
		if (random2(15) == 0) ghost.ghs [18] = 18; /* dig */

		/* and demon-summoning should be fairly common: */
		if (random2(12) == 0) ghost.ghs [16] = 51; /* summon class 1 demons */
		if (random2(12) == 0) ghost.ghs [16] = 27; /* summon demons */
		if (random2(20) == 0) ghost.ghs [17] = 51; /* summon class 1 demons */
		if (random2(20) == 0) ghost.ghs [17] = 27; /* summon demons */

		if (ghost.ghs [17] == 250) ghost.ghs [17] = 27; /* at least they can summon demons */
	}
}
