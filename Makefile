#
# Makefile for rogue
# %W% (Berkeley) %G%
#
VERSION=$(shell git tag | sed 's/.*\([0-9]\+\.[0-9]\+\.[0-9]\+\)/\1/' | sort -nt . | tail -1)

HDRS=	rogue.h machdep.h
CFILES=	vers.c armor.c chase.c command.c daemon.c daemons.c fight.c \
	init.c io.c list.c main.c misc.c monsters.c move.c newlevel.c \
	options.c pack.c passages.c potions.c rings.c rip.c rooms.c \
	save.c scrolls.c sticks.c things.c weapons.c wizard.c
TEST_FILES = $(wildcard tests/*.c)
$(shell mkdir -p tmp)
$(shell mkdir -p tmp/tests)
OBJS = $(addprefix tmp/,$(CFILES:.c=.o)) $(addprefix tmp/,$(TEST_FILES:.c=.o))
POBJS = $(addprefix tmp/,$(CFILES:.c=.po))
CFLAGS= -O # FIXME -Werror
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

rogue: newvers a.out
	cp a.out rogue
	strip rogue

test: rogue
	./rogue test

.c.po:
	@echo $(CC) -c $(PROFLAGS) $?
	@rm -f x.c
	@ln $? x.c
	@$(CC) -c $(PROFLAGS) x.c
	@mv x.o $*.po

#.DEFAULT:
#	$(GET) $@

tmp/%.o: %.c $(HRDS)
	$(CC) $(CFLAGS) -c $< -o $@

load: rogue
	screen sh -c './rogue -r || exec bash'

run: rogue
	screen sh -c './rogue || exec bash'

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

deb: rogue
	@debpackage.py \
		cybercrawl \
		-v $(VERSION) \
		--maintainer 'umi041 <umi0451@gmail.com>' \
		--bin rogue \
		--build-dir tmp \
		--dest-dir . \
		--description 'Original Rogue clone in cyberpunk setting.'
