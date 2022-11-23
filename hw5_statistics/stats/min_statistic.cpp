#include "min_statistic.h"

#include <limits>


MinStatistic::MinStatistic() :
	m_min(std::numeric_limits<double>::max())
{

}

MinStatistic::~MinStatistic()
{

}

void MinStatistic::update(double t_next)
{
	if (t_next < m_min) {
		m_min = t_next;
	}
}

double MinStatistic::eval() const
{
	return m_min;
}

std::string MinStatistic::name() const
{
	return "min";
}
