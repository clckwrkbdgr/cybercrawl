void moname(int mcl, char mench, char see_inv, char descrip, char glog [40]);

void item(void);
void monster_grid(void);
void noisy(char loudness, char nois_x, char nois_y);

int check_awaken(int mons_aw);

void viewwindow(char draw_it);
void magic_mapping(int map_radius, int proportion);
void show_map(int spec_place [2]);
char mons_near(unsigned char monst);
void losight(unsigned int sh [19] [19], unsigned char gr [80] [70], int x_p, int y_p);
void draw_border(int bord_col, char your_name [30], char clasnam [40], char tspecies);


