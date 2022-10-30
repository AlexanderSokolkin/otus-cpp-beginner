#include <iostream>
#include <ctime>
#include <signal.h>
#include <filesystem>

#include "utils.h"
#include "structs.h"
#include "config.h"
#include "constants.h"


static void sigint(int)
{
	exit(0);
}

static void printHelp()
{
	std::cout << "In the \"Guess the number\" game, " \
					"the player is asked to guess a random " \
					"number from " + std::to_string(TOTAL_MIN_RAND_NUMBER) + \
					" to " + std::to_string(DEFAULT_MAX_RAND_NUMBER) + " in the minimum " \
					"number of attempts\n";
	std::cout << "The following parameters are available:\n" \
					"1. --table: Displays a table with records.\n" \
					"2. --max <number>: Еhe player can choose the" \
						"maximum value of a randomly generated number" \
						"number => [" + std::to_string(TOTAL_MIN_RAND_NUMBER) + \
						", " + std::to_string(TOTAL_MAX_RAND_NUMBER) + "].\n" \
					"3. --help: displays the current help.\n" \
					"4. --level <number>: Setting the difficulty level.\n" \
					"\tAvailable levels numbers - 1, 2, 3.\n" \
					"\tlevel 1 - random number from " + std::to_string(TOTAL_MIN_RAND_NUMBER) + \
					" to " + std::to_string(LVL1_MAX_RAND_NUMBER) + ".\n" \
					"\tlevel 2 - random number from " + std::to_string(TOTAL_MIN_RAND_NUMBER) + \
					" to " + std::to_string(LVL2_MAX_RAND_NUMBER) + ".\n" \
					"\tlevel 3 - random number from " + std::to_string(TOTAL_MIN_RAND_NUMBER) + \
					" to " + std::to_string(LVL3_MAX_RAND_NUMBER) + ".\n";
}

static void invalidInputPrint()
{
	std::cout << "Invalid input!\n" << std::endl;
	printHelp();
	exit(0);
}


int main(int argc, char* argv[])
{
	signal(SIGINT, sigint);
	signal(SIGTERM, sigint);

	std::filesystem::path cwd = std::filesystem::path(argv[0]).parent_path();
	cwd.append(CONFIG_FILE_NAME);
	initConfig(cwd.string());

	unsigned int max_value = DEFAULT_MAX_RAND_NUMBER;

	if (argc == 2) {
		std::string parameter(argv[1]);
		if (parameter == "--table") {
			std::string table;
			if (!getTable(table)) {
				std::cout << "The high score table could not be read." << std::endl;
			}
			std::cout << (table.length() ? table : "The high score table is empty!") << std::endl;
			exit(0);
		}
		else if (parameter == "--help") {
			printHelp();
			exit(0);
		}
		else {
			invalidInputPrint();
		}
	}
	else if (argc == 3) {
		std::string parameter(argv[1]);
		if (parameter == "--max") {
			max_value = atoi(argv[2]);
			if (max_value < TOTAL_MIN_RAND_NUMBER || max_value > TOTAL_MAX_RAND_NUMBER) {
				invalidInputPrint();
			}
		}
		else if (parameter == "--level") {
			int lvl = atoi(argv[2]);
			if (lvl < 1 || lvl > 3) {
				invalidInputPrint();
			}
			switch (lvl) {
				case 1:
					max_value = LVL1_MAX_RAND_NUMBER;
					break;
				case 2:
					max_value = LVL2_MAX_RAND_NUMBER;
					break;
				case 3:
					max_value = LVL3_MAX_RAND_NUMBER;
					break;
			}
		}
		else {
			invalidInputPrint();
		}
	}
	else if (argc > 3) {
		std::cout << "Too many parameters have been entered." << std::endl;
		printHelp();
		exit(0);
	}

	// Приветствуем и запрашиваем имя
	std::string name;
	do {
		std::cout << "Hey! Enter your name, please: ";
		std::getline(std::cin, name);
		if (trim(name).empty()) {
			std::cout << "Invalid name entered. Please enter again." << std::endl;
		}
	} while (name.empty());
	

	// Формируем случайное число
	std::srand(std::time(nullptr));
	unsigned int random_number = (std::rand() % (max_value)) + 1;
#ifdef _DEBUG
	std::cout << random_number << std::endl;
#endif
	// Начинаем циклично опрашивать игрока
	int user_number = 0;
	unsigned int attempts = 0;
	do {
		std::cout << "Enter your guess: ";
		std::string guess;
		std::getline(std::cin, guess);
		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (!stringToInt(guess, user_number)) {
			std::cout << "Incorrect input. Please try again" << std::endl;
			continue;
		}
		attempts++;
		if (user_number > random_number) {
			std::cout << "Less than " << user_number << std::endl;
		}
		else if (user_number < random_number) {
			std::cout << "Greater than " << user_number << std::endl;
		}
		else {
			std::cout << "You win! attempts = " << attempts << std::endl;
		}
	} while (user_number != random_number);

	std::string table;
	recordTheScore(UserScore{ name, attempts }, table);

	std::cout << std::endl << "High scores table:\n" << table;
}