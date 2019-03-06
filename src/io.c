#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "defs.h"

static int path_expansion(char* output, const char* filepath) {
  if(filepath[0] == '~' && filepath[1] == '/') {
    char buffer[4096];
    strcpy(buffer, getenv("HOME"));
    strcat(buffer, &filepath[1]);
    strcpy(output, buffer);
    return EXIT_SUCCESS;
  }
  strcpy(output, filepath);
  return EXIT_SUCCESS;
}

static int get_parent_directory(char* output, const char* filepath) {
  char buffer[4096];
  strcpy(buffer, filepath);
  int i = strlen(buffer) - 1;
  while(i > -1) {
    if(buffer[i] == '/')
      break;
    buffer[i] = '\0';
    i--;
  }
  strcpy(output, buffer);
  return EXIT_SUCCESS;
}

static int make_parent_directory(const char* filepath) {
  char pd[4096];
  get_parent_directory(pd, filepath);
  return mkdir(pd, 0777);
}

int write_listfile(struct tasklist* tl, const char* filepath) {
  // get full filepath
  char full_filepath[4096];
  path_expansion(full_filepath, filepath);

  // make directory if not exist
  make_parent_directory(full_filepath);

  // open file
  FILE* outfile;
  outfile = fopen(full_filepath, "w");
  if(!outfile) {
    fprintf(stderr, "Error opening file.\n");
    return EXIT_FAILURE;
  }

  // write file
  fprintf(outfile, "name: %s\n", tl->name);
  for(int i = 0; i < tl->task_count; i++) {
    fwrite(&tl->tasks[i], sizeof(struct task), 1, outfile);
  }

  // close file
  fclose(outfile);
  return EXIT_SUCCESS;
}

int read_listfile(struct tasklist* tl, const char* filepath) {
  // get full filepath
  char full_filepath[4096];
  path_expansion(full_filepath, filepath);

  // make directory if not exist
  make_parent_directory(full_filepath);

  // open file
  FILE* infile;
  infile = fopen(full_filepath, "r");
  if(!infile) {
    fprintf(stderr, "Error opening file.\n");
    return EXIT_FAILURE;
  }

  // read file
  fscanf(infile, "name: %[^\n]", tl->name);
  fseek(infile, 1, SEEK_CUR);
  struct task temp;
  while(fread(&temp, sizeof(struct task), 1, infile)) {
    add_task_to_list(tl, temp);
  }
  
  // close file
  fclose(infile);
  return EXIT_SUCCESS;
}

// XML Exporting
static int task_to_xml(char* output, struct task t) {
  char buffer[1024];
  sprintf(buffer, "  <ptaskr-task>\n    <desc>%s</desc>\n    <goal>%u</goal>\n    <reached>%u</reached>\n  </ptaskr-task>", t.desc, t.goal, t.reached);
  strcpy(output, buffer);
  return EXIT_SUCCESS;
}

int export_tasklist_xml(const char* filepath, struct tasklist* tl) {
  FILE* outfile;
  outfile = fopen(filepath, "w");
  fprintf(outfile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<ptaskr-list>\n");
  fprintf(outfile, "  <name>%s</name>\n", tl->name);
  for(int i = 0; i < tl->task_count; i++) {
    char buffer[1024];
    task_to_xml(buffer, tl->tasks[i]);
    fprintf(outfile, "%s\n", buffer);
  }
  fprintf(outfile, "</ptaskr-list>");
  fclose(outfile);
  return EXIT_SUCCESS;
}

// YAML Exporting
static int task_to_yaml(char* output, struct task t) {
  char buffer[1024];
  sprintf(buffer, "- !ptaskr-task\n  desc: \"%s\"\n  goal: %u\n  reached: %u", t.desc, t.goal, t.reached);
  strcpy(output, buffer);
  return EXIT_SUCCESS;
}

int export_tasklist_yaml(const char* filepath, struct tasklist* tl) {
  FILE* outfile;
  outfile = fopen(filepath, "w");
  fprintf(outfile, "---\nname: %s\n", tl->name);
  for(int i = 0; i < tl->task_count; i++) {
    char buffer[1024];
    task_to_yaml(buffer, tl->tasks[i]);
    fprintf(outfile, "%s\n", buffer);
  }
  fclose(outfile);
  return EXIT_SUCCESS;
}

// JSON Exporting
static int task_to_json(char* output, struct task t) {
  char buffer[1024];
  sprintf(buffer, "{ \"desc\": \"%s\", \"goal\": %u, \"reached\": %u }", t.desc, t.goal, t.reached);
  strcpy(output, buffer);
  return EXIT_SUCCESS;
}

int export_tasklist_json(const char* filepath, struct tasklist* tl) {
  FILE* outfile;
  outfile = fopen(filepath, "w");
  fprintf(outfile, "{\n  \"name\": \"%s\",\n  \"tasks\": [\n", tl->name);
  for(int i = 0; i < tl->task_count; i++) {
    char buffer[1024];
    task_to_json(buffer, tl->tasks[i]);
    fprintf(outfile, "    %s\n", buffer);
  }
  fprintf(outfile, "  ]\n}\n");
  fclose(outfile);
  return EXIT_SUCCESS;
}

// Markdown Exporting
static int task_to_markdown(char* output, struct task t) {
  char buffer[1024];
  if(t.goal == 1) {
    sprintf(buffer, " - [%c] %s", (task_complete(&t)) ? 'x' : ' ', t.desc);
  } else {
    sprintf(buffer, " - [%d/%d] %s", t.reached, t.goal, t.desc);
  }
  strcpy(output, buffer);
  return EXIT_SUCCESS;
}

int export_tasklist_markdown(const char* filepath, struct tasklist* tl) {
  FILE* outfile;
  outfile = fopen(filepath, "w");
  fprintf(outfile, "## %s\n\n", tl->name);
  for(int i = 0; i < tl->task_count; i++) {
    char buffer[1024];
    task_to_markdown(buffer, tl->tasks[i]);
    fprintf(outfile, "%s\n", buffer);
  }
  fclose(outfile);
  return EXIT_SUCCESS;
}