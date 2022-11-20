#include "mean_statistic.h"


MeanStatistic::MeanStatistic() :
	m_sum(0), m_numCount(0)
{

}

MeanStatistic::~MeanStatistic()
{

}

void MeanStatistic::update(double t_next)
{
	m_sum += t_next;
	++m_numCount;
}

double MeanStatistic::eval() const
{
	return m_sum / m_numCount;
}

std::string MeanStatistic::name() const
{
	return "mean";
}
