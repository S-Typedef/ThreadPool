#include <iostream>
#include "threadpool.h"
#include "task.h"



template<typename Ret, typename ...Args>
std::future<Ret> addTask(ThreadPool* p, Task<Ret, Args...>* task)
{
	bool exp = false;
	while (p->m_inQueue.compare_exchange_weak(exp, true))
		exp = false;
	auto task_future = std::make_shared<std::packaged_task<Ret(Args...)>>(
		[task]() {
			return task->run();
		}
	);
	auto ret_future = task_future->get_future();

	//包装
	std::function<void()> f = [task_future]() {
		try
		{

			(*task_future)();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
	};
	p->m_tasks.push(f);
	p->m_inQueue.store(false, std::memory_order_seq_cst);
	return ret_future;
}


void test1()
{
	std::cout << "123" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
void test2()
{
	std::cout << "456" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
void test3()
{
	std::cout << "789" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int, char**) {
	ThreadPool pool;

	for (int i = 0; i < 30; i++)
	{
		addTask(&pool, new Task<void>(test1));
		addTask(&pool, new Task<void>(test2));
		addTask(&pool, new Task<void>(test3));
	}
	std::this_thread::sleep_for(std::chrono::seconds(60));
	pool.setPoolFinish();
	return 0;
}
