#include <ncurses.h>
#include <string.h>
#include "map.h"
#define SPACE 32

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

int Map::addCreature(int x, int y, char type){
  creatureList[y][x].push_front(Creature(x, y, 'r'));
}

// Draws the creature and steps it
int Map::drawMoveCreatures(WINDOW* window){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      std::list<Creature>::iterator it;
      std::list<Creature>* list = &creatureList[i][j];
      for(it = list->begin(); it != list->end(); ++it){
        (*it).drawCreature(window);
        //(*it).step(this);
      }
    }
  }
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
  // mvprintw(2,0,"DEBUG %i %i", maxX, maxY);
}
