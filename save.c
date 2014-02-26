/*
 * save and restore routines
 *
 * @(#)save.c	3.9 (Berkeley) 6/16/81
 */

#include "curses.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include "rogue.h"

#define AS_IT_IS(var) { &var, sizeof(var) }
#define WRITE_AS_IT_IS(var) fwrite(&var, 1, sizeof(var), savef)
#define READ_AS_IT_IS(var) fread(&var, 1, sizeof(var), savef)
struct as_it_is_var {
	void * variable;
	int size;
} as_it_is[] = {
	AS_IT_IS(rooms), AS_IT_IS(rdes), AS_IT_IS(between), AS_IT_IS(level),
	AS_IT_IS(purse), AS_IT_IS(ntraps), AS_IT_IS(no_move), AS_IT_IS(no_command),
	AS_IT_IS(inpack), AS_IT_IS(max_hp), AS_IT_IS(lastscore), AS_IT_IS(no_food),
	AS_IT_IS(count), AS_IT_IS(fung_hit), AS_IT_IS(quiet), AS_IT_IS(max_level),
	AS_IT_IS(food_left), AS_IT_IS(group), AS_IT_IS(hungry_state),
	AS_IT_IS(whoami), AS_IT_IS(fruit), AS_IT_IS(ch_ret), AS_IT_IS(nh),
	AS_IT_IS(oldpos), AS_IT_IS(delta), AS_IT_IS(traps), AS_IT_IS(huh),
	AS_IT_IS(running), AS_IT_IS(playing), AS_IT_IS(wizard), AS_IT_IS(after),
	AS_IT_IS(notify), AS_IT_IS(fight_flush), AS_IT_IS(terse),
	AS_IT_IS(door_stop), AS_IT_IS(jump), AS_IT_IS(slow_invent),
	AS_IT_IS(firstmove), AS_IT_IS(waswizard), AS_IT_IS(askme), AS_IT_IS(amulet),
	AS_IT_IS(in_shell), AS_IT_IS(take), AS_IT_IS(runch), AS_IT_IS(s_know),
	AS_IT_IS(p_know), AS_IT_IS(r_know), AS_IT_IS(ws_know),
	AS_IT_IS(max_stats),
	{ 0, 0 }
};

write_object_list(struct linked_list * l, FILE * savef)
{
	char ok = 1, end = 0;
	struct linked_list * ptr = l;
	while(ptr) {
		fwrite(&ok, 1, 1, savef);
		fwrite(ldata(ptr), 1, sizeof(struct object), savef);
		ptr = next(ptr);
	}
	fwrite(&end, 1, 1, savef);
}

read_object_list(struct linked_list ** l, FILE* savef)
{
	*l = 0;
	char ok = 1;
	struct linked_list * ptr;
	fread(&ok, 1, 1, savef);
	while(ok) {
		if(*l) {
			next(ptr) = new_item(sizeof(struct object));
			struct linked_list * tmp_ptr = ptr;
			ptr = next(ptr);
			ptr->l_next = NULL;
			ptr->l_prev = tmp_ptr;
		} else {
			*l = ptr = new_item(sizeof(struct object));
			ptr->l_next = NULL;
			ptr->l_prev = NULL;
		}

		fread(ldata(ptr), 1, sizeof(struct object), savef);

		fread(&ok, 1, 1, savef);
	}
}

write_thing(struct thing * t, FILE * savef)
{
	fwrite(t, 1, sizeof(struct thing), savef);
	char t_dest = 0;
	if(t->t_dest == &hero) {
		t_dest = 100;
	}
	int i;
	for(i = 0; i < MAXROOMS; ++i) {
		if(t->t_dest == &rooms[i].r_gold) {
			t_dest = 10 + i;
			break;
		}
	}
	fwrite(&t_dest, 1, sizeof(t_dest), savef);
	write_object_list(t->t_pack, savef);
}

read_thing(struct thing * t, FILE * savef)
{
	fread(t, 1, sizeof(struct thing), savef);

	char t_dest = 0;
	fread(&t_dest, 1, sizeof(t_dest), savef);
	if(t_dest == 100) {
		t->t_dest = &hero;
	} else if(10 <= t_dest && t_dest < 100) {
		t->t_dest = &rooms[t_dest - 10].r_gold;
	} else {
		t->t_dest = NULL;
	}
	read_object_list(&t->t_pack, savef);
}

write_monster_list(struct linked_list * l, FILE * savef)
{
	char ok = 1, end = 0;
	struct linked_list * ptr = l;
	while(ptr) {
		fwrite(&ok, 1, 1, savef);
		write_thing((struct thing *)ldata(ptr), savef);
		ptr = next(ptr);
	}
	fwrite(&end, 1, 1, savef);
}

read_monster_list(struct linked_list ** l, FILE* savef)
{
	*l = 0;
	char ok = 1;
	struct linked_list * ptr;
	fread(&ok, 1, 1, savef);
	while(ok) {
		ptr = new_item(sizeof(struct thing));
		struct thing * t = (struct thing *)ldata(ptr);
		read_thing(t, savef);
		attach(*l, ptr);

		fread(&ok, 1, 1, savef);
	}
}

void write_win(WINDOW * win, FILE * savef)
{
	int x, y;
	int w, h;
	getmaxyx(win, h, w);
	for(x = 0; x < w; ++x) {
		for(y = 0; y < h; ++y) {
			char ch = mvwinch(win, y, x);
			fwrite(&ch, 1, 1, savef);
		}
	}
}

void read_win(WINDOW * win, FILE * savef)
{
	int x, y;
	int w, h;
	getmaxyx(win, h, w);
	for(x = 0; x < w; ++x) {
		for(y = 0; y < h; ++y) {
			char ch;
			fread(&ch, 1, 1, savef);
			mvwaddch(win, y, x, ch);
		}
	}
}

void write_string_array(char ** array, int count, FILE * savef)
{
	int index;
	for(index = 0; index < count; ++index) {
		int length = 0;
		if(array[index]) {
			length = strlen(array[index]) + 1;
			fwrite(&length, 1, sizeof(length), savef);
			fwrite(array[index], 1, length, savef);
		} else {
			fwrite(&length, 1, sizeof(length), savef);
		}
	}
}

void read_string_array(char ** array, int count, FILE * savef)
{
	int index;
	for(index = 0; index < count; ++index) {
		int length;
		fread(&length, 1, sizeof(length), savef);
		if(length) {
			array[index] = (char *) new(length);
			fread(array[index], 1, length, savef);
		} else {
			array[index] = 0;
		}
	}
}

write_game(FILE *savef)
{
	WRITE_AS_IT_IS(rooms); WRITE_AS_IT_IS(rdes); WRITE_AS_IT_IS(between); WRITE_AS_IT_IS(level);
	WRITE_AS_IT_IS(purse); WRITE_AS_IT_IS(ntraps); WRITE_AS_IT_IS(no_move); WRITE_AS_IT_IS(no_command);
	WRITE_AS_IT_IS(inpack); WRITE_AS_IT_IS(max_hp); WRITE_AS_IT_IS(lastscore); WRITE_AS_IT_IS(no_food);
	WRITE_AS_IT_IS(count); WRITE_AS_IT_IS(fung_hit); WRITE_AS_IT_IS(quiet); WRITE_AS_IT_IS(max_level);
	WRITE_AS_IT_IS(food_left); WRITE_AS_IT_IS(group); WRITE_AS_IT_IS(hungry_state);
	WRITE_AS_IT_IS(whoami); WRITE_AS_IT_IS(fruit); WRITE_AS_IT_IS(ch_ret); WRITE_AS_IT_IS(nh);
	WRITE_AS_IT_IS(oldpos); WRITE_AS_IT_IS(delta); WRITE_AS_IT_IS(traps); WRITE_AS_IT_IS(huh);
	WRITE_AS_IT_IS(running); WRITE_AS_IT_IS(playing); WRITE_AS_IT_IS(wizard); WRITE_AS_IT_IS(after);
	WRITE_AS_IT_IS(notify); WRITE_AS_IT_IS(fight_flush); WRITE_AS_IT_IS(terse);
	WRITE_AS_IT_IS(door_stop); WRITE_AS_IT_IS(jump); WRITE_AS_IT_IS(slow_invent);
	WRITE_AS_IT_IS(firstmove); WRITE_AS_IT_IS(waswizard); WRITE_AS_IT_IS(askme); WRITE_AS_IT_IS(amulet);
	WRITE_AS_IT_IS(in_shell); WRITE_AS_IT_IS(take); WRITE_AS_IT_IS(runch); WRITE_AS_IT_IS(s_know);
	WRITE_AS_IT_IS(p_know); WRITE_AS_IT_IS(r_know); WRITE_AS_IT_IS(ws_know);
	WRITE_AS_IT_IS(max_stats);

	if(lvl_obj) {
		write_object_list(lvl_obj, savef);
	}
	write_thing(&player, savef);
	write_monster_list(mlist, savef);

	int player_weapon = 0, player_armor = 0, player_r_ring = 0, player_l_ring = 0;
	int index = 1;
	struct linked_list * ptr = player.t_pack;
	while(ptr) {
		if((struct object *)ldata(ptr) == cur_weapon) {
			player_weapon = index;
		} else if((struct object *)ldata(ptr) == cur_armor) {
			player_armor = index;
		} else if((struct object *)ldata(ptr) == cur_ring[0]) {
			player_r_ring = index;
		} else if((struct object *)ldata(ptr) == cur_ring[1]) {
			player_l_ring = index;
		}
		ptr = next(ptr);
		++index;
	}
	fwrite(&player_weapon, 1, sizeof(player_weapon), savef);
	fwrite(&player_armor, 1, sizeof(player_armor), savef);
	fwrite(&player_r_ring, 1, sizeof(player_r_ring), savef);
	fwrite(&player_l_ring, 1, sizeof(player_l_ring), savef);

	for(index = 0; index < MAXDAEMONS; ++index) {
		fwrite(&d_list[index], 1, sizeof(struct delayed_action), savef);
		enum { NONE, DOCTOR, NOHASTE, ROLLWAND, RUNNERS, SIGHT, STOMACH, SWANDER, UNCONFUSE, UNSEE };
		int d = 0;
		if(d_list[index].d_func == doctor) {
			d = DOCTOR;
		} else if(d_list[index].d_func == nohaste) {
			d = NOHASTE;
		} else if(d_list[index].d_func == rollwand) {
			d = ROLLWAND;
		} else if(d_list[index].d_func == runners) {
			d = RUNNERS;
		} else if(d_list[index].d_func == sight) {
			d = SIGHT;
		} else if(d_list[index].d_func == stomach) {
			d = STOMACH;
		} else if(d_list[index].d_func == swander) {
			d = SWANDER;
		} else if(d_list[index].d_func == unconfuse) {
			d = UNCONFUSE;
		} else if(d_list[index].d_func == unsee) {
			d = UNSEE;
		}
		fwrite(&d, 1, sizeof(d), savef);
	}

	write_string_array(s_names, MAXSCROLLS, savef);
	write_string_array(p_colors, MAXPOTIONS, savef);
	write_string_array(r_stones, MAXRINGS, savef);
	write_string_array(ws_made, MAXSTICKS, savef);
	write_string_array(s_guess, MAXSCROLLS, savef);
	write_string_array(p_guess, MAXPOTIONS, savef);
	write_string_array(r_guess, MAXRINGS, savef);
	write_string_array(ws_guess, MAXSTICKS, savef);
	write_string_array(ws_type, MAXSTICKS, savef);

	write_win(cw, savef);
	write_win(mw, savef);
	write_win(hw, savef);
	write_win(stdscr, savef);
}

read_game(FILE *savef)
{
	READ_AS_IT_IS(rooms); READ_AS_IT_IS(rdes); READ_AS_IT_IS(between); READ_AS_IT_IS(level);
	READ_AS_IT_IS(purse); READ_AS_IT_IS(ntraps); READ_AS_IT_IS(no_move); READ_AS_IT_IS(no_command);
	READ_AS_IT_IS(inpack); READ_AS_IT_IS(max_hp); READ_AS_IT_IS(lastscore); READ_AS_IT_IS(no_food);
	READ_AS_IT_IS(count); READ_AS_IT_IS(fung_hit); READ_AS_IT_IS(quiet); READ_AS_IT_IS(max_level);
	READ_AS_IT_IS(food_left); READ_AS_IT_IS(group); READ_AS_IT_IS(hungry_state);
	READ_AS_IT_IS(whoami); READ_AS_IT_IS(fruit); READ_AS_IT_IS(ch_ret); READ_AS_IT_IS(nh);
	READ_AS_IT_IS(oldpos); READ_AS_IT_IS(delta); READ_AS_IT_IS(traps); READ_AS_IT_IS(huh);
	READ_AS_IT_IS(running); READ_AS_IT_IS(playing); READ_AS_IT_IS(wizard); READ_AS_IT_IS(after);
	READ_AS_IT_IS(notify); READ_AS_IT_IS(fight_flush); READ_AS_IT_IS(terse);
	READ_AS_IT_IS(door_stop); READ_AS_IT_IS(jump); READ_AS_IT_IS(slow_invent);
	READ_AS_IT_IS(firstmove); READ_AS_IT_IS(waswizard); READ_AS_IT_IS(askme); READ_AS_IT_IS(amulet);
	READ_AS_IT_IS(in_shell); READ_AS_IT_IS(take); READ_AS_IT_IS(runch); READ_AS_IT_IS(s_know);
	READ_AS_IT_IS(p_know); READ_AS_IT_IS(r_know); READ_AS_IT_IS(ws_know);
	READ_AS_IT_IS(max_stats);

	read_object_list(&lvl_obj, savef);
	read_thing(&player, savef);
	read_monster_list(&mlist, savef);

	int player_weapon = 0, player_armor = 0, player_r_ring = 0, player_l_ring = 0;
	fread(&player_weapon, 1, sizeof(player_weapon), savef);
	fread(&player_armor, 1, sizeof(player_armor), savef);
	fread(&player_r_ring, 1, sizeof(player_r_ring), savef);
	fread(&player_l_ring, 1, sizeof(player_l_ring), savef);
	int index = 1;
	struct linked_list * ptr = player.t_pack;
	cur_weapon = 0;
	cur_armor = 0;
	cur_ring[0] = 0;
	cur_ring[1] = 0;
	while(ptr) {
		if(player_weapon == index) {
			cur_weapon = (struct object *)ldata(ptr);
		} else if(player_armor == index) {
			cur_armor = (struct object *)ldata(ptr);
		} else if(player_r_ring == index) {
			cur_ring[0] = (struct object *)ldata(ptr);
		} else if(player_l_ring == index) {
			cur_ring[1] = (struct object *)ldata(ptr);
		}
		ptr = next(ptr);
		++index;
	}

	for(index = 0; index < MAXDAEMONS; ++index) {
		fread(&d_list[index], 1, sizeof(struct delayed_action), savef);
		int d = 0;
		fread(&d, 1, sizeof(d), savef);
		enum { NONE, DOCTOR, NOHASTE, ROLLWAND, RUNNERS, SIGHT, STOMACH, SWANDER, UNCONFUSE, UNSEE };
		switch(d) {
			case DOCTOR: d_list[index].d_func = doctor; break;
			case NOHASTE: d_list[index].d_func = nohaste; break;
			case ROLLWAND: d_list[index].d_func = rollwand; break;
			case RUNNERS: d_list[index].d_func = runners; break;
			case SIGHT: d_list[index].d_func = sight; break;
			case STOMACH: d_list[index].d_func = stomach; break;
			case SWANDER: d_list[index].d_func = swander; break;
			case UNCONFUSE: d_list[index].d_func = unconfuse; break;
			case UNSEE: d_list[index].d_func = unsee; break;
		}
	}

	read_string_array(s_names, MAXSCROLLS, savef);
	read_string_array(p_colors, MAXPOTIONS, savef);
	read_string_array(r_stones, MAXRINGS, savef);
	read_string_array(ws_made, MAXSTICKS, savef);
	read_string_array(s_guess, MAXSCROLLS, savef);
	read_string_array(p_guess, MAXPOTIONS, savef);
	read_string_array(r_guess, MAXRINGS, savef);
	read_string_array(ws_guess, MAXSTICKS, savef);
	read_string_array(ws_type, MAXSTICKS, savef);

	read_win(cw, savef);
	read_win(mw, savef);
	read_win(hw, savef);
	read_win(stdscr, savef);
}


typedef struct stat STAT;

extern char version[], encstr[];
//extern char *sys_errlist[], version[], encstr[];
extern bool _endwin;
//extern int errno;

char *sbrk();

STAT sbuf;

save_game()
{
    register FILE *savef;
    register int c;
    char buf[80];

    /*
     * get file name
     */
    mpos = 0;
    if (file_name[0] != '\0')
    {
	msg("Save game and exit (y/n)? ");
	do
	{
	    c = getchar();
	} while (c != 'n' && c != 'N' && c != 'y' && c != 'Y');
	mpos = 0;
	if (c == 'y' || c == 'Y')
	{
	    msg("File name: %s", file_name);
	    goto gotfile;
	}
    }
	return FALSE;

    do
    {
	msg("File name: ");
	mpos = 0;
	buf[0] = '\0';
	if (get_str(buf, cw) == QUIT)
	{
	    msg("");
	    return FALSE;
	}
	strcpy(file_name, buf);
gotfile:
	if ((savef = fopen(file_name, "w")) == NULL)
	    msg(strerror(errno));	/* fake perror() */
    } while (savef == NULL);

    /*
     * write out encrpyted file (after a stat)
     * The fwrite is to force allocation of the buffer before the write
     */
    save_file(savef);
    return TRUE;
}

/*
 * automatically save a file.  This is used if a HUP signal is
 * recieved
 */
void auto_save()
{
    register FILE *savef;
    register int i;

    for (i = 0; i < NSIG; i++)
	signal(i, SIG_IGN);
    if (file_name[0] != '\0' && (savef = fopen(file_name, "w")) != NULL)
	save_file(savef);
    exit(1);
}

/*
 * write the saved game on the file
 */
save_file(savef)
register FILE *savef;
{
    //wmove(cw, lines()-1, 0);
    //draw(cw);
    //fstat(fileno(savef), &sbuf);
    //fwrite("junk", 1, 5, savef);
    //fseek(savef, 0L, 0);
	fwrite(version, 1, strlen(version) + 1, savef);
	//encwrite(version, strlen(version) + 1, savef);
    _endwin = TRUE;
	write_game(savef);
    //encwrite(version, sbrk(0) - version, savef);
    fclose(savef);
}

restore(file, envp)
register char *file;
char **envp;
{
    register FILE* inf;
    extern char **environ;
    char buf[80];
    STAT sbuf2;

	// Reading.
    if (strcmp(file, "-r") == 0)
	file = file_name;
    if ((inf = fopen(file, "r")) == NULL)
    {
	perror(file);
	return FALSE;
    }

    fflush(stdout);
	fread(buf, 1, strlen(version) + 1, inf);
    //encread(buf, strlen(version) + 1, inf);
    if (strcmp(version, buf) != 0)
    {
	printf("Sorry, saved game is out of date.\n");
	return FALSE;
    }

    initscr();				/* Start up cursor package */
    crmode();				/* Cbreak mode */
    noecho();				/* Echo off */
    /*
     * Set up windows
     */
    cw = newwin(lines(), cols(), 0, 0);
    mw = newwin(lines(), cols(), 0, 0);
    hw = newwin(lines(), cols(), 0, 0);
    clearok(curscr, TRUE);
    touchwin(cw);

    fstat(fileno(inf), &sbuf2);
    fflush(stdout);
    //brk(version + sbuf2.st_size);
    //lseek(inf, 0L, 0);
	read_game(inf);
    //encread(version, (unsigned int) sbuf2.st_size, inf);
    /*
     * we do not close the file so that we will have a hold of the
     * inode for as long as possible
     */

	/* TODO check inode savescumming.
    if (!wizard)
	if (sbuf2.st_ino != sbuf.st_ino || sbuf2.st_dev != sbuf.st_dev)
	{
	    printf("Sorry, saved game is not in the same file.\n");
	    return FALSE;
	}
	else if (sbuf2.st_ctime - sbuf.st_ctime > 300)
	{
	    printf("Sorry, file has been touched.\n");
	    return FALSE;
	}
	*/
    mpos = 0;
    mvwprintw(cw, 0, 0, "%s: %s", file, ctime(&sbuf2.st_mtime));

    /*
     * defeat multiple restarting from the same place
     */
    if (!wizard)
	if (sbuf2.st_nlink != 1)
	{
	    printf("Cannot restore from a linked file\n");
	    return FALSE;
	}
	else if (unlink(file) < 0)
	{
	    printf("Cannot unlink file\n");
	    return FALSE;
	}

    environ = envp;
	/*
    if (!My_term && isatty(2))
    {
	register char	*sp;

	_tty_ch = 2;
	gettmode();
	if ((sp = getenv("TERM")) == NULL)
	    sp = Def_term;
	setterm(sp);
    }
    else
	setterm(Def_term);
	*/
    strcpy(file_name, file);
	seed = getpid();

	// Afterload.
    srand(seed);

    init_things();			/* Set up probabilities of things */
	int i;
    for (i = 0; i < MAXSCROLLS; i++) {
		if (i > 0)
			s_magic[i].mi_prob += s_magic[i-1].mi_prob;
	}
    badcheck("scrolls", s_magic, MAXSCROLLS);
    setup();
    waswizard = wizard;

    playit();
    /*NOTREACHED*/
}

/*
 * perform an encrypted write
 */
encwrite(start, size, outf)
register char *start;
unsigned int size;
register FILE *outf;
{
    register char *ep;

    ep = encstr;

    while (size--)
    {
	putc(*start++ ^ *ep++, outf);
	if (*ep == '\0')
	    ep = encstr;
    }
}

/*
 * perform an encrypted read
 */
encread(start, size, inf)
register char *start;
unsigned int size;
register int inf;
{
    register char *ep;
    register int read_size;

    if ((read_size = read(inf, start, size)) == -1 || read_size == 0)
	return read_size;

    ep = encstr;

    while (size--)
    {
	*start++ ^= *ep++;
	if (*ep == '\0')
	    ep = encstr;
    }
    return read_size;
}
