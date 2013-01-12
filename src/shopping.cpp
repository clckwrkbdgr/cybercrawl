
#include "linuxlib.h"

#include "externs.h"
#include "enum.h"

#include "describe.h"
#include "invent.h"
#include "itemname.h"
#include "player.h"
#include "stuff.h"

#include <stdlib.h>
#include <string.h>

/*#include "crawlfnc.h"
#include "fn2.h"
#include "invent.h"
#include "struct.h"*/

//#define ITEMS 500
/*
struct item_struct
{
	 int iclass [ITEMS];
	 int itype [ITEMS];
	 int iplus [ITEMS]; // +, charges, remaining food value
         int iplus2 [ITEMS];
	 int idam [ITEMS]; // damage
	 int iquant [ITEMS]; // multiple items
	 int ix [ITEMS]; //  x-location
	 int iy [ITEMS]; //  y-location
	 int icol [ITEMS];
	 int iid [ITEMS];

	 int ilink [ITEMS];

};

//struct item_struct it [1];

struct shop_struct
{

int keeper_name [5] [3];
int sh_x [5];
int sh_y [5];
int sh_greed [5];
int sh_type [5];
int sh_level [5];

};

*/

void purchase(int item_got);

//void purchase(struct player you [1]);

int item_value(int item_clas, int item_typ, int item_da, int it_plus, int it_plus2, int item_quant, char ident_lev, int id [4] [50]);


char in_a_shop(char shoppy, int id [4] [50]);

void shop_print(const char *shoppy, char sh_line);


char more3(void);


void clear_line(void);


char shop_getch(void);

void shop_init_id(int i, int shop_id [4] [50]);
void shop_uninit_id(int i, int shop_id [4] [50]);

void shop_set_id(int i, int shop_id [4] [50], int iclass, int itype);

char book_rarity(char which_book);

char in_a_shop(char shoppy_char, int id [4] [50])
{
	  Format format("You have @1 credit chip@2.");
	int shoppy = shoppy_char;
   int greedy = env[0].sh_greed [shoppy];
   int shop_id [4] [50];
   int shop_items [20];

   char st_pass [60];

   int gp_value = 0;
   char i;
   int ft;

   strcpy(st_pass, "");

   clrscr();
//   char st_pass [50];
int itty = 0;
char sh_name [40];


//char *make_name(int var1, int var2, int var3, char ncase)

      strcpy(sh_name, "Welcome to ");
      strcat(sh_name, make_name(env[0].keeper_name [shoppy] [0], env[0].keeper_name [shoppy] [1], env[0].keeper_name [shoppy] [2], 3).c_str());
      strcat(sh_name, "'s ");
      switch(env[0].sh_type [shoppy])
      {
       case 0: strcat(sh_name, "Weapon Store!"); break;
       case 1: strcat(sh_name, "Armour Store!"); break;
       case 2: strcat(sh_name, "Antique Weapon Store!"); break;
       case 3: strcat(sh_name, "Antique Armour Store!"); break;
       case 4: strcat(sh_name, "Assorted Antiques!"); break;
       case 5: strcat(sh_name, "Bioimplant Lab!"); break;
       case 6: strcat(sh_name, "Gun Shop!"); break;
       case 7: strcat(sh_name, "Program Repository!"); break;
       case 8: strcat(sh_name, "Food Shop!"); break;
       case 9: strcat(sh_name, "Chem Lab!"); break;
       case 10: strcat(sh_name, "Devices Store!"); break;
       case 11: strcat(sh_name, "General Store!"); break;
      }

//      strcat(sh_name, " shop.");

      shop_print(sh_name, 20); //"Hello, and welcome to ");

      more3();

//      for (i = 0; i < 4; i ++) // when invent becomes available, must replace!
//      {
//      }
      shop_init_id(shoppy, shop_id);
  /* THINGS TO DO:
   Allow inventory
   Remove id change for antique shops
   selling?
  */
       save_id(id);

print_stock :
clrscr();
itty = igrd [0] [5 + shoppy];


    if (igrd [0] [5 + shoppy] == 501)
     {
      empty_shop : shop_print("I'm sorry, my store is empty now.", 20);
      more3();
      goto goodbye;
     }

     if (you[0].inv_no >= 52)
     {
  	 shop_print("You seem to be carrying too much.", 20);
	 more3();
	 goto goodbye;
     }


      if (itty == 501) goto empty_shop;

      for (i = 1; i < 20; i ++) // remember i is a char
      {

       shop_items [i - 1] = itty;

       if (itty == 501) //mitm.ilink [itty] == 501)
       {
         shop_items [i - 1] = 501;
         continue;
       }

       itty = mitm.ilink [itty];

      }

      itty = igrd [0] [5 + shoppy];

      for (i = 1; i < 18; i ++) // remember i is a char
      {

       gotoxy(1, i);

       if (i % 2 == 0) textcolor(LIGHTGREY); else textcolor(WHITE);

       putch(i + 96);
       cprintf(" - ");
       cprintf(it_name(itty, 3).c_str());

       gp_value = greedy * item_value(mitm.iclass [itty], mitm.itype [itty], mitm.idam [itty], mitm.iplus [itty], mitm.iplus2 [itty], mitm.iquant [itty], mitm.iid [itty], id);
       gp_value /= 10;
       if (gp_value <= 1) gp_value = 1;
       gotoxy(60, i);
       cprintf(to_string(gp_value).c_str());
       cprintf("cc");
       if (mitm.ilink [itty] == 501) break;

       itty = mitm.ilink [itty];

      }

      textcolor(LIGHTGREY);

      shop_print("Type letter to buy item, x/Esc to leave, ?/* for inventory, V to examine.", 23);

      purchase :
      format << you[0].gp << ((you[0].gp == 1) ? "" : "s");
      shop_print(format.str().c_str(), 19);

      //shop_print("What would you like to purchase?", 20);

      shop_print("What would you like to purchase?", 20);

      ft = shop_getch();

      if (ft == 'x' || ft == 27) goto goodbye;

      if (ft == 'V')
      {
       shop_print("Examine which item?", 20);
       ft = shop_getch();

       if (ft < 'a' || ft > 'z')
         {
          goto huh;
         }

       ft -= 97;

       if (ft > 18) goto huh;

       if (shop_items [ft] == 501)
         {
          shop_print("I'm sorry, you seem to be confused.", 20);
          more3();
          goto purchase;
         }

       describe_item(mitm.iclass [shop_items [ft]], mitm.itype [shop_items [ft]], mitm.iplus [shop_items [ft]], mitm.iplus2 [shop_items [ft]], mitm.idam [shop_items [ft]], mitm.iid [shop_items [ft]]);

       goto print_stock;

      }

      if (ft == '?' || ft == '*')
      {
       shop_uninit_id(shoppy, shop_id);
       invent(-1, 0);
       shop_init_id(shoppy, shop_id);
       goto print_stock;
      }

      if (ft < 'a' || ft > 'z')
        {
         huh : shop_print("Huh?", 20);
         more3();
         goto purchase;
        }

      ft -= 97;

      if (ft > 18) goto huh;

      if (shop_items [ft] == 501)
        {
         shop_print("I'm sorry, you seem to be confused.", 20);
         more3();
         goto purchase;
        }

      if (greedy * (item_value(mitm.iclass [shop_items [ft]], mitm.itype [shop_items [ft]], mitm.idam [shop_items [ft]], mitm.iplus [shop_items [ft]], mitm.iplus2 [shop_items [ft]], mitm.iquant [shop_items [ft]], mitm.iid [shop_items [ft]], id)) / 10 > you[0].gp)
      {
	 shop_print("I'm sorry, you don't seem to have enough credits.", 20);
         more3();
         goto purchase;
      }

      purchase(shop_items [ft]);
      shop_set_id(i, shop_id, mitm.iclass [shop_items [ft]], mitm.itype [shop_items [ft]]);

      you[0].gp -= (greedy * item_value(mitm.iclass [shop_items [ft]], mitm.itype [shop_items [ft]], mitm.idam [shop_items [ft]], mitm.iplus [shop_items [ft]], mitm.iplus2 [shop_items [ft]], mitm.iquant [shop_items [ft]], mitm.iid [shop_items [ft]], id)) / 10;

      mitm.iquant [shop_items [ft]] = 0;
      if (ft == 0)
      {
       igrd [0] [5 + shoppy] = mitm.ilink [shop_items [ft]];
      } else mitm.ilink [shop_items [ft - 1]] = mitm.ilink [shop_items [ft]];

      goto print_stock;

      goodbye :
      clear_line();
      shop_print("Goodbye!", 20);
      more3();
//      for (i = 0; i < 4; i ++)
//      {
//      }
//
      shop_uninit_id(shoppy, shop_id);
      return 0;
}


void shop_init_id(int i, int shop_id [4] [50])
{
int j = 0;
    if (env[0].sh_type [i] != 2 && env[0].sh_type [i] != 3 && env[0].sh_type [i] != 4)
       for (j = 0; j < 50; j ++)
       {
        shop_id [0] [j] = get_id(OBJ_WANDS, j);
        set_id(OBJ_WANDS, j, 1);
        shop_id [1] [j] = get_id(OBJ_SCROLLS, j);
        set_id(OBJ_SCROLLS, j, 1);
        shop_id [2] [j] = get_id(OBJ_JEWELLERY, j);
        set_id(OBJ_JEWELLERY, j, 1);
        shop_id [3] [j] = get_id(OBJ_POTIONS, j);
        set_id(OBJ_POTIONS, j, 1);
       }


}

void shop_uninit_id(int i, int shop_id [4] [50])
{
int j = 0;

    if (env[0].sh_type [i] != 2 && env[0].sh_type [i] != 3 && env[0].sh_type [i] != 4)
       for (j = 0; j < 50; j ++)
       {
//        shop_id [i] [j] = get_id(i, j);
        set_id(OBJ_WANDS, j, shop_id [0] [j]);
        set_id(OBJ_SCROLLS, j, shop_id [1] [j]);
        set_id(OBJ_JEWELLERY, j, shop_id [2] [j]);
        set_id(OBJ_POTIONS, j, shop_id [3] [j]);
       }
}

void shop_set_id(int i, int shop_id [4] [50], int iclass, int itype)
{

  if (env[0].sh_type [i] != 2 && env[0].sh_type [i] != 3 && env[0].sh_type [i] != 4)
  {
      switch(iclass)
        {
          case OBJ_WANDS: shop_id [0] [itype] = 1; break;
          case OBJ_SCROLLS: shop_id [1] [itype] = 1; break;
          case OBJ_JEWELLERY: shop_id [2] [itype] = 1; break;
          case OBJ_POTIONS: shop_id [3] [itype] = 1; break;
        }
      set_id(iclass, itype, 1);
  }

}


void shop_print(const char *shoppy, char sh_lines)
{
 int i;
 gotoxy(1, sh_lines);
 cprintf(shoppy);
 for (i = strlen(shoppy); i < 80; i ++)
 {
  cprintf(" ");
 }

}

char more3(void)
{
 char keyin = 0;
 gotoxy(70,20);
 cprintf("-more-");
 keyin = getkey();
 if (keyin == 0) getkey();
 clear_line();
 return keyin;
}

void clear_line(void)
{
// int i;
//window(1, 20, 80, 21);
//clrscr();
//window(1, 1, 80, 25);
// gotoxy(1,20);
// for (i = 0; i < 80; i ++)
//    {
//     cprintf("XXXXXXXXXXXXXXXXXX                                                       ");
//    }
return;
}

char shop_getch(void)
{
 char keyin = 0;
 keyin = getkey();
 if (keyin == 0) getkey();
 return keyin;
}

void purchase(int item_got)
{

 int i = 0;
 for (i = 0; i < 53; i ++)
 {
  if (i == 52)
    {
     shop_print("The demon of the infinite cyberspace grins at you.", 20);
     more3();
     return;
    }
  if (you[0].inv_quant [i] <= 0) break;
 }

		you[0].inv_ident [i] = mitm.iid [item_got];
		you[0].inv_class [i] = mitm.iclass [item_got];
		you[0].inv_type [i] = mitm.itype [item_got];
		you[0].inv_plus [i] = mitm.iplus [item_got];
		you[0].inv_plus2 [i] = mitm.iplus2 [item_got];
		you[0].inv_dam [i] = mitm.idam [item_got];
		you[0].inv_col [i] = mitm.icol [item_got];
		you[0].inv_quant [i] = mitm.iquant [item_got];//mitm.iquant [item_got];

                you[0].inv_no++;
}

int item_value(int item_clas, int item_typ, int item_da, int it_plus, int it_plus2, int item_quant, char ident_lev, int id [4] [50])
{

int valued = 0;
int charge_value = 0;

switch(item_clas)
  {
        case OBJ_WEAPONS:

 if (item_da > 180)
 {
  if (ident_lev > 0)
  {
   switch(item_da - 180)
   {
    	case 1: valued += 1200; break; //"Singing Sword"); break;
    	case 2: valued += 1000; break; //"Wrath of Trog"); break;
    	case 3: valued += 800; break; //"Scythe of Curses"); break;
    	case 4: valued += 700; break; //"Mace of Variability"); break;
    	case 5: valued += 1000; break; //"Glaive of Prune"); break;
    	case 6: valued += 1200; break; //"Sceptre of Torment"); break;
    	case 7: valued += 1250; break; //"Sword of Zonguldrok"); break;
    	case 8: valued += 2000; break; //"Sword of Okawaru"); break;
    	case 9: valued += 1200; break; //""); break;
    	case 10: valued += 1500; break; //""); break;


        default: valued += 1000; break;
   }
  break;
  } // end if ident_lev
 } // end uniques

//        valued = 30;
	switch(item_typ)
	{
	case WPN_CLUB: valued += 10; break; //"club"); break;
	case WPN_MACE: valued += 30; break; //"mace"); break;
	case WPN_FLAIL: valued += 35; break; //"flail"); break;
	case WPN_DAGGER: valued += 20; break; //"dagger"); break;
	case WPN_MORNINGSTAR: valued += 35; break; //"spiked mace"); break;
	case WPN_SHORT_SWORD: valued += 32; break; //"short sword"); break;
	case WPN_LONG_SWORD: valued += 45; break; //"long sword"); break;
	case WPN_GREAT_SWORD: valued += 60; break; //"great sword"); break;
	case WPN_SCIMITAR: valued += 45; break; //"scimitar"); break;
	case WPN_HAND_AXE: valued += 28; break; //"hand axe"); break;
	case WPN_BATTLEAXE: valued += 65; break; //"battleaxe"); break;
	case WPN_SPEAR: valued += 32; break; //"spear"); break;
	case WPN_HALBERD: valued += 52; break; //"halberd"); break;
	case WPN_SLING: valued += 15; break; //"sling"); break;
	case WPN_BOW: valued += 31; break; //"bow"); break;
	case WPN_CROSSBOW: valued += 41; break; //"crossbow"); break;
        case WPN_HAND_CROSSBOW: valued += 51; break; //"hand crossbow"); break;
        case WPN_GLAIVE: valued += 55; break; //"glaive"); break;
	case WPN_QUARTERSTAFF: valued += 25; break; //"quarterstaff"); break;
        case WPN_SCYTHE: valued += 30; break; //"scythe"); break;
	case WPN_GIANT_CLUB: valued += 17; break; //"giant club"); break;
	case WPN_GIANT_SPIKED_CLUB: valued += 19; break; //"giant spiked club"); break;
	case WPN_EVENINGSTAR: valued += 65; break; // eveningstar
	case WPN_QUICK_BLADE: valued += 250; break; // quick blade
	case WPN_KATANA: valued += 300; break; // katana
	case WPN_EXECUTIONERS_AXE: valued += 100; break; // exec axe
	case WPN_DOUBLE_SWORD: valued += 200; break; // 2x sw
	case WPN_TRIPLE_SWORD: valued += 300; break; // 3x sw
	case WPN_HAMMER: valued += 30; break; // hammer
	case WPN_ANCUS: valued += 40; break; // ancus
	case WPN_WHIP: valued += 25; break; // whip
	case WPN_SABRE: valued += 40; break; // sabre
	case WPN_DEMON_BLADE: valued += 300; break; // demon blade
	case WPN_DEMON_WHIP: valued += 230; break; // demon whip

	}


if (ident_lev > 1)
{
 switch(item_da % 30)
 {
  case SPWPN_NORMAL: break;
  case SPWPN_FLAMING: valued *= 25; break; // " of flaming"); break;
  case SPWPN_FREEZING: valued *= 25; break; // " of freezing"); break;
  case SPWPN_HOLY_WRATH: valued *= 50; break; // " of holy wrath"); break;
  case SPWPN_ELECTROCUTION: valued *= 30; break; // " of electrocution"); break;
  case SPWPN_ORC_SLAYING: valued *= 21; break; // " of orc slaying"); break;
  case SPWPN_VENOM: valued *= 23; break; // " of venom"); break;
  case SPWPN_PROTECTION: valued *= 20; break; // " of protection"); break;
  case SPWPN_DRAINING: valued *= 64; break; // " of draining"); break;
  case SPWPN_SPEED: valued *= 40; break; // " of speed"); break;
  case SPWPN_VORPAL: valued *= 30; break; // slicing etc
  case SPWPN_FLAME: valued *= 50; break; // flaming
  case SPWPN_FROST: valued *= 50; break; // frost
  case SPWPN_VAMPIRICISM: valued *= 60; break; // vampiric
  case SPWPN_DISRUPTION: valued *= 50; break; // mace of disruption
  case SPWPN_PAIN: valued *= 30; break; // pain
  case SPWPN_DISTORTION: valued *= 30; break; // distortion
  default: valued *= 70; break; /* randart */
 }
 if (item_da % 30 != 0) valued /= 10;
}

if (item_da / 30 == DWPN_ELVEN || item_da / 30 == DWPN_DWARVEN) // elf/dwarf
{
  valued *= 12;
  valued /= 10;
}

if (item_da / 30 == 6) // orc
{
  valued *= 8;
  valued /= 10;
}


	if (ident_lev > 2)
	{

 	 if (it_plus >= 50 && (it_plus <= 130 || it_plus >= 150))
         {
           valued += (it_plus % 50) * 2;
           valued *= 10 + (it_plus % 50) + 2 * (it_plus % 50);
           valued /= 10;
         }

         if (it_plus2 >= 50)
         {
           valued += (it_plus2 % 50) * 2;
           valued *= 10 + (it_plus2 % 50) + 2 * (it_plus2 % 50);
           valued /= 10;
         }

         if (it_plus % 50 > 30)
         {
          valued -= 5;
          valued += ((it_plus % 50) - 50) * ((it_plus % 50) - 50) * ((it_plus % 50) - 50);
          if (valued < 1) valued = 1;
//          break;
         }

	 if (it_plus2 % 50 > 30)
         {
          valued -= 5;
          valued += ((it_plus2 % 50) - 50) * ((it_plus2 % 50) - 50) * ((it_plus2 % 50) - 50);
          if (valued < 1) valued = 1;
         }


	}

        if (ident_lev > 0)
	{

	if (it_plus >= 130)
	{
           valued *= 6;
           valued /= 10;
        }


	}

 break;


	case OBJ_MISSILES: // ammunition
	if (ident_lev > 0)
	{
         // assume not cursed (can they be anyway?)
         if (it_plus < 50) valued -= 11150;
         if (it_plus >= 50) valued += (it_plus % 50) * 2;
        }

	switch(item_typ)
	{
	case MI_STONE: valued += 1; break; // "stone"); break;
	case MI_ARROW: valued += 2; break; // "arrow"); break;
	case MI_BOLT: valued += 2; break; // "bolt"); break;
	case MI_DART: valued += 1; break; // "dart"); break;
	case MI_EGGPLANT: valued += 1; break; // "eggplant"); break;
	case MI_LARGE_ROCK: valued += 1; break; // "large rock"); break;
	case 6: valued += 5; break; // ""); break;
	case 7: valued += 5; break; // ""); break;
	case 8: valued += 5; break; // ""); break;
	case 9: valued += 5; break; // ""); break;
	case 10: valued += 5; break; // ""); break;
	case 12: valued += 5; break; // ""); break;
	case 13: valued += 5; break; // ""); break;
	case 14: valued += 5; break; // ""); break;
	case 15: valued += 5; break; // ""); break;
	case 16: valued += 5; break; // ""); break;
	}

 	break;



	case OBJ_ARMOUR: // armour


  /*
	if (ident_lev > 2)
	{
                if (it_plus >= 50 && (it_plus <= 130 || it_plus >= 150)) valued += it_plus % 50 * 15;
                if (it_plus < 50) valued -= 20 + (50 - it_plus) * 15;

//             valued += it_plus % 50;
        }*/

	switch(item_typ )
	{
	case ARM_ROBE: valued += 7; break; // "robe"); break;
	case ARM_LEATHER_ARMOUR: valued += 20; break; // "leather armour");
	case ARM_RING_MAIL: valued += 40; break; // "ring mail"); break;
	case ARM_SCALE_MAIL: valued += 83; break; // "scale mail"); break;
	case ARM_CHAIN_MAIL: valued += 110; break; // "chain mail"); break;
	case ARM_SPLINT_MAIL: valued += 140; break; // "splint mail"); break;
	case ARM_BANDED_MAIL: valued += 150; break; // "banded mail"); break;
	case ARM_PLATE_MAIL: valued += 230; break; // "plate mail"); break;
	case ARM_SHIELD: valued += 45; break; // "shield"); break;
	case ARM_CLOAK: valued += 10; break; // "cloak");
	case ARM_HELMET: valued += 25; break; // "helmet"); break;
	case ARM_GLOVES: valued += 12; break; // "gloves");
	case ARM_BOOTS: valued += 15; break; // "boots");
	case ARM_BUCKLER: valued += 25; break; // "buckler"); break;
	case ARM_LARGE_SHIELD: valued += 75; break; // "large shield"); break;
        case ARM_DRAGON_HIDE: valued += 500; break; // "dragon hide"); break;
        case ARM_TROLL_HIDE: valued += 130; break; //  "troll hide"); break;
        case ARM_CRYSTAL_PLATE_MAIL: valued += 500; break; //  "crystal plate mail"); break;
        case ARM_DRAGON_ARMOUR: valued += 750; break; //  "dragon scale mail"); break;
        case ARM_TROLL_LEATHER_ARMOUR: valued += 500; break; //  "troll leather armour"); break;
        case ARM_ICE_DRAGON_HIDE: valued += 500; break; //  "ice dragon hide"); break;
        case ARM_ICE_DRAGON_ARMOUR: valued += 750; break; //  "ice dragon scale mail"); break;
        case ARM_STEAM_DRAGON_HIDE: valued += 200; break; // "steam dragon hide"); break;
        case ARM_STEAM_DRAGON_ARMOUR: valued += 300; break; //  , "steam dragon scale mail"); break;
        case ARM_MOTTLED_DRAGON_HIDE: valued += 300; break; //  , "mottled dragon hide"); break;
        case ARM_MOTTLED_DRAGON_ARMOUR: valued += 400; break; //  , "mottled dragon scale mail"); break;
        case ARM_STORM_DRAGON_HIDE: valued += 900; break; //  , "storm dragon hide"); break;
        case ARM_STORM_DRAGON_ARMOUR: valued += 1050; break; //  , "storm dragon scale mail"); break;
        case ARM_GOLD_DRAGON_HIDE: valued += 1400; break; //  , "gold dragon hide"); break;
        case ARM_GOLD_DRAGON_ARMOUR: valued += 1600; break; //  , "gold dragon scale mail"); break;
		case ARM_ANIMAL_SKIN: valued += 3; break; // animal skins
        case ARM_SWAMP_DRAGON_HIDE: valued += 400; break; //  "swamp dragon hide"); break;
        case ARM_SWAMP_DRAGON_ARMOUR: valued += 650; break; //  "swamp dragon scale mail"); break;


        }


 if (ident_lev > 1)
 {
  switch(item_da % 30)
  {
   case SPARM_NORMAL: break;
   case SPARM_RUNNING: valued *= 40; break; //" of running"); break;
   case SPARM_FIRE_RESISTANCE: valued *= 30; break; //" of fire resistance"); break;
   case SPARM_COLD_RESISTANCE: valued *= 30; break; //" of cold resistance"); break;
   case SPARM_POISON_RESISTANCE: valued *= 20; break; //" of poison resistance"); break;
   case SPARM_SEE_INVISIBLE: valued *= 30; break; //" of see invisible"); break;
   case SPARM_DARKNESS: valued *= 60; break; //" of invisibility"); break;
   case SPARM_STRENGTH: valued *= 30; break; //" of strength"); break;
   case SPARM_DEXTERITY: valued *= 30; break; //" of dexterity"); break;
   case SPARM_INTELLIGENCE: valued *= 30; break; //" of intelligence"); break;
  case SPARM_PONDEROUSNESS: valued *= 5; break; //" of ponderousness"); break;
  case SPARM_LEVITATION: valued *= 30; break; //" of levitation"); break;
  case SPARM_MAGIC_RESISTANCE: valued *= 40; break; //" of magic resistance"); break;
  case SPARM_PROTECTION: valued *= 40; break; //" of protection"); break;
  case SPARM_STEALTH: valued *= 30; break; // of stealth
  case SPARM_RESISTANCE: valued *= 60; break; // of resistance
  case SPARM_POSITIVE_ENERGY: valued *= 50; break; // of life prot
  case SPARM_ARCHMAGI: valued *= 100; break; // of the Archmagi
  case SPARM_PRESERVATION: valued *= 30; break; // of preservation
  default: /* assume a randart */
       valued *= 70; break;
  }
 if (item_da % 30 != 0) valued /= 10;
 }

if (item_da / 30 == DARM_ELVEN || item_da / 30 == DARM_DWARVEN) // elf/dwarf
{
  valued *= 12;
  valued /= 10;
}

if (item_da / 30 == DARM_ORCISH) // orc
{
  valued *= 8;
  valued /= 10;
}

 	if (ident_lev >= 2)
	{
         valued += 5;
 	 if (it_plus >= 50 && (it_plus <= 130 || it_plus >= 150))
         {
           valued += (it_plus % 50) * 30;
           valued *= 10 + (it_plus % 50) * 3 * (it_plus % 50);
           valued /= 10;
         }

         if (it_plus % 50 > 30)
         {
          valued += ((it_plus % 50) - 50) * ((it_plus % 50) - 50) * ((it_plus % 50) - 50);
          if (valued < 1) valued = 1;
//          break;
         }
	}

        if (ident_lev > 0)
	{

	if (it_plus >= 130)
	{
           valued *= 6;
           valued /= 10;
        }


	}




 break;


	case OBJ_WANDS: // devices
        charge_value = 0;
        if (id [0] [item_typ] >= 1)
	{
		switch(item_typ)
		{
		case WAND_FLAME: valued += 5; charge_value += 2; break; // "wand of flame"); break;
		case WAND_FROST: valued += 5; charge_value += 2; break; // "wand of frost"); break;
		case WAND_SLOWING: valued += 10; charge_value += 3; break; // "wand of slowing"); break;
		case WAND_HASTING: valued += 15; charge_value += 3; break; // "wand of hasting"); break;
		case WAND_MAGIC_DARTS: valued += 3; charge_value += 1; break; // "wand of magic darts"); break;
		case WAND_HEALING: valued += 7; charge_value += 3; break; // "wand of healing"); break;
		case WAND_PARALYSIS: valued += 12; charge_value += 3; break; // "wand of paralysis"); break;
		case WAND_FIRE: valued += 15; charge_value += 3; break; // "wand of fire"); break;
		case WAND_COLD: valued += 15; charge_value += 3; break; // "wand of cold"); break;
		case WAND_CONFUSION: valued += 10; charge_value += 2; break; // "wand of confusion"); break;
		case WAND_INVISIBILITY: valued += 15; charge_value += 2; break; // "wand of invisibility"); break;
		case WAND_DIGGING: valued += 10; charge_value += 2; break; // "wand of digging"); break;
      case WAND_FIREBALL: valued += 20; charge_value += 5; break; // "wand of fireball"); break;
      case 13: valued += WAND_INVISIBILITY; charge_value += 2; break; // "wand of teleportation"); break;
		case 14: valued += 20; charge_value += 5; break; // "wand of lightning"); break;
      case 15: valued += WAND_POLYMORPH_OTHER; charge_value += 4; break; // "wand of polymorph"); break;
      case 16: valued += WAND_POLYMORPH_OTHER; charge_value += 3; break; // "wand of enslavement"); break;
      case 17: valued += 20; charge_value += 4; break; // "wand of draining"); break;
      case 18: valued += WAND_TELEPORTATION; charge_value += 3; break; // "wand of random effects"); break;
      case 19: valued += WAND_DRAINING; charge_value += 4; break; // "wand of disintegration"); break;
      default: valued += 10; break;
      } //else valued = 10;

        if (ident_lev > 1)
	{
         valued += it_plus * charge_value;
        }
       valued *= 3;
       if (it_plus == 0) valued = 3; // change if wands are rechargeable!
       } else valued = 35;
       break;




	case OBJ_POTIONS: // potions

        if (id [3] [item_typ] == 0)
        {
         valued += 9;
         break;
        }
		switch(item_typ)
		{
		case POT_HEALING: valued += 20; break; //, "healing"); break;
		case POT_HEAL_WOUNDS: valued += 30; break; //, "heal wounds"); break;
		case POT_SPEED: valued += 25; break; //, "speed"); break;
		case POT_MIGHT: valued += 25; break; //, "might"); break;
		case POT_GAIN_STRENGTH: valued += 350; break; //, "gain strength"); break;
		case POT_GAIN_DEXTERITY: valued += 350; break; //, "gain dexterity"); break;
		case POT_GAIN_INTELLIGENCE: valued += 350; break; //, "gain intelligence"); break;
		case POT_LEVITATION: valued += 20; break; //, "levitation"); break;
		case POT_POISON: valued += 1; break; //, "poison"); break;
		case POT_SLOWING: valued += 1; break; //, "slowing"); break;
		case POT_PARALYSIS: valued += 1; break; //, "paralysis"); break;
		case POT_CONFUSION: valued += 1; break; //, "confusion"); break;
		case POT_INVISIBILITY: valued += 55; break; //, "invisibility"); break;
		case POT_PORRIDGE: valued += 10; break; //, "porridge"); break;
		case POT_DEGENERATION: valued += 1; break; //, "degeneration"); break;
		case POT_DECAY: valued += 1; break; //, "decay"); break; // Rare!
		case POT_WATER: valued += 1; break; //, "water"); break;
		case POT_EXPERIENCE: valued += 500; break; //, "experience"); break;
		case POT_MAGIC: valued += 120; break; //, "magic"); break;
		case POT_RESTORE_ABILITIES: valued += 50; break; //, "restore abilities"); break;
		case POT_STRONG_POISON: valued += 1; break; //, "strong poison"); break;
		case POT_BERSERK_RAGE: valued += 30; break; // berserk rage
		case POT_CURE_MUTATION: valued += 150; break; // cure mutation
		case POT_MUTATION: valued += 50; break; // mutation
		}
	break;

	case OBJ_FOOD: // food
//	icolour  = 6;
	switch(item_typ )
	{
	case FOOD_MEAT_RATION: valued = 10; break; //, "meat ration"); break;
	case FOOD_BREAD_RATION: valued = 10; break; //, "bread ration"); break;
	case FOOD_PEAR: valued = 3; break; //, "pear"); break;
	case FOOD_APPLE: valued = 3; break; //, "apple"); break; break; // make this less common.
	case FOOD_CHOKO: valued = 5; break; //, "choko"); break;
	case FOOD_HONEYCOMB: valued = 14; break; //, "honeycomb"); break;
	case FOOD_ROYAL_JELLY: valued = 35; break; //, "royal jell"); break; break; // maybe a joke monster of the same name? - mix something with jelly crystals?
	case FOOD_SNOZZCUMBER: valued = 1; break; //, "snozzcumber"); break;
 case FOOD_PIZZA: valued = 5; break; //, "slice of pizza"); break;
 case FOOD_APRICOT: valued = 3; break; //, "apricot"); break;
 case FOOD_ORANGE: valued = 4; break; //, "orange"); break;
 case FOOD_BANANA: valued = 3; break; //, "banana"); break;
 case FOOD_STRAWBERRY: valued = 1; break; //, "strawberr"); break;
 case FOOD_RAMBUTAN: valued = 1; break; //, "rambutan"); break;
 case FOOD_LEMON: valued = 4; break; //, "lemon"); break;
 case FOOD_GRAPE: valued = 1; break; //, "grape"); break;
 case FOOD_SULTANA: valued = 1; break; //, "sultana"); break;
 case FOOD_LYCHEE: valued = 2; break; //, "lychee"); break;
 case FOOD_BEEF_JERKY: valued = 7; break; //, "beef jerk"); break;
 case FOOD_CHEESE: valued = 6; break; //, "cheese"); break;
 case FOOD_SAUSAGE: valued = 4; break; //, "sausage"); break;
 case FOOD_CHUNK: valued = 1; break;
      // break;
 }
 valued *= 8;
 break;

	case OBJ_SCROLLS: // scrolls

	if (id [1] [item_typ] == 0)
	{
         valued += 10;
        }

	switch(item_typ)
	{
	case SCR_IDENTIFY: valued += 20; break; //, "of identify"); break;
	case SCR_TELEPORTATION: valued += 30; break; //, "of teleportation"); break;
	case SCR_FEAR: valued += 45; break; //, "of fear"); break;
	case SCR_NOISE: valued += 2; break; //, "of noise"); break;
	case SCR_REMOVE_CURSE: valued += 30; break; //, "of remove curse"); break;
	case SCR_DETECT_CURSE: valued += 20; break; //, "of detect curse"); break;
	case SCR_SUMMONING: valued += 95; break; //, "of summoning"); break;
	case SCR_ENCHANT_WEAPON_I: valued += 48; break; //, "of enchant weapon"); break;
	case SCR_ENCHANT_ARMOUR: valued += 48; break; //, "of enchant armour"); break;
	case SCR_TORMENT: valued += 75; break; //, "of torment"); break;
	case SCR_RANDOM_USELESSNESS: valued += 2; break; //, "of random uselessness"); break;
	case SCR_CURSE_WEAPON: valued += 1; break; //, "of curse weapon"); break;
	case SCR_CURSE_ARMOUR: valued += 1; break; //, "of curse armour"); break;
	case SCR_IMMOLATION: valued += 1; break; //, "of immolation"); break;
	case SCR_BLINKING: valued += 30; break; //, "of blinking"); break;
 	case SCR_PAPER: valued += 1; break; //, "of paper"); break;
	case SCR_MAGIC_MAPPING: valued += 35; break; //, "of magic mapping"); break;
	case SCR_FORGETFULNESS: valued += 1; break; //, "of forgetfulness"); break;
	case SCR_ACQUIREMENT: valued += 520; break; //, "of acquirement"); break;
	case SCR_ENCHANT_WEAPON_II: valued += 55; break; //, "of enchant weapon II"); break;
	case SCR_VORPALISE_WEAPON: valued += 200; break; //, "of vorpalise weapon"); break;
	case SCR_RECHARGING: valued += 50; break; // recharging
	case SCR_ENCHANT_WEAPON_III: valued += 200; break; // ench wpn iii
	}
 	break;

	case OBJ_JEWELLERY: // jewellery

        if (id [2] [item_typ] == 0)
        {
         valued += 50;
        }


   if (ident_lev > 0)
   {
      if (it_plus >= 80)
      {
         valued -= 10;
      }
   }


   if (id [2] [item_typ] > 0)
	{

	if (ident_lev > 1 && (item_typ == RING_PROTECTION || item_typ == RING_STRENGTH || item_typ == RING_EVASION || item_typ == RING_DEXTERITY || item_typ == RING_INTELLIGENCE))
	{
		if (it_plus >= 50 && (it_plus <= 130 || it_plus >= 150)) valued += 10 * (it_plus % 50);
	}

	switch(item_typ)
	{
	case RING_REGENERATION: valued += 75; break;
	case RING_PROTECTION: valued += 10; break;
	case RING_PROTECTION_FROM_FIRE: valued += 30; break; 
	case RING_POISON_RESISTANCE: valued += 30; break; 
	case RING_PROTECTION_FROM_COLD: valued += 30; break; 
	case RING_STRENGTH: valued += 10; break; 
	case RING_SLAYING: valued += 30; break; 
	case RING_SEE_INVISIBLE: valued += 20; break; 
	case RING_INVISIBILITY: valued += 100; break; 
	case RING_HUNGER: valued -= 50; break; 
	case RING_TELEPORTATION: valued -= 10; break; 
	case RING_EVASION: valued += 10; break; 
 case RING_SUSTAIN_ABILITIES: valued += 25; break; 
 case RING_SUSTENANCE: valued += 25; break; 
 case RING_DEXTERITY: valued += 10; break; 
 case RING_INTELLIGENCE: valued += 10; break; 
 case RING_WIZARDRY: valued += 35; break; 
 case RING_MAGICAL_POWER: valued += 40; break; 
 case RING_LEVITATION: valued += 30; break; 
 case RING_LIFE_PROTECTION: valued += 60; break; 
 case RING_PROTECTION_FROM_MAGIC: valued += 40; break; 
 case RING_FIRE: valued += 62; break; 
 case RING_ICE: valued += 62; break; 
 case RING_TELEPORT_CONTROL: valued += 42; break; 

 case AMU_RAGE: valued += 20; break; 
 case AMU_MAINTAIN_SPEED: valued += 30; break; 
 case AMU_CLARITY: valued += 30; break; 
 case AMU_WARDING: valued += 30; break; 
 case AMU_RESIST_CORROSION: valued += 30; break; 
 case AMU_THE_GOURMAND: valued += 15; break; 
 case AMU_CONSERVATION: valued += 25; break; 
 case AMU_CONTROLLED_FLIGHT: valued += 25; break; 
 case AMU_INACCURACY: valued -= 50; break; 
 case AMU_RESIST_MUTATION: valued += 30; break; 

 // got to do delusion!
 }

 if (item_da == 200) valued += 50;

 valued *= 7;

 if (valued <= 0) valued = 1;

 } // end of if ident_lev
break;

case OBJ_MISCELLANY: // Miscellaneous
switch(ident_lev)
{
case 0:
 switch(item_typ)
 {
 case MISC_BOTTLED_EFREET: valued += 350; break; 
 case MISC_CRYSTAL_BALL_OF_SEEING: valued += 450; break;
// case 2: valued += 40; break;
 case MISC_HORN_OF_GERYON: valued += 1000; break;
 case MISC_RUNE_OF_ZOT: valued += 1000; break;
 case MISC_DECK_OF_TRICKS: valued += 100; break;
 default: valued += 400;
 }
break;

default:
 switch(item_typ)
 {
 case MISC_BOTTLED_EFREET: valued += 400; break;
 case MISC_CRYSTAL_BALL_OF_SEEING: valued += 500; break;
// case 2: valued += 40; break;
 case MISC_HORN_OF_GERYON: valued += 5000; break;
      case MISC_EMPTY_EBONY_CASKET:
      case 12:
      valued += 20; break;
 case MISC_DISC_OF_STORMS:
      valued += 2000;
      break;
 case MISC_RUNE_OF_ZOT: valued += 1200; break;
 default: valued += 500;
 }
break;

} // end switch ident_lev
break;

//case 10: break;

case OBJ_BOOKS:
//strcpy(glog, "");
if (ident_lev == 0)
{
 valued = 150;
 break;
}

valued = 150 + book_rarity(item_typ) * 50;

/*
valued = 210;

if (item_typ < 7 || item_typ == 23 || item_typ == 25)
  {
   valued = 150;
   break;
  }

if (item_typ == 14 || item_typ == 24) // gr conj/necronomicon
{
 valued = 550;
 break;
}

if (item_typ == 15)
{
 valued = 550;
 break;
}

if (item_typ == 17 || item_typ == 35)
{
 valued = 470;
 break;
}
*/
break;


case OBJ_STAVES: // magical staves
if (ident_lev == 0)
{
 valued = 120;
 break;
} // end if

valued = 250;
if (item_typ == STAFF_SMITING || item_typ == STAFF_WARDING || item_typ == STAFF_DISCOVERY)
  {
   valued = 150;
  }
break;


case OBJ_ORBS:
valued = 10000;
     //strcpy(glog , "Orb of Zot");
break;


} // end switch

if (valued <= 1) valued = 1;
valued *= item_quant;
return valued;

//}
}




char book_rarity(char which_book)
{

 switch(which_book)
 {
  case BOOK_MINOR_MAGIC_I:
  case BOOK_MINOR_MAGIC_II:
  case BOOK_MINOR_MAGIC_III:
//  case 3:
//  case 4:
  return 1;

  case BOOK_CONJURATIONS_I:
  case BOOK_CONJURATIONS_II:
  return 3;

  case BOOK_FLAMES:
  case BOOK_FROST:
  return 4;

  case BOOK_INVOCATIONS:
  return 18;

  case BOOK_FIRE:
  case BOOK_ICE:
  return 10;

  case BOOK_SURVEYANCES:
  return 1;

  case BOOK_SPATIAL_TRANSLOCATIONS:
  return 5;

  case BOOK_ENCHANTMENTS:
  return 7;

  case BOOK_POISONINGS:
  return 1;

  case BOOK_STORMS_AND_FIRE:
  case BOOK_DEATH:
  return 11;

  case BOOK_HINDERANCE:
  return 1;

  case BOOK_CHANGES:
  return 2;

  case BOOK_TRANSFIGURATIONS:
  return 8;

  case BOOK_USEFUL_MAGIC:
  return 3;

  case BOOK_CLOUDS:
  return 6;

  case BOOK_HEALING:
  return 100;

  case BOOK_NECROMANCY:
  return 3;

  case BOOK_NECRONOMICON:
  return 20;

  case BOOK_SUMMONINGS:
  return 3;

  case BOOK_CHARMS:
  return 2;

  case BOOK_DEMONOLOGY:
  return 15;

  case BOOK_AIR:
  return 4;

  case BOOK_SKY:
  return 10;

  case BOOK_DIVINATIONS:
  return 8;

  case BOOK_WARP:
  return 6;

  case BOOK_ENVENOMATIONS:
  return 12;

  case BOOK_ANNIHILATIONS:
  return 17;

  case BOOK_UNLIFE:
  return 10;

  case BOOK_DESTRUCTION: // tome of destruction
  return 30;

  case BOOK_CONTROL:
  return 10;

  case BOOK_MUTATIONS:
  return 12;

  case BOOK_TUKIMA:
  return 16;

  case BOOK_GEOMANCY:
  return 4;

  case BOOK_EARTH:
  return 10;

  case BOOK_MANUAL: // manuals
  return 20;

  case BOOK_WIZARDRY: // wizardry
  return 3;

  case BOOK_POWER: // power
  return 6;

 }

return 1;

}

void shop(void)
{

int i;

for (i = 0; i < 6; i ++)
{
 if (i == 5)
 {
  mpr("Help! Non-existent shop.");
  return;
 }
 if (env[0].sh_x [i] == you[0].x_pos && env[0].sh_y [i] == you[0].y_pos)
    break;
}

int identy [4] [50];
save_id(identy);

in_a_shop(i, identy);
you[0].gp_ch = 1;
burden_change();
redraw_screen();
}

