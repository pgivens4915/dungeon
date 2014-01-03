#ifndef ITEM
#define ITEM

#include <ncurses.h>

class Item{
  public:
    Item(int x, int y, char blit);

    // Functions
    void draw(WINDOW* window);

    // Member variables
    int x;
    int y;
    char blit;

  private:
};

#endif
