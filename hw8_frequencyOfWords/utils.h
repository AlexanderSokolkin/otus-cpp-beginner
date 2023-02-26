#pragma once

#include <string>
#include <map>
#include <mutex>


using Counter = std::map<std::string, std::size_t>;


void countWords(std::istream& t_stream, Counter& t_counter);
void countWordsThreadSafe(std::istream& t_stream, Counter& t_counter, std::mutex& t_mutex);
void printTopk(std::ostream& t_stream, const Counter& t_counter, const size_t k);
