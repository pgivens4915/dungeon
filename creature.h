#ifndef CREATURE
#define CREATURE

#include <stack>
#include <list>
#include "map.h"
#include "item.h"
#define MAX_PATH 5000

class Map;

class Creature{
  public:
    // The constructor that takes (x,y) as well as a char representation
    // of the creature to be blitted
    Creature(int x, int y, char blit);
    ~Creature();

    // Functions
    // Draws the creature on the screen at its current position
    int drawCreature(WINDOW* window);
    // Takes one step 
    int step(Map* map);
    int move(int x, int y, Map* map);
    void returnPath(struct Tile* currentTile);

    // Member variables
    std::stack<int> path;
    int x;
    int y;
    // Hunger should be a value from 0 to 100
    int hunger;
    bool dead;
    bool hungry;
  private:
    char blit;

    // Private functions
    // Empties the current path
    void emptyList();
    // TODO : Maybe not euclidian
    // Finds a path to the closest(Euclidian) food
    void findFood(Map* map);
    // Determines if we are on some food
    bool onFood(Map* map);
    void eat(Item* item);
};

#endif
