#pragma once

#include "statistic_interface.h"


class MinStatistic : public StatisticInterface
{
public:
	MinStatistic();
	~MinStatistic();

	void		update(double t_next) override;
	double		eval() const override;
	std::string	name() const override;

private:
	double m_min;
};
