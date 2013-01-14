
int corpse_rot(int power);
int detect_traps(void);
int detect_items(int map_radius);
int detect_creatures(int map_radius);
int animate_dead(int power, int corps_beh, int corps_hit, int actual);
int animate_a_corpse(int axps, int ayps, int corps_beh, int corps_hit, int class_allowed);
int raise_corpse(int corps, int corx, int cory, int corps_beh, int corps_hit, int actual);
void cast_twisted(int power, int corps_beh, int corps_hit);
int brand_weapon(int which_brand, int power);
void manage_shock_shield(void);
int vampiric_drain(int pow);
int burn_freeze(int pow, int b_f);
int summon_elemental(int pow, int restricted_type, int unfriendly);
void summon_small_mammals(void);
void summon_scorpions(int pow);
void summon_ice_beast_etc(int pow, int ibc);
void summon_swarm(int pow);
void summon_undead(int pow);
void summon_butter(void);
void restore_str(void);
void restore_int(void);
void restore_dex(void);
void cast_refrigeration(int pow);
void drain_life(int pow);
void summon_things(int pow);
void turn_undead(int pow); // what should I use for pow?
void holy_word(int pow);
void cast_toxic_radiance(void);




