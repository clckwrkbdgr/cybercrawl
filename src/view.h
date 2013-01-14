void item(void);
void monster_grid(void);
void noisy(int loudness, int nois_x, int nois_y);

int check_awaken(int mons_aw);

void viewwindow(int draw_it);
void magic_mapping(int map_radius, int proportion);
void show_map(int spec_place [2]);
int mons_near(int monst);
void losight(int sh [19] [19], int gr [80] [70], int x_p, int y_p);
void draw_border(int bord_col, const std::string & your_name, const std::string & clasnam, int tspecies);



