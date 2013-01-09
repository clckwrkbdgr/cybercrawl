
char item_name(int item_plus2, char item_clas, char item_typ, int item_da, int it_plus, int it_quant, char ident_lev, char descrip, char glag [60]);
std::string item_name_2(int item_plus2, char item_clas, char item_typ, int item_da, int it_plus, int it_quant, char ident_lev);
void save_id(char identy [4] [50]);
void initial(void);
void set_id(char cla, char ty, char setting);
char get_id(char cla, char ty);
void it_name(int itn, char des, char str_pass [80]);
void in_name(int inn, char des, char str_pass [80]);
int property(int pr1, int pr2, int pr3);
int mass(int pr1, int pr2);
void init_properties(void);
int check_item_knowledge(void);
int weapon_skill(char wclass, char wtype);
char damage_type(char wclass, char wtype);
std::string make_name(int var1, int var2, int var3, char ncase);



