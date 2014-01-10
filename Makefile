CFLAGS = -std=c++11 -Wall -O3 -pg -g
C = g++-4.7

all: Dungeon

Dungeon: dungeon.cpp Map.o Creature.o Item.o
	$(C) $(CFLAGS) dungeon.cpp Map.o Creature.o Item.o -lncurses -o Dungeon

Map.o : map.cpp map.h
	$(C) $(CFLAGS) -c map.cpp -o Map.o

Creature.o : creature.cpp creature.h
	$(C) $(CFLAGS) -c creature.cpp -o Creature.o

Item.o : item.cpp item.h
	$(C) $(CFLAGS) -c item.cpp -o Item.o

clean :
	rm *.o Dungeon *.txt *.log *.out
