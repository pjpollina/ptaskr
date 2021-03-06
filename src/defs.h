#ifndef _TASKLIST_DEFS_H_
#define _TASKLIST_DEFS_H_

#include <ncurses.h>
#include <stdbool.h>

#define DESC_MAX 80
#define TASK_LIST_MAX 256

#define COMPLETE_COLOR 1
#define MENU_NEW_TASK "[New Task]"

#ifdef __APPLE__
  #define CTRL_UP 562
  #define CTRL_DOWN 521
  #define CTRL_LEFT 541
  #define CTRL_RIGHT 556
#else
  #define CTRL_UP 566
  #define CTRL_DOWN 525
  #define CTRL_LEFT 545
  #define CTRL_RIGHT 560
#endif

#define KEY_MAC_BACKSPACE 127

#define CTRL(key) ((key) & 037)

struct task {
  char desc[DESC_MAX];
  unsigned short goal;
  unsigned short reached;
};

struct tasklist {
  char name[DESC_MAX];
  unsigned short task_count;
  struct task tasks[TASK_LIST_MAX];
};

struct task init_task(const char*, unsigned short);
struct tasklist init_tasklist(const char*);
bool task_complete(struct task*);
bool tasklist_full(struct tasklist*);
int add_task_to_list(struct tasklist*, struct task);
struct task remove_task_from_list(struct tasklist*, int);
int move_task_up(struct tasklist*, int);
int move_task_down(struct tasklist*, int);

int tasklist_menu(struct tasklist*, char*);

int render_header(WINDOW*, struct tasklist*);
int render_task(WINDOW*, struct task*, int, int);
int render_tasklist(WINDOW*, struct tasklist*, int);
int render_ntprompt(WINDOW*, int, int);
int render_line_wipeout(int);

int line_edit_prompt(char*, int, int);
int new_task_prompt(struct tasklist*, int);
int save_prompt(struct tasklist*, int, char*);
int rename_list_prompt(struct tasklist*, int);
bool confirmation_prompt(int, const char*);

int write_listfile(struct tasklist*, const char*);
int read_listfile(struct tasklist*, const char*);
int export_tasklist_csv(const char*, struct tasklist*);
int export_tasklist_xml(const char*, struct tasklist*);
int export_tasklist_yaml(const char*, struct tasklist*);
int export_tasklist_json(const char*, struct tasklist*);
int export_tasklist_markdown(const char*, struct tasklist*);

#endif