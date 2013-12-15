all: dungeon

dungeon: dungeon.o map.o creature.o
	g++ -g dungeon.cpp map.o creature.o -lncurses -o dungeon
	rm *.o

map.o : map.cpp map.h
	g++ -c -g map.cpp  

creature.o : creature.cpp creature.h
	g++ -c -g creature.cpp
