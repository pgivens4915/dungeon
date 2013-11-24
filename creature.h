#ifndef CREATURE
#define CREATURE

#include <queue>
#include <list>
#define MAX_PATH 5000

class Creature{
  public:
    // The constructor that takes (x,y) as well as a char representation
    // of the creature to be blitted
    Creature(int x, int y, char blit);
    // Draws the creature on the screen at its current position
    int drawCreature(WINDOW* window);
    // Takes one step 
    int step();
    int move(int x, int y);
    

  private:
    std::queue<int> path;
    int x;
    int y;
    char blit;
};

#endif
