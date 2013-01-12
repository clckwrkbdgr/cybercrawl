
int player_teleport(void);
int player_fast_run(void);
int player_speed(void);
int player_AC(void);
int player_evasion(void);
int player_shield_class(void);
int player_see_invis(void);
int player_mag_abil(void);
int burden_change(void);
int player_regen(void);
int player_sust_abil(void);
int player_res_fire(void);
int player_res_cold(void);
int player_res_poison(void);
int player_prot_life(void);
int you_resist_magic(int power);
void forget_map(int chance_forgotten);
void how_hungered(int hunge);
void gain_exp(int exp_gained);
void display_char_status(void);
void redraw_skill(const std::string & your_name, const std::string & clasnam);
std::string species_name(int speci);
int wearing_amulet(int which_am);
int exp_needed(int lev, int species);
int check_stealth(void);
void level_change(void);
int player_spec_summ(void);
int player_spec_ench(void);
int player_spec_conj(void);
int player_spec_air(void);
int player_spec_earth(void);
int player_spec_cold(void);
int player_spec_fire(void);
int player_spec_holy(void);
int player_spec_death(void);
int player_spec_poison(void);
int player_energy(void);
int slaying_bonus(int which_affected); /* returns bonuses from rings of slaying etc */
int scan_randarts(int which_property);




