#include "threadpool.h"
#include "workthread.h"
#include <utility>

ThreadPool::ThreadPool(int size) : m_threads(size)
{
	for (int i = 0; i < size; i++)
		m_threads.emplace_back(new WorkThread(this));
}


ThreadPool::~ThreadPool()
{
	m_threads.clear();
}

bool ThreadPool::getTask(std::function<void()>& task)
{
	bool exp = false;
	while (!m_inQueue.compare_exchange_strong(exp, true))
		exp = false;
	if (m_tasks.empty())
	{
		m_inQueue.store(false, std::memory_order_seq_cst);
		return false;
	}

	task = m_tasks.front();
	m_tasks.pop();
	m_inQueue.store(false, std::memory_order_seq_cst);
	return true;
}

bool& ThreadPool::isPoolFinish()
{
	return m_isFinish;
}
