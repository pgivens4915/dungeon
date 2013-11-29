#ifndef CREATURE
#define CREATURE

#include <stack>
#include <list>
#include "map.h"
#define MAX_PATH 5000

class Map;

class Creature{
  public:
    // The constructor that takes (x,y) as well as a char representation
    // of the creature to be blitted
    Creature(int x, int y, char blit);
    // Draws the creature on the screen at its current position
    int drawCreature(WINDOW* window);
    // Takes one step 
    int step();
    int move(int x, int y, Map* map);
    void returnPath(struct Tile* currentTile);
    

  private:
    std::stack<int> path;
    int x;
    int y;
    char blit;
};

#endif
