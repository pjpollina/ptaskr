#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defs.h"

int line_editor(char data[], int line, int col) {
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

int insert_into_string(char str[], char new, int pos) {
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

int remove_from_string(char str[], int pos) {
  if(pos != strlen(str))
    memmove(str + pos, str + pos + 1, (strlen(str) - 1));
  str[strlen(str) - 1] = '\0';
  return 0;
}