
#include "../include/types.h"

int 
valid_file_extension(const char *filename)
{
const char *ext = strrchr(filename, '.');
return (ext && strcmp(ext, ".kid") == 0);
}

int
grab_file(int argc, char *argv[], char *file_list[], int *file_count)
{
if (argc < 2)
{
fprintf(stderr, "Usage: %s <file1.kid> <file2.kid> ... <fileN.kid>\n", argv[0]);
return 1;
}    

*file_count = 0;
for (int i = 1; i < argc; i++)
{
if (valid_file_extension(argv[i]))
{
file_list[*file_count] = argv[i];
(*file_count)++;
}
else
{
fprintf(stderr, "Invalid file extension: %s\n", argv[i]);
}
}
return 0;
}

void
process_files(char *file_list[], int file_count)
{
for (int i = 0; i < file_count; i++)
{
printf("Processing file: %s\n", file_list[i]);
// hand off to another function to process the file to an IR
}
}