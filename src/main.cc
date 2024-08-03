#include "frontend/frontend.hh"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        // A filename is provided, process the file
        std::string filename = argv[1];
        std::string extension = filename.substr(filename.find_last_of(".") + 1);
        if (extension == "kid") {
            file_grab(filename);
        } else {
            std::cout << "\033[1;31mError: The file given is not a KiddieScript compatible source file.\033[0m" << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        // No filename provided, start the CLI
        render_cli();
    }
    return EXIT_SUCCESS;
}