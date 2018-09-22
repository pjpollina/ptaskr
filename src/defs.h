#ifndef _TASKLIST_DEFS_H_
#define _TASKLIST_DEFS_H_

#include <ncurses.h>
#include <stdbool.h>

#define DESC_MAX 80
#define TASK_LIST_MAX 256

#define COMPLETE_COLOR 1
#define MENU_NEW_TASK "[New Task]"

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

struct task init_task(char*, unsigned short);
struct tasklist init_tasklist(char*);
bool task_complete(struct task);
bool tasklist_full(struct tasklist);
int add_task_to_list(struct tasklist*, struct task);
struct task remove_task_from_list(struct tasklist*, int);

int tasklist_menu(struct tasklist*);

int render_header(WINDOW*, struct tasklist);
int render_task(WINDOW*, struct task, int, int);
int render_tasklist(WINDOW*, struct tasklist, int);
int render_ntprompt(WINDOW*, int, int);

int line_editor(char[], int, int);
int newtask_menu(struct tasklist*, int);
bool confirmation_prompt(int);

int write_listfile(struct tasklist*, char*);
int read_listfile(struct tasklist*, char*);

#endif