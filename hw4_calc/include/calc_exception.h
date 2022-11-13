#pragma once

#include <string>


class CalcException
{
public:
	CalcException(const std::string& t_data);

	std::string error() const;

private:
	std::string	m_errorData;
};

