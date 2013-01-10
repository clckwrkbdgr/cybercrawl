#pragma once
#include <string>

int spell_hunger(int spell_value, int spell);
int which_spell_in_book(int sbook_type, int spl);
char spellbook_contents(int plus, int type);
std::string spell_name(int spell);
char spell_value(int spell);
int spell_type(int spell, int typy);
int spell_spec(int spell, int power);
char spell_fail(int spell);
void print_slash(char already);
char spell_list(void);
char undead_can_memorise(int spell);

