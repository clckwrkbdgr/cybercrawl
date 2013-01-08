#define NO_UNRANDARTS 14


const char *randart_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev);
const char *randart_armour_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev);
const char *randart_ring_name(int aclass, int atype, int adam, int aplus, int aplus2, int ident_lev);
void standard_name_weap(int item_typ, char glog [80]);
int randart_wpn_properties(int aclass, int atype, int adam, int aplus, int aplus2, int acol, char prop);
void standard_name_armour(int item_typ, int item_plus2, char glorg [80]);

int number_unrandarts(void);
void set_unrandart_exist(int whun, char is_exist);
char does_unrandart_exist(int whun);
int find_okay_unrandart(int aclass);
void make_item_unrandart(int x, int ura_item);
const char *unrandart_descrip(char which_descrip, int aclass, int atype, int aplus, int aplus2);






