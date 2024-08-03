#include "frontend.hh"

bool exit_requested = false;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        if (exit_requested) {
            std::cout << "Goodbye from skid!" << std::endl;
            exit(0);
        } else {
            exit_requested = true;
            exit_cli();
        }
    } else if (signal == SIGTERM) {
        std::cout << "Goodbye from skid!" << std::endl;
        exit(0);
    }
}

void render_cli() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGQUIT, signal_handler);

    std::cout << "Welcome to skid" << version() << std::endl;
    std::cout << "Type 'help' for a list of commands" << std::endl;
    std::string input;
    while (true) {
        std::cout << ">" << std::flush;
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        exit_requested = false;
    }
}

void exit_cli() {
    std::cout << "(To exit, press Ctrl+C again or Ctrl+D or type 'exit')" << std::endl;
}

void file_grab(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Process each line of the file
        std::cout << line << std::endl;
    }

    file.close();
}

std::string version() {
    std::ifstream meta_file("../meta.data");
    if (!meta_file.is_open()) {
        std::cerr << "Error: Could not open meta.data file" << std::endl;
        return "";
    }

    std::string line;
    while (std::getline(meta_file, line)) {
        if (line.find("VERSION:") != std::string::npos) {
            std::string version_number = line.substr(line.find(":") + 1);
            meta_file.close();
            return version_number;
        }
    }

    meta_file.close();
    
    return "";
}