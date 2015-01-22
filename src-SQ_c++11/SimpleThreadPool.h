#include <queue>
#include <mutex> //require -std=c++11
#include <condition_variable> //require -std=c++11
#include <memory>

#include "./ThreadSafeQueue.h"
#include "../src/CriticalStorage.h" // for SizeType definition

namespace ConcurrentPsi {

//template<typename T>  //template me with class T, if needed
class SimpleThreadPool {
	std::atomic_bool done;
	ThreadSafeQueue< std::function<void()> > workQueue;
	std::vector<std::thread> threadsVector;
	JoinThreads joiner;

	void threadRunTaskFromQueue()
	{
		while(!done)
		{
			std::function<void()> task;
			if(workQueue.can_pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}
public:
	SimpleThreadPool():
		done(false),joiner(threadsVector)
	{
		unsigned const thread_count=std::thread::hardware_concurrency();
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				threadsVector.push_back(
					std::thread(&SimpleThreadPool::threadRunTaskFromQueue, this)
				);
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}
	
	~SimpleThreadPool()
	{
		done=true;
	}

	template<typename workFunctionType>
	void submit(workFunctionType f)
	{
		workQueue.push(std::function<void()>(f));
	}
}; //class ThreadPool

} //namespace ConcurrentPsi
