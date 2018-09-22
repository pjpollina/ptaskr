#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#include "defs.h"

static int insert_into_string(char str[], char new, int pos) {
  if(strlen(str) == pos) {
    str[pos] = new;
    str[pos + 1] = '\0';
    return 0;
  }
  memmove(str + pos + 1, str + pos, (strlen(str) + 1));
  str[pos] = new;
  str[strlen(str)] = '\0';
  return 0;
}

static int remove_from_string(char str[], int pos) {
  if(pos != strlen(str))
    memmove(str + pos, str + pos + 1, (strlen(str) - 1));
  str[strlen(str) - 1] = '\0';
  return 0;
}

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

int line_edit_prompt(char data[], int line, int col) {
  // set up editor window
  WINDOW* editor = newwin(1, 80, line+1, col);
  keypad(editor, true);
  curs_set(true);

  // init data
  int pos = strlen(data);
  char buffer[80];
  strcpy(buffer, data);

  // logic
  bool is_active = true;
  while(is_active) {
    wclear(editor);
    mvwprintw(editor, 0, 0, "%s", buffer);
    wmove(editor, 0, pos);
    wrefresh(editor);

    // input
    int ch = wgetch(editor);
    switch(ch) {
      // move the cursor horizontally
      case KEY_LEFT:
        if(pos != 0)
          pos--;
        break;
      case KEY_RIGHT:
        if(pos != strlen(data))
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
        break;
      // insert typed chars
      default:
        if(isprint(ch) && (strlen(buffer) != 79)) {
          insert_into_string(buffer, (char) ch, pos);
          pos++;
        }
        break;
    }
  }

  curs_set(0);
  strcpy(data, buffer);
  return 0;
}

int new_task_prompt(struct tasklist* tl, int line) {
  // task values
  char desc[DESC_MAX];
  int goal = 0;

  // get string
  line_edit_prompt(desc, line, 0);

  // get goal
  goal = get_goal_input(line);

  // add to list
  add_task_to_list(tl, init_task(desc, goal));
  return 0;
}

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