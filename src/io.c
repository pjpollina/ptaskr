#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "defs.h"

static int path_expansion(char* output, char* filepath) {
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

static int get_parent_directory(char* output, char* filepath) {
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

static int make_parent_directory(char* filepath) {
  char pd[4096];
  get_parent_directory(pd, filepath);
  return mkdir(pd, 0777);
}

int write_listfile(struct tasklist* tl, char* filepath) {
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

int read_listfile(struct tasklist* tl, char* filepath) {
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