all: Dungeon

Dungeon: dungeon.cpp Map.o Creature.o Item.o
	g++ -pg -g dungeon.cpp Map.o Creature.o Item.o -lncurses -o Dungeon

Map.o : map.cpp map.h
	g++ -pg -c -g map.cpp -o Map.o

Creature.o : creature.cpp creature.h
	g++ -pg -c -g creature.cpp -o Creature.o

Item.o : item.cpp item.h
	g++ -pg -c -g item.cpp -o Item.o

clean :
	rm *.o Dungeon *.txt *.log *.out
