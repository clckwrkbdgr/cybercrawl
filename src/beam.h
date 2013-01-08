void missile(struct bolt beam [1], int throw_2);
int check_mons_resists(struct bolt beam [1], int o, int hurted);
void beam(struct bolt beam [1]);
int check_mons_magres(int mn, int pow);
void mass_enchantment(int wh_enchant, int pow);
int mons_ench_f2(int o, int is_near, int func_pass [10], struct bolt beam [1]);
void poison_monster(int mn, int source);
void sticky_flame_monster(int mn, int source, int power);
void place_cloud(int cl_type, int ctarget_x, int ctarget_y, int cl_range);
void tracer_f(int i, struct bolt beem [1]);
void mimic_alert(int mimic);

