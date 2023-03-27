#include "utils.h"

#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>


static std::string toLower(const std::string_view& t_str)
{
	std::string lowerStr;
	std::transform(std::cbegin(t_str),
				   std::cend(t_str),
				   std::back_inserter(lowerStr),
				   [](unsigned char ch) {
						return static_cast<char>(std::tolower(ch));
				   });

	return lowerStr;
}

DifficultyLevel parseArgs(int argc, char* argv[])
{
	// дефолтная сложность - новичок
	if (argc < 2) {
		return DifficultyLevel::Beginner;
	}

	if (argc > 2) {
		printHelp();
		return DifficultyLevel::Beginner;
	}

	std::string p = toLower(std::string_view(argv[1]));
	if (p == "1" || p == "beginner") {
		return DifficultyLevel::Beginner;
	}
	else if (p == "2" || p == "amateur") {
		return DifficultyLevel::Amateur;
	}
	else if (p == "3" || p == "professional") {
		return DifficultyLevel::Professional;
	}
	else {
		printHelp();
		return DifficultyLevel::Beginner;
	}
}

void printHelp()
{
	std::cout << "Classic game - Saper!\n"
				 "to select the difficulty, you need to pass the following parameter:\n"
				 "\t1 or beginner - the easiest difficulty level;\n"
				 "\t2 or amateur - average difficulty level;\n"
				 "\t3 or professional - the most difficult level of difficulty.\n"
				 "\n"
				 "Note: by default, the beginner level is started.";
}
