
void destroy_item(int dest);
void place_chunks(int mcls, int rot_status, int chx, int chy, int ch_col);
char search_around(void);
void in_a_cloud(void);
void up_stairs(void);
void down_stairs(char remove_stairs, int old_level);
void dart_trap(int trap_known, int trapped, struct bolt beam [1]);
void itrap(struct bolt beam [1], int trapped);
void cull_items(void);
void handle_traps(char trt, char trap_known);
void disarm_trap(struct dist disa [1]);
int manage_clouds(void);
void weird_writing(char stringy [40]);
void fall_into_a_pool(char place, int grype);
void new_level(void);
void weird_colours(int coll, char wc [30]);
char go_berserk(void);








