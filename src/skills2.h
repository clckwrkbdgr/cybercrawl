void show_skills(void);

std::string skill_title(int best_skill, int skill_lev, int char_class, int char_lev);

int best_skill(int min_skill, int max_skill, int excl_skill);

int species_skills(int skill, int species);

int calc_hp(void);
int calc_ep(void);

int skill_exp_needed(int lev);

std::string skill_name(int which_skill);

int add_skill(int min_skill, int max_skill, int sk_tot);
void class_sk(int clsk [8] [3], int chcls);

