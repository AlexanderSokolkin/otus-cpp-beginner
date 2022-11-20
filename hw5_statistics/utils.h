#pragma once

#include "constants.h"

#include <vector>


class StatisticInterface;


ConsoleArgs							parseConsoleArgs(int t_argc, char** t_argv);
void								printHelp();
std::vector<StatisticInterface*>	initStatistics(const ConsoleArgs &t_consoleArgs);
void								readSequence(std::vector<StatisticInterface*> &t_statistics);
