
#include "../include/types.h"
#include "../include/lexer.h"

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
return EXIT_FAILURE;
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
return EXIT_SUCCESS;
}

void
process_files(char *file_list[], int file_count)
{
for (int i = 0; i < file_count; i++)
{
printf("Lexing file: %s\n", file_list[i]);
process_file(file_list[i]);
}
}