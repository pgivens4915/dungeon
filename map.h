#ifndef MAP
#define MAP

class Map{
    public:
      int drawMap(WINDOW* window);
    private:
      // The height and width of the map
      const static char width = 100;
      const static char height = 100;
      char map [height][width]; 
};

#endif

