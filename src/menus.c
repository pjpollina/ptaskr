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
      case CTRL('e'):
        if(pos != tl->task_count) {
          keypad(body, false);
          line_edit_prompt(tl->tasks[pos].desc, pos % height, 3);
          keypad(body, true);
        }
        break;
      // delete current task
      case CTRL('d'):
      case KEY_DC:
        if(pos != tl->task_count) {
          keypad(body, false);
          if(confirmation_prompt(pos % height)) {
            remove_task_from_list(tl, pos);
          }
          keypad(body, true);
          break;
        }
      // save list to file
      case CTRL('s'):
        if(confirmation_prompt(pos % height)) {
          keypad(body, false);
          save_prompt(tl, pos % height);
          keypad(body, true);
        }
      // shift tasks
      case CTRL_UP:
        if(pos > 0) {
          move_task_up(tl, pos);
          pos--;
        }
        break;
      case CTRL_DOWN:
        if(pos < tl->task_count-1) {
          move_task_down(tl, pos);
          pos++;
        }
        break;
      // new task check
      case 10:
        if(pos == tl->task_count) {
          keypad(body, false);
          new_task_prompt(tl, pos % height);
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