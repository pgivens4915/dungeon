#ifndef MAP
#define MAP

#include <list>
#include "creature.h"
class Creature;

class Map{
    private:
      // The height and width of the map
      const static char width = 100;
      const static char height = 100;
      // Creatures are kept on a list depending on there square
      std::list<Creature*> creatureList[height][width];
    public:
      // Intilizes the map object from a file
      int initMap();
      // Draws the visual content of the map  
      int drawMap(WINDOW* window);
      char map [height][width]; 
      int addCreature(int x, int y, char type);
      int drawMoveCreatures(WINDOW* window);
};

#endif

