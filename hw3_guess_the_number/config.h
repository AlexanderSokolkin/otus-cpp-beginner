#pragma once
#include <string>
#include "structs.h"

// namespace std {
// class string;
// }
// struct UserScore;


void initConfig(const std::string& t_configPath);
bool getTable(std::string& t_table);
bool recordTheScore(const UserScore& t_us, std::string& t_resultTable);
