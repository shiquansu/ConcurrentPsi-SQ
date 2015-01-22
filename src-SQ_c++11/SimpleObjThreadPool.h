#include <queue>
#include <mutex> //require -std=c++11
#include <condition_variable> //require -std=c++11
#include <memory>

#include "./ThreadSafeQueue.h"
#include "../src/CriticalStorage.h" // for SizeType definition

namespace ConcurrentPsi {

template<typename T>  
class SimpleObjThreadPool {
public:
	std::atomic_bool done;
	std::atomic_bool submissionDone;
	std::atomic_bool jobDone;
private:
	ConcurrentPsi::ThreadSafeQueue<T> workQueue;
	std::vector<std::thread> threadsVector;
	JoinThreads joiner;

	void threadRunTaskFromQueue()
	{
		while( !(workQueue.empty() && submissionDone) )
		//while(!done)
		{
			T task;
			if(workQueue.can_pop(task))
			{
				task->executeTask();
			}
			else
			{
				std::this_thread::yield();
			}
		}
		jobDone=true;
	}
public:
	SimpleObjThreadPool():
		done(false), submissionDone(false), jobDone(false),joiner(threadsVector)
	{
		unsigned const thread_count=std::thread::hardware_concurrency();
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				threadsVector.push_back(
					std::thread(&SimpleObjThreadPool::threadRunTaskFromQueue, this)
				);
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}
	
	~SimpleObjThreadPool()
	{
		done=true;
		jobDone=true;
		submissionDone=true;
	}

	void submit(T taskObj)
	{
		//workQueue.push(std::move(taskObj));
		workQueue.push(taskObj);
	}
}; //class SimpleObjThreadPool

} //namespace ConcurrentPsi
