#include "item.h"

Item::Item(int startX, int startY, char startBlit){
  x = startX;
  y = startY;
  blit = startBlit;
}

// Drawing the item on a window
void Item::draw(WINDOW* window){
  mvaddch(y, x, blit);
}
