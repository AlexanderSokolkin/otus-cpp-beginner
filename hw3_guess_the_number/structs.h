#pragma once
#include <string>

struct UserScore
{
	std::string username;
	unsigned long attempts;

	bool operator<(const UserScore& otherUS) const {
		return	attempts == otherUS.attempts &&
				username < otherUS.username ||
				attempts < otherUS.attempts;
	}
};