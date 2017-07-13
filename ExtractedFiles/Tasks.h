#ifndef TASKS_H
#define TASKS_H
///////////////////////////////////////////////////////////////////////////////
// Tasks.h - executes a specified callable object on a ThreadPool thread     //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project 3 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Package provides a Task class that executes a specified callable object on
* a ThreadPool thread, by using services of the ThreadPool package.
*
* Public Interface:
*------------------
* template<typename Result, typename Parameter>
* class Task :
*		++ void start(size_t) :
*				        -- Pstart child thread that dequeus work items
*		++ void doWork(workItem<Result,Parameter>* pWi) :
*						-- enqueue work item
*		++ Result result() :
*						-- retrieve results with blocking call
*		++ void wait() :
*						-- wait for child thread to terminate
*
* Build Process:
* --------------
* Required Files: Tasks.h, Tasks.cpp, ThreadPool.h, ThreadPool.cpp, 
*                 Workitem.h, Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
* Build Command: devenv Project 3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*/

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../ThreadPool/ThreadPool.h"
#include "../ThreadPool/Workitem.h"
#include <thread>
#include <functional>

template<typename Result, typename Parameter>
class Task
{
private:
	ThreadPool<bool, std::string> tp;
public:
	void start(size_t);
	void doWork(workItem<Result, Parameter>* pWi);
	Result result();
	void wait();
};

//----< wait for child thread to terminate >---------------------------

template<typename Result, typename Parameter>
void Task<Result, Parameter>::wait()
{
	tp.wait();
}

//----< enqueue work item >--------------------------------------------

template<typename Result, typename Parameter>
void Task<Result, Parameter>::doWork(workItem<Result, Parameter>* pWi)
{
	tp.doWork(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result, typename Parameter>
Result Task<Result, Parameter>::result()
{
	return tp.result();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result, typename Parameter>
void Task<Result, Parameter>::start(size_t size) {
	tp.start(size);
}

#endif
