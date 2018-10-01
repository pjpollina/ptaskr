#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "defs.h"

struct task init_task(char* desc, unsigned short goal) {
  struct task t = { .goal = goal, .reached = 0 };
  strcpy(t.desc, desc);
  return t;
}

struct tasklist init_tasklist(char* name) {
  struct tasklist tl = { .task_count = 0 };
  strcpy(tl.name, name);
  return tl;
}

bool task_complete(struct task t) {
  return t.reached == t.goal;
}

bool tasklist_full(struct tasklist tl) {
  return tl.task_count == TASK_LIST_MAX;
}

int add_task_to_list(struct tasklist* tl, struct task t) {
  tl->tasks[tl->task_count] = t;
  tl->task_count++;
  return EXIT_SUCCESS;
}

struct task remove_task_from_list(struct tasklist* tl, int index) {
  struct task removed = tl->tasks[index];
  for(int i = index; i < tl->task_count; i++) {
    tl->tasks[i] = tl->tasks[i + 1];
  }
  tl->task_count--;
  return removed;
}

int move_task_up(struct tasklist* tl, int pos) {
  struct task t = tl->tasks[pos-1];
  tl->tasks[pos-1] = tl->tasks[pos];
  tl->tasks[pos] = t;
  return EXIT_SUCCESS;
}

int move_task_down(struct tasklist* tl, int pos) {
  struct task t = tl->tasks[pos];
  tl->tasks[pos] = tl->tasks[pos+1];
  tl->tasks[pos+1] = t;
  return EXIT_SUCCESS;
}