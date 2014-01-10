#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#define SPACE 32

Map::Map(){
}

int Map::initMap(){
  // Declarations
  FILE* mapFile;
  // Where width is the max width of the map  
  char line[width];
  int i = 0;
  int stringLength;

  // Zeroing the array
  memset(map, SPACE,  width * height);

  // Opening the file and reading its contents into the map object
  mapFile = fopen("map.map", "r");
  // Handling fopen errors
  if(mapFile == NULL){
    printf("Map file does not open\n");
    return(-1);
  }

  // Puting the contents of the file into the map array
  while(fgets(line, sizeof(line), mapFile)){
    // Copying the string
    strncpy(map[i],line, sizeof(line));
    // Filling the rest of the map line with spaces
    stringLength = strlen(line);
    for(int j = stringLength; j < width; j++){
      map[i][j] = SPACE;
    }
    i++;
  }
  return(0);
}

// Adding a creature
int Map::addCreature(int x, int y, char type){
  Creature* creaturePoint;
  creaturePoint = new Creature(x, y, type);
  creatureList.push_front(creaturePoint);
  return(0);
}

// Adding an item
void Map::addItem(int x, int y, char type, int amount){
  Item* itemPoint;
  itemPoint = new Item(x, y, type, amount);
  itemList.push_front(itemPoint);
}

// Draws an Item
void Map::drawItems(WINDOW* window){
  std::list<Item*>::iterator it;
  for(it = itemList.begin(); it != itemList.end(); it++){
    (*it)->draw(window);
  }
}

// Draws the creature and steps it
int Map::drawMoveCreatures(WINDOW* window){
  std::list<Creature*>::iterator it;
  std::list<Creature*>* list = &creatureList;
  for(it = list->begin(); it != list->end(); ++it){
    if ((*it)->dead){
      list->erase(it);
      it--;
      // TODO get this delete working right
      //delete(*it);
    }
    else{
      std::list<Item*>::iterator ij;
      int i = 0;
      for(ij = itemList.begin(); ij != itemList.end(); ij++){
        mvprintw(21, 0 + i*8, ":%i,%i", (*ij)->x, (*ij)->y);
        i++;
      }
      (*it)->step(this);
      (*it)->drawCreature(window);
    }
  }
  return(0);
}

// Prints out the entire map
int Map::drawMap(WINDOW* window){
  // Declarations
  int maxX = 0;
  int maxY = 0;

  //Getting the terminal size
  getmaxyx(window, maxY, maxX);
  // Prints out the entirity of the map array
  // This might exceed the terminal size
  for(int i = 0; (i < height) && (i < maxY); i++){
    for(int j = 0; (j < width) && (j < maxX); j++){
      mvaddch(i,j, map[i][j]);
    }    
  }
  return(0);
}

// Removes an Item from the list
void Map::remove(Item* item){
  itemList.remove(item);
  delete item;
}
