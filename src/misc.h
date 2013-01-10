#pragma once
#include <string>

void destroy_item(int dest);
void place_chunks(int mcls, int rot_status, int chx, int chy, int ch_col);
int search_around(void);
void in_a_cloud(void);
void up_stairs(void);
void down_stairs(int remove_stairs, int old_level);
void dart_trap(int trap_known, int trapped, struct bolt beam [1]);
void itrap(struct bolt beam [1], int trapped);
void cull_items(void);
void handle_traps(int trt, int trap_known);
void disarm_trap(struct dist disa [1]);
int manage_clouds(void);
std::string weird_writing();
void fall_into_a_pool(int place, int grype);
void new_level(void);
std::string weird_colours(int coll);
int go_berserk(void);








