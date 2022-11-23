#include "pct_statistic.h"

#include <limits>
#include <string>


PctStatistic::PctStatistic(int t_percentile) :
	m_max(std::numeric_limits<double>::min()), m_percentile(t_percentile)
{

}

PctStatistic::~PctStatistic()
{

}

void PctStatistic::update(double t_next)
{
	if (t_next > m_max) {
		m_max = t_next;
	}
}

double PctStatistic::eval() const
{
	return m_max * m_percentile / 100;
}

std::string PctStatistic::name() const
{
	std::string name = "pct";
	name += std::to_string(m_percentile);
	return name;
}
