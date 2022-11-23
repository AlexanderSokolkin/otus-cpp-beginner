#pragma once

#include "statistic_interface.h"


class PctStatistic : public StatisticInterface
{
public:
	PctStatistic(int t_percentile);
	~PctStatistic();

	void		update(double t_next) override;
	double		eval() const override;
	std::string	name() const override;

private:
	double	m_max;
	int		m_percentile;
};
