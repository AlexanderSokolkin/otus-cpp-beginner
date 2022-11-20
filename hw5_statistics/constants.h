#pragma once

#include <list>


enum StatisticItem : unsigned short {
	NoStatisticItems = 0,

	Min = 1,
	Max = 2,
	Mean = 4,
	Std = 8,
	Pct = 16,

	AllStatisticItems = Min		|
						Max		|
						Mean	|
						Std		|
						Pct
};

struct ConsoleArgs
{
	unsigned short statisticItems = NoStatisticItems;
	std::list<int> percentiles;
};
