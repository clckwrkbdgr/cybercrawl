void spore_goes_pop(int i);
void mons_throw(int i, struct bolt beem [1], int hand_used);
void dragon(int i, struct bolt beem [1]);
void monster_teleport(int monstel, int instan);
void mons_cast(int i, struct bolt beem [1], int spell_cast);
void mons_trap(int i);
int mons_spells(int spell_cast, int func_pass [10]/*, char beam_name [30]*/);
std::string spell_beam_name(int spell_cast);

