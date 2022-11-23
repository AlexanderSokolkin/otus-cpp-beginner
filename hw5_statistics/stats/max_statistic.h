#pragma once

#include "statistic_interface.h"


class MaxStatistic : public StatisticInterface
{
public:
	MaxStatistic();
	~MaxStatistic();

	void		update(double t_next) override;
	double		eval() const override;
	std::string	name() const override;

private:
	double	m_max;
};
