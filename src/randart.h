#define NO_UNRANDARTS 14


std::string randart_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev);
std::string randart_armour_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev);
std::string randart_ring_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev);
std::string standard_name_weap(int item_typ);
int randart_wpn_properties(int aclass, int atype, int adam, int aplus, int aplus2, int acol, int prop);
std::string standard_name_armour(int item_typ, int item_plus2);

int number_unrandarts(void);
void set_unrandart_exist(int whun, int is_exist);
int does_unrandart_exist(int whun);
int find_okay_unrandart(int aclass);
void make_item_unrandart(int x, int ura_item);
std::string unrandart_descrip(int which_descrip, int aclass, int atype, int aplus, int aplus2);






