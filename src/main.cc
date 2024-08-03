#include "frontend.hh"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        // A filename is provided, process the file
        std::string filename = argv[1];
        file_grab(filename);
    } else {
        // No filename provided, start the CLI
        render_cli();
    }
    return EXIT_SUCCESS;
}