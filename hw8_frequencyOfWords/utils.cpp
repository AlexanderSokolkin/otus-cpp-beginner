#include "utils.h"

#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <mutex>


static std::string toLower(const std::string& t_str)
{
	std::string lowerStr;
	std::transform(std::cbegin(t_str),
				   std::cend(t_str),
				   std::back_inserter(lowerStr),
				   [](unsigned char ch) {
						return static_cast<char>(std::tolower(ch));
				   });

	return lowerStr;
}


void countWords(std::istream& t_stream, Counter& t_counter)
{
	std::for_each(std::istream_iterator<std::string>(t_stream),
				  std::istream_iterator<std::string>(),
				  [&t_counter](const std::string &s) {
						++t_counter[toLower(s)];
				  });
}

void countWordsThreadSafe(std::istream& t_stream, Counter& t_counter, std::mutex& t_mutex)
{
	std::for_each(std::istream_iterator<std::string>(t_stream),
				std::istream_iterator<std::string>(),
				[&t_counter, &t_mutex](const std::string &s) {
					t_mutex.lock();
					++t_counter[toLower(s)];
					t_mutex.unlock();
				});
}

void printTopk(std::ostream& t_stream, const Counter& t_counter, const size_t k)
{
	std::vector<Counter::const_iterator> words;
	words.reserve(t_counter.size());
	for (auto it = std::cbegin(t_counter); it != std::cend(t_counter); ++it) {
		words.push_back(it);
	}

	std::partial_sort(
		std::begin(words),
		std::begin(words) + k,
		std::end(words),
		[](auto& lhs, auto& rhs) {
			return lhs->second > rhs->second;
		});

	std::for_each(
		std::begin(words),
		std::begin(words) + k,
		[&t_stream](const Counter::const_iterator& pair) {
			t_stream << std::setw(4) << pair->second << " " << pair->first << '\n';
		});
}
