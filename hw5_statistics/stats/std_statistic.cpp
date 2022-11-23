#include "std_statistic.h"

#include <cmath>


StdStatistic::StdStatistic() :
	m_sum(0), m_nums({})
{

}

StdStatistic::~StdStatistic()
{

}

void StdStatistic::update(double t_next)
{
	m_sum += t_next;
	m_nums.push_back(t_next);
}

double StdStatistic::eval() const
{
	size_t numsCount = m_nums.size();
	double mean = m_sum / numsCount;
	double squareDeviation = 0;
	for (auto num : m_nums) {
		squareDeviation += std::pow(num - mean, 2);
	}
	return std::sqrt(squareDeviation / numsCount);
}

std::string StdStatistic::name() const
{
	return "std";
}
