#include "frontend.hh"

void render_cli(std::string) {
    std::cout << "Rendering CLI" << std::endl;
}
#include <iostream>
#include <string>

void render_cli() {
    std::cout << "Welcome to Skid v" << version() << std::endl;
    std::cout << "Type 'help' for a list of commands" << std::endl;
    std::string input;

}

void exit_cli() {
    
}

std::string version() {
    
    std::ifstream file("meta.data");
    std::string line;
    std::string current_version;

    while (std::getline(file, line)) {
        if (line.find("VERSION:") != std::string::npos) {
            current_version = line.substr(line.find(":") + 1);
            break;
        }
    }

    return current_version;
}
