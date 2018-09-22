#include <ncurses.h>
#include <string.h>

#include "defs.h"

int tasklist_menu(struct tasklist* tl) {
  // set up windows
  WINDOW* header = newwin(1, getmaxx(stdscr), 0, 0);
  WINDOW* body = newwin(getmaxy(stdscr)-1, getmaxx(stdscr), 1, 0);

  // activate keyboard control
  keypad(body, true);

  // logic
  int pos = 0;
  int is_active = true;
  while(is_active) {
    // check for term resize
    if(getmaxx(stdscr) != getmaxx(body) || getmaxy(stdscr) != getmaxy(body)) {
      wresize(header, 1, getmaxx(stdscr));
      wresize(body, getmaxy(stdscr)-1, getmaxx(stdscr));
    }
    
    // get screen values
    int height = getmaxy(body);

    // rendering  
    render_header(header, tl[0]);
    wclear(body);
    render_tasklist(body, tl[0], pos);
    wrefresh(body);

    // input
    int ch = wgetch(body);
    switch(ch) {
      // key scrolling
      case KEY_UP:
        pos--;
        if(pos < 0)
          pos++;
        break;
      case KEY_DOWN:
        pos++;
        if(pos > tl->task_count)
          pos = tl->task_count;
        break;
      // page jumps
      case KEY_PPAGE:
        if(pos >= height) {
          pos -= height;
        } else {
          pos = 0;
        }
        break;
      case KEY_NPAGE:
        if(pos + height <= tl->task_count) {
          pos += height;
        } else {
          pos = tl->task_count;
        }
        break;
      // increment selected task
      case KEY_LEFT:
        if(tl->tasks[pos].reached != 0)
          tl->tasks[pos].reached--;
        break;
      case KEY_RIGHT:
        if(!task_complete(tl->tasks[pos]))
          tl->tasks[pos].reached++;
        break;
      // edit current task
      case 'e':
        if(pos != tl->task_count) {
          keypad(body, false);
          line_editor(tl->tasks[pos].desc, pos % height, 3);
          keypad(body, true);
        }
        break;
      // delete current task
      case 'd':
      case KEY_DC:
        if(pos != tl->task_count) {
          keypad(body, false);
          if(confirmation_prompt(pos % height)) {
            remove_task_from_list(tl, pos);
          }
          keypad(body, true);
          break;
        }
      // new task check
      case 10:
        if(pos == tl->task_count) {
          keypad(body, false);
          newtask_menu(tl, pos % height);
          keypad(body, true);
        }
        break;
      // exit menu
      case KEY_F(1):
        is_active = false;
        break;
    }
  }
  return 0;
}

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
  return 0;
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
  return 0;
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
  return 0;
}

int render_ntprompt(WINDOW* w, int line, int highlight) {
  // turn on relevant attributes
  if(line == highlight)
    wattron(w, A_REVERSE);
  // render the task appropriately
  mvwprintw(w, line, 0, MENU_NEW_TASK);
  // turn off attributes
  wattroff(w, A_REVERSE);
  return 0;
}

/*
bool confirmation_prompt(int line) {
  // set up prompt window
  WINDOW* prompt = newwin(1, 25, line+1, 0);
  keypad(prompt, true);
  curs_set(true);

  // values
  char yn = '\0';

  // logic
  bool is_active = true;
  while(is_active) {
    // rendering
    wclear(prompt);
    mvwprintw(prompt, 0, 0, "Are you sure? (y/n): %c", yn);
    wrefresh(prompt);

    // input
    int input = wgetch(prompt);
    switch(input) {
      // delete current input
      case KEY_BACKSPACE:
      case KEY_DC:
        yn = '\0';
        break;
      // confirm input with enter key
      case 10:
        is_active = false;
        break;
      // check for valid input
      default:
        if(yn == '\0' && (input == 'y' || input == 'n')) {
          yn = input;
        }
        break;
    }
  }

  // remove window
  keypad(prompt, false);
  curs_set(false);
  delwin(prompt);

  // return
  return (yn == 'y');
}
*/