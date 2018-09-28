#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "defs.h"

static char* path_expansion(char* filepath) {
  if(filepath[0] == '~' && filepath[1] == '/') {
    char* buffer = malloc(4096);
    strcpy(buffer, getenv("HOME"));
    strcat(buffer, &filepath[1]);
    return buffer;
  }
  return filepath;
}

static char* get_directory(char* filepath) {
  char* buffer = malloc(4096);
  strcpy(buffer, filepath);
  int i = strlen(buffer) - 1;
  while(i > -1) {
    if(buffer[i] == '/')
      break;
    buffer[i] = '\0';
    i--;
  }
  return buffer;
}

int write_listfile(struct tasklist* tl, char* filepath) {
  // make directory if not exist
  mkdir(get_directory(path_expansion(filepath)), 0777);
  // open file
  FILE* outfile;
  outfile = fopen(path_expansion(filepath), "w");
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

int read_listfile(struct tasklist* tl, char* filepath) {
  // make directory if not exist
  mkdir(get_directory(path_expansion(filepath)), 0777);
  // open file
  FILE* infile;
  infile = fopen(path_expansion(filepath), "r");
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