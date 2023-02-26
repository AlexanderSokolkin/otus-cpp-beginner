#pragma once

#include <future>
#include <thread>
#include <list>
#include <vector>
#include <atomic>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <utility>



class ThreadPool
{
public:
	explicit ThreadPool(size_t t_numOfThreads);
	~ThreadPool();

	template<typename Task>
	std::future<void> pushTask(Task&& t_task);
	void stop();

private:
	void workerFunc();


	std::vector<std::thread> m_threads;
	std::list<std::pair<std::function<void()>, std::promise<void>>> m_taskPromiseList;
	std::condition_variable m_condition;
	std::mutex m_mutex;
	size_t m_numOfThreads;
	std::atomic<bool> m_stopped;
};


template<typename Task>
std::future<void> ThreadPool::pushTask(Task&& t_task)
{
	std::pair<std::function<void()>, std::promise<void>> taskPromise 
		= std::make_pair(
			std::forward<Task>(t_task),
			std::promise<void>{}
		);
	std::future<void> resultFuture = taskPromise.second.get_future();

	std::lock_guard<std::mutex> lck{m_mutex};
	m_taskPromiseList.emplace_back(std::move(taskPromise));
	m_condition.notify_one();

	return resultFuture;
}