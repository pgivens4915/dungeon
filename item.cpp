#include "item.h"

Item::Item(int startX, int startY, char startBlit, int startAmount){
  x = startX;
  y = startY;
  blit = startBlit;
  amount = startAmount;
}

Item::~Item(){
  x = 0;
  y = 0;
}

// Drawing the item on a window
void Item::draw(WINDOW* window){
  mvaddch(y, x, blit);
}

