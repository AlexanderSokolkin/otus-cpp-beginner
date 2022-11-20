#include "utils.h"
#include "statistic/max_statistic.h"
#include "statistic/mean_statistic.h"
#include "statistic/min_statistic.h"
#include "statistic/pct_statistic.h"
#include "statistic/std_statistic.h"

#include <string>
#include <exception>
#include <iostream>
#include <regex>
#include <algorithm>
#include <list>


static void throwErrorConsoleArgument(const std::string &t_arg)
{
	std::string err = "Unknown argument: ";
	err += t_arg;
	throw std::exception(err.c_str());
}



ConsoleArgs parseConsoleArgs(int t_argc, char** t_argv)
{
	if (t_argc == 1) {
		return ConsoleArgs{AllStatisticItems,
						   {90, 95}};
	}

	if (t_argc == 2 && strcmp(t_argv[1], "--help") == 0) {
		printHelp();
		return ConsoleArgs{};
	}

	std::regex pctReg("^--pct([0-9]{1,3})$");
	std::smatch m;
	ConsoleArgs consoleArgs{};
	for (int i = 1; i < t_argc; ++i) {
		std::string arg{t_argv[i]};
		if (arg == "--min") {
			consoleArgs.statisticItems |= Min;
		}
		else if (arg == "--max") {
			consoleArgs.statisticItems |= Max;
		}
		else if (arg == "--mean") {
			consoleArgs.statisticItems |= Mean;
		}
		else if (arg == "--std") {
			consoleArgs.statisticItems |= Std;
		}
		else if (std::regex_search(arg, m, pctReg)) {
			consoleArgs.statisticItems |= Pct;
			int percentile = stoi(m[1].str());
			if (percentile < 0 || percentile > 100) {
				throwErrorConsoleArgument(arg);
			}
			auto it = std::find(consoleArgs.percentiles.begin(), consoleArgs.percentiles.end(), percentile);
			if (it != consoleArgs.percentiles.end()) {
				continue;
			}
			consoleArgs.percentiles.insert(std::find_if(consoleArgs.percentiles.begin(),
														consoleArgs.percentiles.end(),
														[&](int val){ return val > percentile; }),
										   percentile);
		}
		else {
			throwErrorConsoleArgument(arg);
		}
	}

	return consoleArgs;
}

void printHelp()
{
	std::cout << "The \"statistics\" application accepts a sequence " \
				 "of real numbers as input and returns the following " \
				 "statistics of the entered sequence:\n" \
				 "\t1. min - minimum sequence number;\n" \
				 "\t2. max - maximum sequence number;\n" \
				 "\t3. mean - the arithmetic mean of the sequence numbers;\n" \
				 "\t4. std - root-mean-square deviation of the sequence numbers;\n" \
				 "\t5. pct90 - the 90th percentile of the sequence numbers;\n" \
				 "\t6. pct95 - the 95th percentile of the sequence numbers.\n\n";
	std::cout << "The following parameters are available:\n" \
				 "1) --help - displays the current help;\n" \
				 "2) \"--min\", \"--max\", \"--mean\", \"--std\", \"--pct<n> where <n> " \
				 "can take values from 0 to 100\"  - enable corresponding statistic " \
				 "(*NOTE: can be used together)\n";
}

static size_t getStatisticsCount(unsigned short t_statisticsItems)
{
	size_t size = 0;
	while (t_statisticsItems != NoStatisticItems) {
		if (t_statisticsItems & 1) {
			++size;
		}
		t_statisticsItems >>= 1;
	}

	return size;
}

std::vector<StatisticInterface*> initStatistics(const ConsoleArgs &t_consoleArgs)
{
	unsigned short statisticItems = t_consoleArgs.statisticItems;
	if (statisticItems == NoStatisticItems) {
		return std::vector<StatisticInterface*>{};
	}

	size_t size = getStatisticsCount(statisticItems);
	if (statisticItems & Pct) {
		size += t_consoleArgs.percentiles.size() - 1;
	}

	std::vector<StatisticInterface*> statistics{size};
	unsigned short item = 0;
	int index = 0;
	while (statisticItems != NoStatisticItems) {
		StatisticItem statisticItem = static_cast<StatisticItem>(statisticItems & (1 << item));
		switch (statisticItem) {
			case NoStatisticItems:
				break;
			case Min:
				statistics[index++] = new MinStatistic;
				break;
			case Max:
				statistics[index++] = new MaxStatistic;
				break;
			case Mean:
				statistics[index++] = new MeanStatistic;
				break;
			case Std:
				statistics[index++] = new StdStatistic;
				break;
			case Pct:
				for (int percentile : t_consoleArgs.percentiles) {
					statistics[index++] = new PctStatistic(percentile);
				}
				break;
			default:
				break;
		}
		statisticItems &= ~(1 << item);
		++item;
	}
	return statistics;
}

void readSequence(std::vector<StatisticInterface*> &t_statistics)
{
	double num = 0;
	while (std::cin >> num) {
		for (auto statistic : t_statistics) {
			statistic->update(num);
		}
	}

	if (!std::cin.eof() && !std::cin.good()) {
		throw std::exception("Invalid input data");
	}
}
