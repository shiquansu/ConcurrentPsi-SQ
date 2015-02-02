// edit from list 9.2, but have not compiled, good luck
#include <queue>
#include <mutex> //require -std=c++11
#include <condition_variable> //require -std=c++11
#include <memory>
#include <future> // for future
#include <type_traits> // for result_of

#include "./ThreadSafeQueue.h"
#include "./FunctionWrapper.h"
#include "../src/CriticalStorage.h" // for SizeType definition

namespace ConcurrentPsi {

class WaitableTasksThreadPool {
public:
	std::atomic_bool done;
	std::atomic_bool submissionDone;
	std::atomic_bool jobDone;
private:
	ConcurrentPsi::ThreadSafeQueue<FunctionWrapper> workQueue;
	std::vector<std::thread> threadsVector;
	JoinThreads joiner;

	void threadRunTaskFromQueue(){
		//while(!done)
		while( !(workQueue.empty() && submissionDone) ){
			FunctionWrapper task;
			if(workQueue.can_pop(task)){
				task->executeTask();
			}else{
				std::this_thread::yield();
			}
		}
		jobDone=true;
	}
public:
	WaitableTasksThreadPool():
		done(false), submissionDone(false), jobDone(false),joiner(threadsVector)
	{
		unsigned const thread_count=std::thread::hardware_concurrency();
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				threadsVector.push_back(
					std::thread(&WaitableTasksThreadPool::threadRunTaskFromQueue, this)
				);
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}
	
	~WaitableTasksThreadPool()
	{
		done=true;
		jobDone=true;
		submissionDone=true;
	}

	template <typename FuncType>
	std::future<typename std::result_of<FuncType()>::type> submit(FuncType taskObj){
		typedef typename std::result_of<FuncType()>::type resultType;
		std::packaged_task<resultType()> task(std::move(taskObj));
		std::future<resultType> res(task.get_future()); 
		workQueue.push(std::move(taskObj));
		return res;
	}
}; //class WaitableTasksThreadPool

} //namespace ConcurrentPsi
