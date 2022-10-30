#include <direct.h>
#include <fstream>
#include <regex>
#include <list>

#include "structs.h"
#include "utils.h"


static std::string CONFIG_PATH;
static unsigned int MAX_ROW_COUNT = 5;
static std::regex reg("^(.+)\\s+(\\d+)$");



static bool parseRow(const std::string& t_row, std::string& t_username, unsigned long& t_attempts)
{
	std::smatch m;
	if (std::regex_match(t_row, m, reg)) {
		t_username = m[1].str();
		t_attempts = std::stoul(m[2].str());
		return true;
	}
	return false;
}

static bool isValidRow(const std::string& t_row)
{
	return std::regex_match(t_row, reg);
}

static bool readConfig(std::list<UserScore>& t_table)
{
	t_table.clear();
	if (CONFIG_PATH.empty()) {
		return false;
	}

	std::ifstream i(CONFIG_PATH);

	if (!i.is_open()) {
		return true;
	}

	while (!i.eof()) {
		std::string row;
		std::getline(i, row);
		UserScore us;
		if (parseRow(trim(row), us.username, us.attempts)) {
			t_table.push_back(us);
		}
	}
	i.close();
	return true;
}

static bool writeConfig(const std::list<UserScore>& t_table, std::string& t_tableStr)
{
	if (CONFIG_PATH.empty()) {
		return false;
	}

	std::ofstream o(CONFIG_PATH);

	if (!o.is_open()) {
		return false;
	}

	t_tableStr.clear();
	for (const auto& us : t_table) {
		t_tableStr += us.username + " " + std::to_string(us.attempts) + "\n";
	}
	o << t_tableStr;
	o.close();
}

static std::string tableToString(const std::list<UserScore>& t_table)
{
	std::string strTable;
	for (const auto& us : t_table) {
		strTable += us.username + " - " + std::to_string(us.attempts) + "\n";
	}
	return strTable;
}

static void deleteRecentEntries(std::list<UserScore>& t_table)
{
	while (t_table.size() > MAX_ROW_COUNT) {
		t_table.pop_back();
	}
}




void initConfig(const std::string& t_configPath)
{
	CONFIG_PATH = t_configPath;

}

bool getTable(std::string& t_table)
{
	t_table.clear();
	std::list<UserScore> table;
	if (!readConfig(table)) {
		return false;
	}

	t_table = std::move(tableToString(table));
	return true;
}

bool recordTheScore(const UserScore& t_us, std::string& t_resulTable)
{
	t_resulTable.clear();
	std::list<UserScore> table;
	if (!readConfig(table)) {
		return false;
	}

	deleteRecentEntries(table);

	auto it = std::find_if(table.begin(),
						   table.end(),
						   [&](const UserScore& us) { return us.username == t_us.username; });
	if (it != table.end()) {
		(*it).attempts = t_us.attempts;
		table.sort();
	}
	else {
		it = std::find_if(table.begin(),
						  table.end(),
						  [&](const UserScore& us) { return t_us < us; });
		table.insert(it, t_us);
		deleteRecentEntries(table);
	}

	return writeConfig(table, t_resulTable);
}