#include "threadPool.h"


ThreadPool::ThreadPool(size_t t_numOfThreads) :
	m_numOfThreads{t_numOfThreads},
	m_stopped{false}
{
	for(size_t i = 0; i < m_numOfThreads; ++i) {
		m_threads.emplace_back(
			std::thread{&ThreadPool::workerFunc, this}
		);
	}
}

ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::stop()
{
	if (m_stopped) {
		return;
	}
	m_stopped = true;
	m_condition.notify_all();
	for (auto& thread : m_threads) {
		thread.join();
	}
}

void ThreadPool::workerFunc()
{
	while(!m_stopped) {
		std::unique_lock<std::mutex> lck{m_mutex};
		while(!m_stopped && m_taskPromiseList.empty()) {
			m_condition.wait(lck);
		}
		if (m_stopped) break;
		if (m_taskPromiseList.empty()) continue;

		auto taskPromise = std::move(m_taskPromiseList.front());
		m_taskPromiseList.pop_front();

		lck.unlock();

		auto promise = std::move(taskPromise.second);
		auto task = std::move(taskPromise.first);

		try {
			task();
			promise.set_value();
		}
		catch(...) {
			promise.set_exception(std::current_exception());
		}
	}
}
