#include <ncurses.h>
#include "map.h"

// Prints out the entire map
int Map::drawMap(WINDOW* window){
  // Declarations
  int maxX = 0;
  int maxY = 0;

  map[0][0] = 'M';
  map[0][1] = 'A';
  map[0][2] = 'P';

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
