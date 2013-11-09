all: dungeon

dungeon: dungeon.cpp
	gcc dungeon.cpp -lncurses -o dungeon
