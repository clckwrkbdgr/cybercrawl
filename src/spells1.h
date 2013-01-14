
int spell_direction(struct dist spelld [1], struct bolt beam [1]);


void blink(void);
void random_blink(void);
void fireball(int power);
void cast_fire_storm(int powc);
void identify(int pow);
void conjure_flame(int pow);
void stinking_cloud(void);
void stinkcl(int cl_x, int cl_y, struct bolt beam [1]);

void cast_big_c(int pow, int cty);
void big_cloud(int clouds, int cl_x, int cl_y, int pow); //, struct bolt beam [1]);
int cast_lesser_healing(void);
int cast_greater_healing(void);
int cast_greatest_healing(void);
void cast_revivification(int mabil);
void cast_cure_poison(int mabil);
void purification(void);
void cast_deaths_door(int pow);
void abjuration(int pow);
void extension(int pow);
void ice_armour(int pow, int extending);
void stone_scales(int pow);
void missile_prot(int pow);
void deflection(int pow);
void cast_regen(int pow);
void cast_berserk(void);

void cast_swiftness(int power);
void cast_fly(int power);
void cast_teleport_control(int power);
void cast_ring_of_flames(int power);
void cast_insulation(int power);
void cast_resist_poison(int power);




