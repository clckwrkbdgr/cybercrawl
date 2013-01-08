void moname(int mcl, char mench, char see_inv, char descrip, char glog [40]);

void item(void);
void monster_grid(void);
void noisy(char loudness, char nois_x, char nois_y);

int check_awaken(int mons_aw);

void viewwindow(char draw_it);
void magic_mapping(int map_radius, int proportion);
void show_map(int spec_place [2]);
char mons_near(int monst);
void losight(int sh [19] [19], int gr [80] [70], int x_p, int y_p);
void draw_border(int bord_col, const char * your_name, const char * clasnam, char tspecies);



