#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#include "defs.h"

static int get_goal_input(int line) {
  // set up editor window
  WINDOW* w = newwin(1, getmaxx(stdscr), line+1, 0);
  keypad(w, true);
  curs_set(1);

  // allocate data
  int input = 0;
  char buffer[16];

  // logic
  int pos = 0;
  bool is_active = true;
  while(is_active) {
    // render
    wclear(w);
    mvwprintw(w, 0, 0, "%s", buffer);
    wmove(w, 0, pos);
    wrefresh(w);

    // editor
    int ch = wgetch(w);
    switch(ch) {
      // move the cursor horizontally
      case KEY_LEFT:
        if(pos != 0)
          pos--;
        break;
      case KEY_RIGHT:
        if(pos != strlen(buffer))
          pos++;
        break;
      // delete a char
      case KEY_BACKSPACE:
      case KEY_DC:
        if(pos != 0) {
          remove_from_string(buffer, pos);
          pos--;
        }
        break;
      // exit with enter key
      case 10:
        is_active = false;
        if(buffer[0] == '\0') {
          strcpy(buffer, "1");
        }
        break;
      // insert typed chars
      default:
        if(isdigit(ch) && ch != '-' && (strlen(buffer) != 6)) {
          insert_into_string(buffer, (char) ch, pos);
          pos++;
        }
        break;
    }
  }

  // parse input
  input = atoi(buffer);
  if(input > USHRT_MAX) {
    input = USHRT_MAX;
  }

  // close
  curs_set(0);
  keypad(w, false);
  return input;
}

int newtask_menu(struct tasklist* tl, int line) {
  // task values
  char desc[DESC_MAX];
  int goal = 0;

  // get string
  line_editor(desc, line, 0);

  // get goal
  goal = get_goal_input(line);

  // add to list
  add_task_to_list(tl, init_task(desc, goal));
  return 0;
}