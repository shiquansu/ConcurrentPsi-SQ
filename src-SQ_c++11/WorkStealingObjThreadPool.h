#include <queue>
#include <mutex> //require -std=c++11
#include <condition_variable> //require -std=c++11
#include <memory>
#include <future> // for future
#include <type_traits> // for result_of

#include "./ThreadSafeQueue.h"
#include "./WorkStealingQ.h"
#include "../src/CriticalStorage.h" // for SizeType definition

namespace ConcurrentPsi {

template<typename T>
class WorkStealingObjThreadPool {
public:
	std::atomic_bool done;
	std::atomic_bool submissionDone;
	std::atomic_bool jobDone;
private:
	ConcurrentPsi::ThreadSafeQueue<T> centralWorkQ;
	std::vector< std::unique_ptr<WorkStealingQ<T>> > workerQ;
	std::vector<std::thread> threadsVector;
	JoinThreads joiner;

	thread_local WorkStealingQ<T>* localWorkQ;
	//static thread_local WorkStealingQ<T>* localWorkQ;
	unsigned myIndex;	
	//thread_local unsigned myIndex;	
	//static thread_local unsigned myIndex;	

	void threadRunTaskFromQueue(unsigned myIndex_){
		myIndex=myIndex_;
		localWorkQ=workerQ[myIndex].get();
		//while(!done){
		while( !(workerQ.empty() && centralWorkQ.empty() && submissionDone) ){
			runPendingTask();
		}
		jobDone=true;
	}

	bool popTaskFromLocalQ(T& task){
		return localWorkQ && localWorkQ->can_pop(task);
	}

	bool popTaskFromCentralQ(T& task){
		return centralWorkQ.can_pop(task);
	}


	bool popTaskFromOtherQ(T& task){
		for (unsigned i=0; i<workerQ.size();++i){
			unsigned const index=(myIndex+i+1)%workerQ.size();
			if( workerQ[index]->can_steal(task) ) return true;
		}
		return false;
	}

public:
	WorkStealingObjThreadPool():
		done(false), submissionDone(false), jobDone(false),joiner(threadsVector)
	{
		unsigned const thread_count=std::thread::hardware_concurrency();
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				workerQ.push_back( std::unique_ptr<WorkStealingQ<T>>(new WorkStealingQ<T>) );
				threadsVector.push_back(
					std::thread(&WorkStealingObjThreadPool::threadRunTaskFromQueue, this, i)
				);
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}
	
	~WorkStealingObjThreadPool()
	{
		done=true;
		jobDone=true;
		submissionDone=true;
	}

	void submit(T taskObj)
	{
		if( localWorkQ ){
			localWorkQ->push(std::move(taskObj));
		}else{ 
			centralWorkQ.push(std::move(taskObj));
		}
	}

	void runPendingTask(){
		T task;
		if ( popTaskFromLocalQ(task) ||
		     popTaskFromCentralQ(task) ||
		     popTaskFromOtherQ(task) )
		{
			task->executeTask();
		}else{
			std::this_thread::yield();
		}
	}	
}; //class WorkStealingObjThreadPool

} //namespace ConcurrentPsi
