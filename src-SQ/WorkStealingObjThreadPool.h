#include <queue>
//#include <mutex> //require -std=c++11
//#include <condition_variable> //require -std=c++11
#include <memory>
//#include <future> // for future
#include <type_traits> // for result_of
//#include <atomic> // require -std=c++11

#include "./ThreadSafeQueue.h"
#include "./WorkStealingQ.h"
#include "../src/CriticalStorage.h" // for SizeType definition

namespace ConcurrentPsi {

template<typename T>
class WorkStealingObjThreadPool {
public:
	//std::atomic_bool done;
	//std::atomic_bool submissionDone;
	//std::atomic_bool jobDone;
	bool done;
	bool submissionDone;
	bool jobDone;
private:
	ConcurrentPsi::ThreadSafeQueue<T> centralWorkQ;

	ConcurrentPsi::WorkStealingQ<T>* localWorkQ;
	//typedef ConcurrentPsi::WorkStealingQ<T> LocalWorkQType;
	//std::unique_ptr<LocalWorkQType> localWorkQ;
	//static thread_local ConcurrentPsi::WorkStealingQ<T>* localWorkQ;
 
	std::vector< std::unique_ptr<WorkStealingQ<T>> > workerQ;

	//std::vector<std::thread> threadsVector;
	std::vector<Pthreads::PthreadType> threadsVector;

	//JoinThreads joiner;

	//static thread_local ConcurrentPsi::WorkStealingQ<T>* localWorkQ;
	unsigned myIndex;	
	//thread_local unsigned myIndex;	
	//static thread_local unsigned myIndex;	

	void threadRunTaskFromQueue(unsigned myIndex_){
		myIndex=myIndex_;
		localWorkQ=workerQ[myIndex].get();
		//while(!done){
		while( !(localWorkQ->empty() && centralWorkQ.empty() && submissionDone) ){
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
		std::cout<<"In WorkStealingObjThreadPool(), thread_count is: "<<thread_count<< "\n"; 
		try
		{
			for ( unsigned i=0; i<thread_count; ++i )
			{
				workerQ.push_back( std::unique_ptr<WorkStealingQ<T>>(new WorkStealingQ<T>) );
				threadsVector.push_back(
				//	std::thread(&WorkStealingObjThreadPool::threadRunTaskFromQueue, this, i)
					Pthreads::create(this, NULL, &WorkStealingObjThreadPool::threadRunTaskFromQueue, i)
				);
			}
		std::cout<<"In WorkStealingObjThreadPool(), after generating all the threads.\n"; 
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
			std::cout<<"In WorkStealingObjThreadPool.h, before push to localWorkQ.\n";
			//taskObj->executeTask();
			localWorkQ->push(taskObj);
			//localWorkQ->push(std::move(taskObj));
			std::cout<<"In WorkStealingObjThreadPool.h, after push to localWorkQ.\n";
		}else{ 
			std::cout<<"In WorkStealingObjThreadPool.h, push to centralWorkQ.\n";
			centralWorkQ.push(std::move(taskObj));
		}
	}

	void runPendingTask(){
		T taskRef;
		if ( popTaskFromLocalQ(taskRef) ||
		     popTaskFromCentralQ(taskRef) ||
		     popTaskFromOtherQ(taskRef) )
		{
			taskRef->executeTask();
		}else{
			std::this_thread::yield();
		}
	}	
}; //class WorkStealingObjThreadPool

} //namespace ConcurrentPsi
