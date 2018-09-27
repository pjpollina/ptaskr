#include <ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"

#define DEFAULT_FILEPATH "data/list.dat"

int main(int argc, char** argv) {
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

  // set filepath
  char filepath[4096];
  strcpy(filepath, (argc > 1) ? argv[1] : DEFAULT_FILEPATH);

  // test structs
  struct tasklist tl;
  int err = read_listfile(&tl, filepath);
  if(err) {
    tl = init_tasklist("Test List");
    for(int i = 1; i < 31; i++) {
      char str[20];
      sprintf(str, "Task #%d", i);
      add_task_to_list(&tl, init_task(str, (i > 15) ? 25 : 1));
    }
  }

  // logic
  tasklist_menu(&tl, filepath);

  // terminate
  endwin();
  return EXIT_SUCCESS;
}