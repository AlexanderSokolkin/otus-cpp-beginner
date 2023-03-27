#include "game.h"
#include "constants.h"
#include "utils.h"

#include <stdexcept>
#include <iostream>



int main(int argc, char* argv[])
{
	DifficultyLevel lvl = parseArgs(argc, argv);
	try {
		Game game(lvl);

		return game.exec();
	}
	catch(const std::runtime_error& e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

}
