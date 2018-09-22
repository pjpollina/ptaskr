#include <ncurses.h>
#include <stdbool.h>
#include "defs.h"

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