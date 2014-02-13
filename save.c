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

#ifdef VARIABLES_TO_SAVE
// Contains d_func() pointer
delayed_action d_list[MAXDAEMONS]
// Recreate linked_list of thing
struct linked_list *mlist; // contains coord * t_dest and linked_list[object] * t_pack and stats (s_dmg);
// Recreate linked_list of object: contains o_text, o_damage, o_hurldamage
struct linked_list *lvl_obj;		/* List of objects on this level */
// contains coord * t_dest and linked_list[object] * t_pack and stats (s_dmg);
struct thing player;			/* The rogue */
// Contains s_dmg
struct stats max_stats;			/* The maximum for the player */

// Save/load as it is.
struct room rooms[MAXROOMS];		/* One for each room -- A level */
struct rdes rdes[MAXROOMS];
int between;
int level;				/* What level rogue is on */
int purse;				/* How much gold the rogue has */
int ntraps;				/* Number of traps on this level */
int no_move;				/* Number of turns held in place */
int no_command;				/* Number of turns asleep */
int inpack;				/* Number of things in pack */
int max_hp;				/* Player's max hit points */
int lastscore;				/* Score before this turn */
int no_food;				/* Number of levels without food */
int count;				/* Number of times to repeat command */
int fung_hit;				/* Number of time fungi has hit */
int quiet;				/* Number of quiet turns */
int max_level;				/* Deepest player has gone */
int food_left;				/* Amount of food in hero's stomach */
int group;				/* Current group number */
int hungry_state;			/* How hungry is he */
char whoami[80];			/* Name of player */
char fruit[80];				/* Favorite fruit */
coord ch_ret
coord nh;
coord oldpos;				/* Position before last look() call */
coord delta;				/* Change indicated to get_dir() */
traps[MAXTRAPS];
char huh[80];				/* The last message printed */
bool running;				/* True if player is running */
bool playing;				/* True until he quits */
bool wizard;				/* True if allows wizard commands */
bool after;				/* True if we want after daemons */
bool notify;				/* True if player wants to know */
bool fight_flush;			/* True if toilet input */
bool terse;				/* True if we should be short */
bool door_stop;				/* Stop running when we pass a door */
bool jump;				/* Show running as series of jumps */
bool slow_invent;			/* Inventory one line at a time */
bool firstmove;				/* First move after setting door_stop */
bool waswizard;				/* Was a wizard sometime */
bool askme;				/* Ask about unidentified things */
bool amulet;				/* He found the amulet */
bool in_shell;				/* True if executing a shell */
char take;				/* Thing the rogue is taking */
char runch;				/* Direction player is running */
bool s_know[MAXSCROLLS];		/* Does he know what a scroll does */
bool p_know[MAXPOTIONS];		/* Does he know what a potion does */
bool r_know[MAXRINGS];			/* Does he know what a ring does */
bool ws_know[MAXSTICKS];		/* Does he know what a stick does */

// From player pack.
struct object *cur_weapon;		/* Which weapon he is weilding */
struct object *cur_armor;		/* What a well dresssed rogue wears */
struct object *cur_ring[2];		/* Which rings are being worn */

// Strings. Recreate using new().
char *s_names[MAXSCROLLS];		/* Names of the scrolls */
char *p_colors[MAXPOTIONS];		/* Colors of the potions */
char *r_stones[MAXRINGS];		/* Stone settings of the rings */
char *ws_made[MAXSTICKS];		/* What sticks are made of */
char *s_guess[MAXSCROLLS];		/* Players guess at what scroll is */
char *p_guess[MAXPOTIONS];		/* Players guess at what potion is */
char *r_guess[MAXRINGS];		/* Players guess at what ring is */
char *ws_guess[MAXSTICKS];		/* Players guess at what wand is */
char *ws_type[MAXSTICKS];		/* Is it a wand or a staff */

#endif

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
	msg("Save file (%s)? ", file_name);
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
    wmove(cw, lines()-1, 0);
    draw(cw);
    fstat(fileno(savef), &sbuf);
    fwrite("junk", 1, 5, savef);
    fseek(savef, 0L, 0);
    _endwin = TRUE;
    encwrite(version, sbrk(0) - version, savef);
    fclose(savef);
}

restore(file, envp)
register char *file;
char **envp;
{
    register int inf;
    extern char **environ;
    char buf[80];
    STAT sbuf2;

    if (strcmp(file, "-r") == 0)
	file = file_name;
    if ((inf = open(file, 0)) < 0)
    {
	perror(file);
	return FALSE;
    }

    fflush(stdout);
    encread(buf, strlen(version) + 1, inf);
    if (strcmp(buf, version) != 0)
    {
	printf("Sorry, saved game is out of date.\n");
	return FALSE;
    }

    fstat(inf, &sbuf2);
    fflush(stdout);
    brk(version + sbuf2.st_size);
    lseek(inf, 0L, 0);
    encread(version, (unsigned int) sbuf2.st_size, inf);
    /*
     * we do not close the file so that we will have a hold of the
     * inode for as long as possible
     */

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
    init_names();			/* Set up names of scrolls */
    init_colors();			/* Set up colors of potions */
    init_stones();			/* Set up stone settings of rings */
    init_materials();			/* Set up materials of wands */
    setup();
    /*
     * Set up windows
     */
    cw = newwin(lines(), cols(), 0, 0);
    mw = newwin(lines(), cols(), 0, 0);
    hw = newwin(lines(), cols(), 0, 0);
    waswizard = wizard;
    clearok(curscr, TRUE);
    touchwin(cw);

	oldrp = roomin(&oldpos);

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
