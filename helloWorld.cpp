#include <ncurses.h>

int main(){
  WINDOW* mainWindow;
  mainWindow = initscr();
  printw("Hello World");
  refresh();
  getch();
  endwin();

  return 0;
}
