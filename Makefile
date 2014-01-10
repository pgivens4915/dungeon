CFLAGS = -std=c++11 -Wall -O3 -pg -g

all: Dungeon

Dungeon: dungeon.cpp Map.o Creature.o Item.o
	g++ $(CFLAGS) dungeon.cpp Map.o Creature.o Item.o -lncurses -o Dungeon

Map.o : map.cpp map.h
	g++ $(CFLAGS) -c map.cpp -o Map.o

Creature.o : creature.cpp creature.h
	g++ $(CFLAGS) -c creature.cpp -o Creature.o

Item.o : item.cpp item.h
	g++ $(CFLAGS) -c item.cpp -o Item.o

clean :
	rm *.o Dungeon *.txt *.log *.out
