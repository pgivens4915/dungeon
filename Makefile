all: Dungeon

Dungeon: dungeon.cpp Map.o Creature.o
	g++ -g dungeon.cpp Map.o Creature.o -lncurses -o Dungeon

Map.o : map.cpp map.h
	g++ -c -g map.cpp -o Map.o

Creature.o : creature.cpp creature.h
	g++ -c -g creature.cpp -o Creature.o

clean :
	rm *.o Dungeon
