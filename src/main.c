#include "../include/types.h"

int main(int argc, char *argv[]) {
    char *file_list[MAX_FILES];
    int file_count;
    if (grab_file(argc, argv, file_list, &file_count) != 0) {
        return 1;
    }
    process_files(file_list, file_count);
    return 0;
}