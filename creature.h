#ifndef CREATURE
#define CREATURE

class Creature{
  public:
    int drawCreature(WINDOW* window);
    Creature(int x, int y, char blit);

  private:
    int x;
    int y;
    char blit;
};

#endif
