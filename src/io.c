#include <stdio.h>

#include "defs.h"

int write_listfile(struct tasklist* tl, char* filepath) {
  // open file
  FILE* outfile;
  outfile = fopen(filepath, "w");
  if(!outfile) {
    fprintf(stderr, "Error opening file.\n");
    return 1;
  }
  // write file
  fprintf(outfile, "name: %s\n", tl->name);
  for(int i = 0; i < tl->task_count; i++) {
    fwrite(&tl->tasks[i], sizeof(struct task), 1, outfile);
  }
  // close file
  fclose(outfile);
  return 0;
}

int read_listfile(struct tasklist* tl, char* filepath) {
  // open file
  FILE* infile;
  infile = fopen(filepath, "r");
  if(!infile) {
    fprintf(stderr, "Error opening file.\n");
    return 1;
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
  return 0;
}