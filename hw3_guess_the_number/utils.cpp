#include "utils.h"
#include <string>
#include <algorithm>
#include <regex>


bool stringToInt(const std::string& t_str, int& t_result)
{
	static std::regex r("^\\s*(-*\\d+)\\s*$");
	std::smatch m;
	if (std::regex_search(t_str, m, r)) {
		t_result = stoi(m[0].str());
		return true;
	}
	return false;
}


std::string& trim(std::string& t_str)
{
	bool (*p_notSpace)(char c) = [](char c) { return !std::isspace(c); };
	t_str.erase(t_str.begin(), std::find_if(t_str.begin(), t_str.end(), p_notSpace));
	t_str.erase(std::find_if(t_str.rbegin(), t_str.rend(), p_notSpace).base(), t_str.end());
	return t_str;
}