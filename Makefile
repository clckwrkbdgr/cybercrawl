#
# Makefile for rogue
# %W% (Berkeley) %G%
#
HDRS=	rogue.h machdep.h
OBJS=	vers.o armor.o chase.o command.o daemon.o daemons.o fight.o \
	init.o io.o list.o main.o misc.o monsters.o move.o newlevel.o \
	options.o pack.o passages.o potions.o rings.o rip.o rooms.o \
	save.o scrolls.o sticks.o things.o weapons.o wizard.o
POBJS=	vers.po armor.po chase.po command.po daemon.po daemons.po fight.po \
	init.po io.po list.po main.po misc.po monsters.po move.po newlevel.po \
	options.po pack.po passages.po potions.po rings.po rip.po rooms.po \
	save.po scrolls.po sticks.po things.po weapons.po wizard.po
CFILES=	vers.c armor.c chase.c command.c daemon.c daemons.c fight.c \
	init.c io.c list.c main.c misc.c monsters.c move.c newlevel.c \
	options.c pack.c passages.c potions.c rings.c rip.c rooms.c \
	save.c scrolls.c sticks.c things.c weapons.c wizard.c
CFLAGS= -O -Werror
PROFLAGS= -p -O
#LDFLAGS=-i	# For PDP-11's
#LDFLAGS=	# For VAXes
VGRIND=/usr/ucb/vgrind
CRLIB= -lcurses
#CRLIB=	/usr/lib/libncurses.a
#CRLIB=	/ra/csr/arnold/=lib/=curses/crlib
#CRLIB=	/ra/csr/toy/_nc/crlib
PCRLIB= -lcurses
MISC=	Makefile TODO
# for sites without sccs front end, GET= get
#GET=	sccs get
GET=	echo

.SUFFIXES: .po

.c.po:
	@echo $(CC) -c $(PROFLAGS) $?
	@rm -f x.c
	@ln $? x.c
	@$(CC) -c $(PROFLAGS) x.c
	@mv x.o $*.po

.DEFAULT:
	$(GET) $@

rogue: newvers a.out
	cp a.out rogue
	strip rogue

a.out: $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(CRLIB)
	size a.out

k.out: $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(CRLIB) -ltermlib -o k.out

install: rogue
	cp rogue $(DESTDIR)/u/games/rogue

p.out:	$(HDRS) $(POBJS)
	@rm -f x.c
	$(CC) $(PROFLAGS) $(LDFLAGS) $(POBJS) $(PCRLIB) -ltermlib -o p.out
	size p.out

newvers:
	$(GET) -e vers.c

main.o rip.o: machdep.h

tags: $(HDRS) $(CFILES)
	ctags -u $?
	ed - tags < :ctfix
	sort tags -o tags

lint:
	lint -hxbc $(CFILES) -lcurses > linterrs

clean:
	rm -f $(POBJS) $(OBJS) core rogue make.out ? rogue.tar vgrind.*

count:
	wc -l $(HDRS) $(CFILES)

realcount:
	cc -E $(CFILES) | ssp - | wc -l

update:
	ar uv .SAVE $(CFILES) $(HDRS) Makefile

dist:
	cp $(CFILES) $(HDRS) Makefile /ra/csr/toy/_dist

xtar: $(CFILES) $(HDRS) $(MISC)
	rm -f rogue.tar
	tar cf rogue.tar $? :ctfix
	touch xtar

vgrind:
	@csh $(VGRIND) -t -h "Rogue Version 3.7" $(HDRS) *.c > vgrind.out
	@ctags -v $(HDRS) *.c > index
	@csh $(VGRIND) -t -x index > vgrind.out.tbl

cfiles: $(CFILES)
