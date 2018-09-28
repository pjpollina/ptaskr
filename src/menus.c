#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "defs.h"

int tasklist_menu(struct tasklist* tl, char* filepath) {
  // set up windows
  WINDOW* header = newwin(1, getmaxx(stdscr), 0, 0);
  WINDOW* body = newwin(getmaxy(stdscr)-1, getmaxx(stdscr), 1, 0);

  // activate keyboard control
  keypad(body, true);

  // logic
  int pos = 0;
  bool is_active = true;
  bool changed_since_save = false;
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
        if(tl->tasks[pos].reached != 0) {
          tl->tasks[pos].reached--;
          changed_since_save = true;
        }
        break;
      case KEY_RIGHT:
        if(!task_complete(tl->tasks[pos])) {
          tl->tasks[pos].reached++;
          changed_since_save = true;
        }
        break;
      // edit current task
      case CTRL('e'):
        if(pos != tl->task_count) {
          keypad(body, false);
          line_edit_prompt(tl->tasks[pos].desc, pos % height, 3);
          keypad(body, true);
          changed_since_save = true;
        }
        break;
      // delete current task
      case CTRL('d'):
      case KEY_DC:
        if(pos != tl->task_count) {
          keypad(body, false);
          render_line_wipeout(pos % height);
          if(confirmation_prompt(pos % height, "Delete selected task?")) {
            remove_task_from_list(tl, pos);
            changed_since_save = true;
          }
          keypad(body, true);
          break;
        }
      // save list to file
      case CTRL('s'):
        keypad(body, false);
        render_line_wipeout(pos % height);
        if(confirmation_prompt(pos % height, "Write changes to disk?")) {
          save_prompt(tl, pos % height, filepath);
          changed_since_save = false;
        }
        keypad(body, true);
        break;
      // shift tasks
      case CTRL_UP:
        if(pos > 0 && pos != tl->task_count) {
          move_task_up(tl, pos);
          pos--;
          changed_since_save = true;
        }
        break;
      case CTRL_DOWN:
        if(pos < tl->task_count-1) {
          move_task_down(tl, pos);
          pos++;
          changed_since_save = true;
        }
        break;
      // rename list
      case CTRL('r'):
        keypad(body, false);
        rename_list_prompt(tl, pos % height);
        keypad(body, true);
        changed_since_save = true;
        break;
      // increase selected task goal
      case CTRL_LEFT:
        if(tl->tasks[pos].goal != 1) {
          tl->tasks[pos].goal--;
          changed_since_save = true;
        }
        break;
      case CTRL_RIGHT:
        if(tl->tasks[pos].goal != USHRT_MAX) {
          tl->tasks[pos].goal++;
          changed_since_save = true;
        }
        break;
      // new task check
      case 10:
        if(pos == tl->task_count) {
          keypad(body, false);
          new_task_prompt(tl, pos % height);
          keypad(body, true);
          changed_since_save = true;
        }
        break;
      // exit menu
      case KEY_F(1):
      case CTRL('x'):
        keypad(body, false);
        if(confirmation_prompt(pos % height, "Are you sure?")) {
          if(changed_since_save) {
            render_line_wipeout(pos % height);
            if(confirmation_prompt(pos % height, "Write unsaved changes to disk?"))
              save_prompt(tl, pos % height, filepath);
          }
          is_active = false;
        }
        keypad(body, true);
        break;
    }
  }
  // terminate
  keypad(body, false);
  delwin(header);
  delwin(body);
  return EXIT_SUCCESS;
}