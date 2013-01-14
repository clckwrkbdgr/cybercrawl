# WARNING: Heavily relied on GNU make implicit rules!
CXX = g++
CPPFLAGS = -Wall -Wextra -Weffc++ -Werror
LDLIBS = -lncurses -lgpm
OBJECTS = ability.o acr.o bang.o beam.o chardump.o command.o debug.o decks.o describe.o direct.o dungeon.o effects.o fight.o files.o food.o invent.o it_use2.o it_use3.o item_use.o itemname.o items.o levels.o linuxlib.o llist.o maps.o message.o misc.o monplace.o mons_lev.o monstuff.o mstruct.o mstuff2.o mutation.o newgame.o ouch.o output.o player.o priest.o randart.o religion.o rumours.o shopping.o skills.o skills2.o spell.o spells.o spells0.o spells1.o spells2.o spells3.o stuff.o transfor.o view.o
INSTALLDIR = /usr/games
GAME = crawl

all: ${GAME}

install: ${GAME}
	cp $^ ${INSTALLDIR}

clean:
	rm -f *.o

distclean:
	rm -f *.o bones.* morgue.txt scores ${GAME}

check:
	cpplint * >CPPLINT 2>&1
	cppcheck -q --enable=all >CPPCHECK 2>&1

${GAME}: ${OBJECTS}
	${CXX} ${CPPFLAGS} -o $@ $^ ${LDLIBS}

ability.o: ability.h bang.h beam.h effects.h enum.h externs.h it_use2.h items.h linuxlib.h misc.h monplace.h player.h randart.h religion.h skills.h skills2.h spell.h spells1.h spells2.h spells3.h stuff.h view.h 
acr.o: ability.h chardump.h command.h debug.h describe.h direct.h dungeon.h effects.h enum.h externs.h fight.h files.h food.h invent.h it_use2.h it_use3.h item_use.h itemname.h items.h levels.h linuxlib.h message.h misc.h monplace.h monstuff.h mstruct.h mutation.h newgame.h ouch.h output.h player.h priest.h randart.h religion.h skills.h skills2.h spell.h spells.h spells2.h spells3.h stuff.h transfor.h view.h 
bang.o: bang.h beam.h direct.h effects.h enum.h externs.h fight.h itemname.h linuxlib.h message.h misc.h monstuff.h mstruct.h ouch.h output.h player.h skills.h skills2.h spell.h spells.h spells0.h spells1.h spells2.h spells3.h stuff.h view.h 
beam.o: bang.h beam.h chardump.h command.h debug.h decks.h describe.h direct.h dungeon.h effects.h enum.h externs.h fight.h files.h food.h invent.h it_use2.h it_use3.h item_use.h itemname.h items.h levels.h linuxlib.h maps.h message.h misc.h monplace.h monstuff.h mstruct.h mstuff2.h newgame.h ouch.h output.h player.h priest.h shopping.h skills.h skills2.h spell.h spells.h spells1.h spells2.h spells3.h stuff.h view.h 
chardump.o: enum.h externs.h itemname.h linuxlib.h message.h mutation.h player.h religion.h shopping.h skills.h spells0.h 
command.o: enum.h externs.h invent.h itemname.h items.h linuxlib.h message.h ouch.h spells0.h stuff.h 
debug.o: direct.h dungeon.h enum.h externs.h itemname.h linuxlib.h monplace.h mstruct.h player.h shopping.h skills.h spell.h stuff.h 
decks.o: effects.h enum.h externs.h food.h it_use2.h linuxlib.h misc.h monplace.h mutation.h ouch.h player.h religion.h spells.h spells1.h spells3.h stuff.h 
describe.o: defines.h enum.h externs.h itemname.h linuxlib.h mstruct.h player.h randart.h skills2.h stuff.h 
direct.o: describe.h enum.h externs.h itemname.h linuxlib.h monstuff.h mstruct.h player.h stuff.h view.h 
dungeon.o: enum.h externs.h itemname.h linuxlib.h maps.h mons_lev.h mstruct.h randart.h shopping.h 
effects.o: beam.h direct.h dungeon.h enum.h externs.h fight.h files.h itemname.h linuxlib.h misc.h mstruct.h mutation.h ouch.h player.h skills2.h spells0.h spells2.h spells3.h stuff.h view.h 
fight.o: bang.h beam.h chardump.h command.h debug.h decks.h describe.h direct.h dungeon.h effects.h enum.h externs.h fight.h files.h food.h invent.h it_use2.h it_use3.h item_use.h itemname.h items.h levels.h linuxlib.h maps.h message.h misc.h monplace.h mons_lev.h monstuff.h mstruct.h mstuff2.h mutation.h newgame.h ouch.h output.h player.h priest.h randart.h religion.h shopping.h skills.h skills2.h spell.h spells.h spells0.h spells1.h spells2.h spells3.h stuff.h view.h 
files.o: dungeon.h enum.h externs.h files.h itemname.h linuxlib.h message.h misc.h mstruct.h mstuff2.h output.h player.h randart.h skills2.h stuff.h view.h 
food.o: enum.h externs.h itemname.h linuxlib.h misc.h mstruct.h mutation.h player.h religion.h skills2.h spells2.h stuff.h 
invent.o: defines.h enum.h externs.h itemname.h linuxlib.h shopping.h stuff.h 
it_use2.o: beam.h effects.h enum.h externs.h itemname.h linuxlib.h misc.h mutation.h player.h randart.h skills2.h spells.h spells2.h stuff.h view.h 
it_use3.o: bang.h decks.h effects.h enum.h externs.h it_use2.h item_use.h itemname.h items.h linuxlib.h misc.h monplace.h monstuff.h player.h skills.h skills2.h spell.h spells.h spells0.h spells1.h spells2.h stuff.h view.h 
item_use.o: bang.h beam.h chardump.h command.h debug.h decks.h describe.h direct.h dungeon.h effects.h enum.h externs.h fight.h files.h food.h invent.h it_use2.h it_use3.h item_use.h itemname.h items.h levels.h linuxlib.h maps.h message.h misc.h monplace.h monstuff.h mstruct.h mstuff2.h newgame.h ouch.h output.h player.h priest.h randart.h religion.h skills.h skills2.h spell.h spells.h spells0.h spells1.h spells2.h spells3.h stuff.h transfor.h view.h 
itemname.o: enum.h externs.h itemname.h linuxlib.h mstruct.h randart.h skills2.h 
items.o: effects.h enum.h externs.h fight.h invent.h it_use2.h itemname.h linuxlib.h misc.h monplace.h mstruct.h mutation.h player.h randart.h religion.h skills.h stuff.h 
levels.o: enum.h externs.h linuxlib.h monplace.h mstruct.h stuff.h 
linuxlib.o: defines.h linuxlib.h 
llist.o: linuxlib.h llist.h 

maps.o: enum.h linuxlib.h monplace.h stuff.h 
message.o: enum.h externs.h linuxlib.h
misc.o: direct.h enum.h externs.h fight.h files.h it_use2.h items.h levels.h linuxlib.h misc.h monplace.h mstruct.h ouch.h output.h player.h shopping.h skills.h skills2.h spells.h spells1.h spells3.h stuff.h view.h 
monplace.o: dungeon.h enum.h externs.h linuxlib.h stuff.h 
mons_lev.o: enum.h externs.h linuxlib.h mstruct.h 
monstuff.o: beam.h enum.h externs.h fight.h itemname.h linuxlib.h misc.h monplace.h monstuff.h mstruct.h mstuff2.h ouch.h player.h stuff.h view.h 
mstruct.o: defines.h enum.h externs.h linuxlib.h m_list.h mstruct.h player.h 
mstuff2.o: bang.h beam.h effects.h enum.h externs.h fight.h itemname.h linuxlib.h monplace.h mstruct.h mstuff2.h player.h spells.h spells2.h stuff.h view.h 
mutation.o: enum.h externs.h linuxlib.h message.h player.h skills2.h stuff.h transfor.h 
newgame.o: enum.h externs.h itemname.h linuxlib.h player.h randart.h skills2.h stuff.h 
ouch.o: chardump.h enum.h externs.h files.h invent.h itemname.h linuxlib.h mstruct.h ouch.h player.h randart.h religion.h shopping.h skills2.h stuff.h view.h 
output.o: enum.h externs.h itemname.h linuxlib.h ouch.h player.h 
player.o: enum.h externs.h itemname.h linuxlib.h misc.h mstruct.h mutation.h output.h player.h priest.h randart.h religion.h skills2.h spells0.h stuff.h view.h 
priest.o: dungeon.h enum.h externs.h levels.h linuxlib.h message.h 
randart.o: enum.h externs.h itemname.h linuxlib.h randart.h stuff.h unrand.h 
religion.o: bang.h describe.h dungeon.h effects.h enum.h externs.h it_use2.h itemname.h linuxlib.h misc.h monplace.h mutation.h ouch.h player.h shopping.h spells.h spells1.h spells3.h stuff.h 
rumours.o: defines.h linuxlib.h 
shopping.o: describe.h enum.h externs.h invent.h itemname.h linuxlib.h player.h stuff.h 
skills.o: enum.h externs.h linuxlib.h player.h skills.h skills2.h stuff.h 
skills2.o: enum.h externs.h linuxlib.h 
spell.o: beam.h effects.h enum.h externs.h fight.h it_use2.h items.h linuxlib.h monplace.h mutation.h player.h religion.h skills.h spell.h spells.h spells0.h spells1.h spells2.h spells3.h stuff.h transfor.h view.h 
spells.o: bang.h beam.h effects.h enum.h externs.h invent.h it_use2.h it_use3.h items.h linuxlib.h misc.h monplace.h monstuff.h mutation.h ouch.h player.h religion.h spell.h spells.h spells0.h spells1.h spells3.h stuff.h view.h 
spells0.o: dungeon.h enum.h externs.h itemname.h linuxlib.h player.h 
spells1.o: beam.h direct.h enum.h externs.h fight.h invent.h it_use2.h itemname.h items.h linuxlib.h misc.h monplace.h monstuff.h mstruct.h player.h priest.h randart.h skills.h spells0.h spells2.h spells3.h stuff.h view.h 
spells2.o: beam.h direct.h enum.h externs.h fight.h itemname.h linuxlib.h message.h misc.h monplace.h monstuff.h mstruct.h ouch.h player.h stuff.h view.h 
spells3.o: beam.h direct.h enum.h externs.h fight.h it_use2.h itemname.h items.h linuxlib.h misc.h monplace.h monstuff.h mstruct.h player.h priest.h randart.h spells0.h spells1.h stuff.h view.h 
stuff.o: enum.h externs.h linuxlib.h message.h misc.h output.h skills2.h view.h 
transfor.o: enum.h externs.h it_use2.h itemname.h linuxlib.h skills2.h stuff.h 
view.o: enum.h externs.h linuxlib.h mstruct.h player.h stuff.h 
