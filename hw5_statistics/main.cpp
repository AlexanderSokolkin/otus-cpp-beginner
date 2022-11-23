#include "constants.h"
#include "utils.h"
#include "stats/statistic_interface.h"

#include <iostream>
#include <exception>


int main(int argc, char** argv)
{
	try {
		ConsoleArgs consoleArgs = parseConsoleArgs(argc, argv);
		if (consoleArgs.statisticItems == NoStatisticItems) {
			exit(0);
		}
		std::vector<StatisticInterface*> statistics = initStatistics(consoleArgs);
		readSequence(statistics);

		for (auto statistic : statistics) {
			std::cout << statistic->name() << " = " << statistic->eval() << std::endl;
		}

		for (auto statistic : statistics) {
			delete statistic;
		}
	}
	catch (const std::exception e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return 0;
}
