#ifndef TYPES_H

#define TYPES_H
#define MAX_FILES 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"



// externalize file inputs
extern char *file_list[MAX_FILES];
extern int file_count;

int valid_file_extension(const char *filename);
int grab_file(int argc, char *argv[], char *file_list[], int *file_count);
void process_files(char *file_list[], int file_count);


#endif // TYPES_H