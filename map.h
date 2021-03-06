#ifndef MAP
#define MAP

#include <list>
#include "creature.h"
#include "item.h"
class Creature;

class Map{
    private:
      // Creatures are kept on a list 
      std::list<Creature*> creatureList;
    public:
      // The height and width of the map
      const static char width = 80;
      const static char height = 40;
      std::list<Item*> itemList;

      // Functions
      Map();
      // Intilizes the map object from a file
      int initMap();
      // Draws the visual content of the map  
      int drawMap(WINDOW* window);
      char map [height][width]; 
      int addCreature(int x, int y, char type);
      int drawMoveCreatures(WINDOW* window);
      void addItem(int x, int y, char type, int amount);
      void drawItems(WINDOW* window);
      void remove(Item* item);
};

#endif

