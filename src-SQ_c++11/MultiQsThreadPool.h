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

class MultiQsThreadPool {
public:
	std::atomic_bool done;
	std::atomic_bool submissionDone;
	std::atomic_bool jobDone;
private:
	ConcurrentPsi::ThreadSafeQueue<FunctionWrapper> centralWorkQueue;
	
	typedef std::queue<FunctionWrapper> LocalQueueType;
	static std::unique_ptr<LocalQueueType> localWorkQueue;
	std::vector<std::thread> threadsVector;
	JoinThreads joiner;

	void threadRunTaskFromQueue(){
		localWorkQueue.reset(new LocalQueueType);
		while(!done){
		//while( !(workQueue.empty() && submissionDone) ){
			runPendingTask();
		}
		jobDone=true;
	}
public:
	MultiQsThreadPool():
		done(false), submissionDone(false), jobDone(false),joiner(threadsVector)
	{
		unsigned const thread_count=std::thread::hardware_concurrency();
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				threadsVector.push_back(
					std::thread(&MultiQsThreadPool::threadRunTaskFromQueue, this)
				);
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}
	
	~MultiQsThreadPool()
	{
		done=true;
		jobDone=true;
		submissionDone=true;
	}

	template <typename FuncType>
	//std::future<void> submit(FuncType taskObj){
	std::future<typename std::result_of<FuncType()>::type> submit(FuncType taskObj){
		typedef typename std::result_of<FuncType()>::type resultType;
		//typedef typename std::result_of<FuncType()>::type resultType;
		std::packaged_task<resultType()> task(taskObj);
		std::future<resultType> res(task.get_future());
		if( localWorkQueue ){
			localWorkQueue->push(std::move(taskObj));
		}else{ 
			centralWorkQueue.push(std::move(taskObj));
		}
		return res;
	}

	void runPendingTask(){
		FunctionWrapper task;
		if ( localWorkQueue && !localWorkQueue->empty() ){
			task=std::move( localWorkQueue->front() );
			localWorkQueue->pop();
			task();
		}else if( centralWorkQueue.can_pop(task) ){
			task();
		}else{
			std::this_thread::yield();
		}
	}	
}; //class MultiQsThreadPool

} //namespace ConcurrentPsi
