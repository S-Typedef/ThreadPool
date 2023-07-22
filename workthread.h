#ifndef __WORKTHREAD_H_
#define __WORKTHREAD_H_


#include <thread>
#include <atomic>
#include "task.h"
#include "threadpool.h"

class WorkThread
{
private:
	enum class WorkStatus
	{
		Wait,
		Suspend,
		Running,
		Exit
	};

	WorkStatus m_status = WorkStatus::Wait;
	std::thread m_thread;
	ThreadPool* m_pool;

public:
	WorkThread(ThreadPool* pool);
	~WorkThread();
	WorkThread(const WorkThread&) = delete;
	WorkThread(WorkThread&&) = delete;
	WorkThread& operator=(const WorkThread&) = delete;
	WorkThread& operator=(WorkThread&&) = delete;

public:

private:
	void changeStatus(WorkStatus nStatus)
	{
		m_status = nStatus;
	}
};

WorkThread::WorkThread(ThreadPool* pool) : m_pool(pool)
{
	m_thread = std::thread([&]()
		{
			std::function<void()> task;
			changeStatus(WorkStatus::Wait);
			while (!m_pool->isPoolFinish())
			{
				if (m_pool->getTask(task))
				{
					changeStatus(WorkStatus::Running);
					task();
				}
			}
			changeStatus(WorkStatus::Exit);
		});
}

WorkThread::~WorkThread()
{
	m_thread.join();
}

#endif	//__WORKTHREAD_H_