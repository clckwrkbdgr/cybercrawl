#include "curses.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "rogue.h"
#include "machdep.h"

#define getuid() Uid

#define SCORE_WIN 4

#undef SCOREFILE
#define SCOREFILE "testroll"

int	Uid;

main()
{
    static int amount;

    for (;;) {
	printf("> ");
	scanf("%d %d %s", &Uid, &amount, whoami);
	score(amount, 1, 'B');
    }
}

struct sc_ent {
    int sc_score;
    int sc_uid;
    char sc_name[80];
    int sc_flags;
    int sc_level;
    char sc_monster;
};

typedef struct sc_ent	SCORE;

SCORE	*scores;

score(amount, flags, monst)
register int amount, flags;
register char monst;
{
    register SCORE *sp, *myscore;
    register int fd;
    register FILE *outf;
    register int uid, i, size;
    register SCORE *endsp;
    SCORE sbuf;
    static struct stat stbuf;
    int	endit(), sccomp();

    /*
     * Open file and read list
     */
    fd = open(SCOREFILE, 2);
    if (fd < 0)
	return;
    outf = fdopen(fd, "w");

    signal(SIGINT, SIG_DFL);
#if 0
    if (flags != -1)
    {
	endwin();
	printf("[Press return to continue]");
	wait_for('\n');
    }
#endif
    sp = &sbuf;
    uid = getuid();
    fstat(fd, &stbuf);
    size = -1;
    for (i = 0; encread(sp, sizeof (SCORE), fd) > 0; i++)
	if (sp->sc_uid == uid)
	{
	    size = stbuf.st_size;
	    break;
	}
    printf("uid = %d, size = %d", uid, size);
    if (size == -1)
	size = stbuf.st_size + sizeof (SCORE), printf("(%d)", size);
    scores = (SCORE *) malloc(size);
    printf(", i = %d\n", i);
    lseek(fd, 0L, 0);
    for (sp = scores; encread(sp, sizeof (SCORE), fd) > 0; sp++)
	continue;
    /*
     * Insert her in list if need be
     */
    sp = &scores[i];
    if (!waswizard && (sp->sc_score < amount || sp->sc_uid != uid))
    {
	sp->sc_score = amount;
	strcpy(sp->sc_name, whoami);
	sp->sc_flags = flags;
	if (flags == 2)
	    sp->sc_level = max_level;
	else
	    sp->sc_level = level;
	sp->sc_monster = monst;
	sp->sc_uid = uid;
    }
    /*
     * Print the list
     */
    printf("Rank\tScore\tName\n");
    size /= sizeof (SCORE);
    qsort(scores, size, sizeof (SCORE), sccomp);
    for (myscore = scores; myscore->sc_uid != uid; myscore++)
	continue;
    sp = (sp - SCORE_WIN < scores ? scores : sp - SCORE_WIN);
    endsp = (sp + SCORE_WIN >= &scores[size] ? &scores[size-1] : sp + SCORE_WIN);
    while (sp <= endsp)
	prscore(sp++);
    fseek(outf, (long) i * sizeof (SCORE), 0);
    encwrite(myscore, sizeof (SCORE), outf);
    fclose(outf);
}

prscore(sp)
register SCORE *sp;
{
    register char *killer;
    static char *reason[] = {
	"killed",
	"quit",
	"A total winner",
    };
    char *killname();

    printf("%d\t%d\t%s: %s on level %d", sp - scores + 1, sp->sc_score,
	sp->sc_name, reason[sp->sc_flags], sp->sc_level);
    if (sp->sc_flags == 0) {
	killer = killname(sp->sc_monster);
	printf(" by a%s %s", vowelstr(killer), killer);
    }
    printf(".\n");
}

sccomp(s1, s2)
SCORE *s1, *s2;
{
    return s2->sc_score - s1->sc_score;
}
