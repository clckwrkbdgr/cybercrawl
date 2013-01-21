#ifndef __DCRAWL__FILES_H__
#define __DCRAWL__FILES_H__


void load (int stair_taken, char moving_level, char was_a_labyrinth, char old_level, char want_followers, char just_made_new_lev, char where_were_you2);
void save_level (int level_saved, int was_a_labyrinth, int where_were_you);
void save_game(bool leave_game);
void restore_game ();
void save_ghost ();
bool file_exists(const std::string & filename);
std::string construct_filename(const std::string & name, int level, int where_are_you, bool is_labyrinth);


#endif //!__DCRAWL__FILES_H__
