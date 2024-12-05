#include "fifth.hh"

int main(int argc, char *argv[]) {
	// if nothing is passed as an argument complain
	if (argc == 1) {
		help();
		return EXIT_FAILURE;
	}
	// if the first argument is not a forth file complain
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			std::filesystem::path file(argv[1]);
			if (is_forth_file(file)) {
				std::cout << file << " is valid";
			} else {
				std::cout << file << " is not valid";
			}
		}
	}

	return 0;
}
