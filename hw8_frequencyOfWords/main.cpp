#include "constants.h"
#include "utils.h"
#include "threadPool.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <filesystem>
#include <stdexcept>

using Counter = std::map<std::string, std::size_t>;


void oneThreadCountWords(int argc, char* argv[], Counter& t_counter)
{
	for (int i = 1; i < argc; ++i) {
		std::ifstream input{argv[i]};
		if (!input.is_open()) {
			std::string err = "Failed to open file ";
			err.append(argv[i]);
			throw std::runtime_error{err};
		}
		countWords(input, t_counter);
	}
}

void countWordsWithSeparateCounters(int argc, char* argv[], Counter& t_counter)
{
	std::vector<std::ifstream> files;
	std::vector<std::thread> threads;
	std::vector<Counter> counters;
	files.reserve(static_cast<size_t>(argc - 1));
	threads.reserve(static_cast<size_t>(argc - 1));
	counters.resize(static_cast<size_t>(argc - 1));
	for (int i = 1; i < argc; ++i) {
		files.emplace_back(
			std::ifstream{argv[i]}
		);
		if (!files.back().is_open()) {
			for (auto& thread : threads) {
				thread.detach();
			}
			std::string err = "Failed to open file ";
			err.append(argv[i]);
			throw std::runtime_error{err};
		}
		threads.emplace_back(
			countWords,
				std::ref(files.back()),
				std::ref(counters[i - 1])
		);
	}
	for (int i = 0; i < argc - 1; ++i) {
		threads[i].join();
		t_counter.insert(counters[i].begin(), counters[i].end());
	}
}

void multithreadCount(int argc, char* argv[], Counter& t_counter)
{
	std::mutex mutex;
	std::vector<std::ifstream> files;
	std::vector<std::thread> threads;
	files.reserve(static_cast<size_t>(argc - 1));
	threads.reserve(static_cast<size_t>(argc - 1));
	for (int i = 1; i < argc; ++i) {
		files.emplace_back(
			std::ifstream{argv[i]}
		);
		if (!files.back().is_open()) {
			for (auto& thread : threads) {
				thread.detach();
			}
			std::string err = "Failed to open file ";
			err.append(argv[i]);
			throw std::runtime_error{err};
		}
		threads.emplace_back(
			std::thread{countWordsThreadSafe,
							std::ref(files.back()),
							std::ref(t_counter),
							std::ref(mutex)}
		);
	}
	for (auto& thread : threads) {
		thread.join();
	}
}

void countWordsWithThreadPool(int argc, char* argv[], Counter& t_counter)
{
	ThreadPool pool{std::thread::hardware_concurrency()};
	std::vector<Counter> counters{static_cast<size_t>(argc - 1)};

	std::vector<std::function<void()>> tasks;
	tasks.reserve(argc - 1);
	for (int i = 1; i < argc; ++i) {
		tasks.emplace_back(
			[argv, i, &counters]() {
				std::ifstream file{argv[i]};
				if (!file.is_open()) {
					std::string err = "Failed to open file ";
					err.append(argv[i]);
					throw std::runtime_error{err};
				}
				countWords(file, counters[i - 1]);
			}
		);
	}

	std::vector<std::future<void>> futures;
	futures.reserve(argc - 1);
	for (int i = 0; i < tasks.size(); ++i) {
		futures.emplace_back(
			pool.pushTask(tasks[i])
		);
	}

	for (int i = 0; i < argc - 1; ++i) {
		futures[i].wait();
		t_counter.insert(counters[i].begin(), counters[i].end());
	}
}




int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: frequencyOfWords [FILES...]\n";
		return EXIT_FAILURE;
	}
	bool incorrectFilesPath = false;
	for (int i = 1; i < argc; ++i) {
		if (!std::filesystem::exists(argv[i])) {
			incorrectFilesPath = true;
			std::cerr << "File " << argv[i] << " is not exist\n";
		}
	}
	if (incorrectFilesPath) return EXIT_FAILURE;


	auto start = std::chrono::high_resolution_clock::now();

	Counter freqDict;

	try {
		// oneThreadCountWords(argc, argv, freqDict);
		// countWordsWithSeparateCounters(argc, argv, freqDict);
		multithreadCount(argc, argv, freqDict);
		// countWordsWithThreadPool(argc, argv, freqDict);
	}
	catch (std::runtime_error e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	printTopk(std::cout, freqDict, TOPK);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Elapsed time is " << elapsed_us.count() << " us\n";
}
