

#include "linuxlib.h"

#include "externs.h"
#include "enum.h"
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "misc.h"
#include "output.h"
#include "view.h"
#include "skills2.h"

extern int wield_change;

std::string substring(const std::string & string, size_t size)
{
	if(string.size() < size) {
		return string;
	}
	return std::string(string, 0, size);
}

int random2(int randmax)
{
 if (randmax <= 0) return 0;
 return random() % randmax;
}

int get_ch(void)
{
 int gotched = getkey();
 if (gotched == 0) gotched = getkey();
 return gotched;
}



int see_grid(int grx, int gry)
{

if (grx > you[0].x_pos - 9 && grx < you[0].x_pos + 9 && gry > you[0].y_pos - 9 && gry < you[0].y_pos + 9)
{
        if (env[0].show [grx - you[0].x_pos + 9] [gry - you[0].y_pos + 9] != 0)
                return 1;
}

return 0;

}


int magic_ability(int mag_abil, int intel)
{

int retv = mag_abil * intel;

retv /= 10;


return retv;

}

void end(int end_arg)
{
lincurses_shutdown();
 exit(end_arg);
}

// this function is used for systems without gettext/puttext to redraw the
// playing screen after a call to for example inventory.
void redraw_screen(void)
{

draw_border(BROWN, you[0].your_name, skill_title(best_skill(0, 50, 99), you[0].skills [best_skill(0, 50, 99)], you[0].clas, you[0].xl).c_str(), you[0].species);


you[0].hp_ch = 1;
you[0].ep_ch = 1;
you[0].strength_ch = 1;
you[0].intel_ch = 1;
you[0].dex_ch = 1;
you[0].AC_ch = 1;
you[0].evasion_ch = 1;
you[0].gp_ch = 1;
you[0].xp_ch = 1;
you[0].hung_ch = 1;
you[0].burden_ch = 1;
wield_change = 1;

print_stats();

new_level();

viewwindow(1);


}


