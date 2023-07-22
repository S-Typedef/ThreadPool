#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_


#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <functional>

template<typename Ret, typename ...Args>
class Task;
class WorkThread;

class ThreadPool
{
public:
	ThreadPool(int size = 20);
	~ThreadPool();
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

public:
	bool getTask(std::function<void()>&);

	bool& isPoolFinish();
	void setPoolFinish(bool b = true) { m_isFinish = b; }

	std::atomic_bool m_inQueue{ false };
	std::queue<std::function<void()>> m_tasks;
private:

private:
	std::vector<WorkThread*> m_threads;
	bool m_isFinish{ false };
};

#endif	//#__THREADPOOL_H_