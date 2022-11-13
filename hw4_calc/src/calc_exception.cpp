#include "calc_exception.h"


CalcException::CalcException(const std::string& t_data) :
	m_errorData(t_data)
{

}

std::string CalcException::error() const
{
	return m_errorData;
}