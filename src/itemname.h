#pragma once
#include <string>

void save_id(int identy [4] [50]);
void initial(void);
void set_id(int cla, int ty, int setting);
int get_id(int cla, int ty);
std::string it_name(int itn, int des);
std::string it_name(int itn, int quant, int des);
std::string in_name(int inn, int des);
std::string in_name(int inn, int quant, int des);
int property(int pr1, int pr2, int pr3);
int mass(int pr1, int pr2);
void init_properties(void);
int check_item_knowledge(void);
int weapon_skill(int wclass, int wtype);
int damage_type(int wclass, int wtype);
std::string make_name(int var1, int var2, int var3, int ncase);



