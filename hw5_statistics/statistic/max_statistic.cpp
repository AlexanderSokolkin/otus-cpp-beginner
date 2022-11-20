#include "max_statistic.h"

#include <limits>


MaxStatistic::MaxStatistic() :
	m_max(std::numeric_limits<double>::min())
{

}

MaxStatistic::~MaxStatistic()
{

}

void MaxStatistic::update(double t_next)
{
	if (t_next > m_max) {
		m_max = t_next;
	}
}

double MaxStatistic::eval() const
{
	return m_max;
}

std::string MaxStatistic::name() const
{
	return "max";
}
