# WARNING: Heavily relied on GNU make implicit rules!
CXX = g++
CPPFLAGS = -Wall -Werror
LDLIBS = -lncurses -lgpm
OBJECTS = ability.o acr.o bang.o beam.o chardump.o command.o debug.o decks.o describe.o direct.o dungeon.o effects.o fight.o files.o food.o invent.o it_use2.o it_use3.o item_use.o itemname.o items.o levels.o linuxlib.o llist.o macro.o maps.o message.o misc.o monplace.o mons_lev.o monstuff.o mstruct.o mstuff2.o mutation.o newgame.o ouch.o output.o player.o priest.o randart.o religion.o rumours.o shopping.o skills.o skills2.o spell.o spells.o spells0.o spells1.o spells2.o spells3.o stuff.o transfor.o view.o
INSTALLDIR = /usr/games
GAME = crawl

all: ${GAME}

install: ${GAME}
		cp $^ ${INSTALLDIR}

clean:
		rm -f *.o

distclean:
		rm -f *.o bones.* morgue.txt scores ${GAME}

${GAME}: ${OBJECTS}
	${CXX} ${CPPFLAGS} -o $@ $^ ${LDLIBS}

