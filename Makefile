all: dungeon

dungeon: dungeon.o map.o
	g++ dungeon.cpp map.o -lncurses -o dungeon
	rm *.o

map.o : map.cpp map.h
	g++ -c map.cpp  

