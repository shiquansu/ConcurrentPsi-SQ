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

template<typename T>
class MultiQsObjThreadPool {
public:
	std::atomic_bool done;
	std::atomic_bool submissionDone;
	std::atomic_bool jobDone;
private:
	ConcurrentPsi::ThreadSafeQueue<T> centralWorkQueue;
	
	typedef std::queue<T> LocalQueueType;
	std::unique_ptr<LocalQueueType> localWorkQueue;
	//static std::unique_ptr<LocalQueueType> localWorkQueue;
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
	MultiQsObjThreadPool():
		done(false), submissionDone(false), jobDone(false),joiner(threadsVector)
	{
		unsigned const thread_count=std::thread::hardware_concurrency();
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				threadsVector.push_back(
					std::thread(&MultiQsObjThreadPool::threadRunTaskFromQueue, this)
				);
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}
	
	~MultiQsObjThreadPool()
	{
		done=true;
		jobDone=true;
		submissionDone=true;
	}

	void submit(T taskObj)
	{
		if( localWorkQueue ){
			localWorkQueue->push(std::move(taskObj));
		}else{ 
			centralWorkQueue.push(std::move(taskObj));
		}
	}

	void runPendingTask(){
		T task;
		if ( localWorkQueue && !localWorkQueue->empty() ){
			task=std::move( localWorkQueue->front() );
			localWorkQueue->pop();
			task->executeTask();
		}else if( centralWorkQueue.can_pop(task) ){
			task->executeTask();
		}else{
			std::this_thread::yield();
		}
	}	
}; //class MultiQsObjThreadPool

} //namespace ConcurrentPsi
