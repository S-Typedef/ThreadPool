#ifndef __TASK_H_
#define __TASK_H_


#include <functional>


template<typename Ret, typename ...Args>
class Task
{
private:
	/* data */
	std::function<Ret(Args...)> m_task;
public:
	Task(std::function<Ret(Args...)>);
	~Task();
	Ret run();
};

template<typename Ret, typename ...Args>
Task<Ret, Args...>::Task(std::function<Ret(Args...)> func) : m_task(func)
{
}

template<typename Ret, typename ...Args>
Task<Ret, Args...>::~Task()
{
}

template<typename Ret, typename ...Args>
Ret Task<Ret, Args...>::run()
{
	return m_task();
}

#endif	//__TASK_H