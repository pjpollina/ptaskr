#include <ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "defs.h"

#define FILEPATH "data/list.dat"

int main() {
  // ncurses setup
  initscr();
  raw();
  noecho();
  curs_set(false);

  // color setup
  if(!has_colors()) {
    endwin();
    fprintf(stderr, "Your terminal does not support color\n");
    exit(1);
  }
  start_color();
  use_default_colors();
  init_pair(COMPLETE_COLOR, COLOR_GREEN, -1);

  // test structs
  struct tasklist tl;
  int err = read_listfile(&tl, FILEPATH);
  if(err) {
    tl = init_tasklist("Test List");
    for(int i = 1; i < 31; i++) {
      char str[20];
      sprintf(str, "Task #%d", i);
      add_task_to_list(&tl, init_task(str, (i > 15) ? 25 : 1));
    }
  }

  // logic
  tasklist_menu(&tl);
  write_listfile(&tl, FILEPATH);
  endwin();
}