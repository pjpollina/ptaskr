#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "defs.h"

int render_header(WINDOW* w, struct tasklist tl) {
  // clear window
  wclear(w);

  // get center x position of window
  int mx = getmaxx(w);
  int lot = strlen(tl.name);
  int cpos = (mx / 2) - (lot / 2);

  // turn on attributes
  wattron(w, A_REVERSE);

  // render header
  mvwhline(w, 0, 0, ' ', mx);
  mvwprintw(w, 0, cpos, "%s", tl.name);

  // turn off attributes
  wattroff(w, A_REVERSE);

  // refresh window
  wrefresh(w);
  return EXIT_SUCCESS;
}

int render_task(WINDOW* w, struct task t, int line, int highlight) {
  // turn on relevant attributes
  if(line == highlight)
    wattron(w, A_REVERSE);
  if(task_complete(t))
    wattron(w, COLOR_PAIR(COMPLETE_COLOR));
  // render the task appropriately
  if(t.goal == 1)
    mvwprintw(w, line, 0, " - %s [%c]", t.desc, (task_complete(t)) ? 'x' : ' ');
  else
    mvwprintw(w, line, 0, " - %s [%d/%d]", t.desc, t.reached, t.goal);
  // turn off attributes
  wattroff(w, A_REVERSE);
  wattroff(w, COLOR_PAIR(COMPLETE_COLOR));
  return EXIT_SUCCESS;
}

int render_tasklist(WINDOW* w, struct tasklist tl, int pos) {
  // initialize local variables
  int rows = getmaxy(w);
  int page = pos / rows;
  // render list
  for(int i = 0; i < rows; i++) {
    if(i+(rows*page) == tl.task_count) {
      render_ntprompt(w, i, pos % rows);
      break;
    }
    render_task(w, tl.tasks[i+(rows*page)], i, pos % rows);
  }
  return EXIT_SUCCESS;
}

int render_ntprompt(WINDOW* w, int line, int highlight) {
  // turn on relevant attributes
  if(line == highlight)
    wattron(w, A_REVERSE);
  // render the task appropriately
  mvwprintw(w, line, 0, MENU_NEW_TASK);
  // turn off attributes
  wattroff(w, A_REVERSE);
  return EXIT_SUCCESS;
}