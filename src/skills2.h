void show_skills(void);

const char *skill_title(char best_skill, char skill_lev, char char_class, char char_lev);

int best_skill(char min_skill, char max_skill, char excl_skill);

int species_skills(char skill, char species);

int calc_hp(void);
int calc_ep(void);

int skill_exp_needed(int lev);

const char *skill_name(char which_skill);

int add_skill(int min_skill, int max_skill, int sk_tot);
void class_sk(int clsk [8] [3], int chcls);

