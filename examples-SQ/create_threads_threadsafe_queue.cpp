#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include <pthread.h> //for pthread_t 


#include "../src-SQ_c++11/ThreadSafeQueue.h"
#include "../src-SQ/CriticalStorage.h"


void *manipulate(void *p);

class VBlock{
	int VIndex;
public:
	VBlock(int index): VIndex(index) {};
	~VBlock() {};
	int getVIndex()	{
		unsigned int microseconds=50000;
                usleep(microseconds);
		return VIndex;
	};
};

int main(int argc, char* argv[]){

	if (argc < 3) {
		std::cerr<<"USAGE "<<argv[0]<<" queue_length nPthreads";
		std::cerr<<"SpareTire\n";
		return 1;
	}
	SizeType QueueLength = atoi(argv[1]);
	SizeType nPthreads = atoi(argv[2]);
	SizeType SpareTire = (argc == 4) ? atoi(argv[3]) : 1;
	std::cout<<"push_object_into_a_queue example \n";
	std::cout<<"input: "<<QueueLength<<" "<<nPthreads<<" "<<SpareTire<<"\n";

	VBlock* V[QueueLength];
	//std::vector< VBlock* > V[QueueLength];
	for (SizeType i=0; i<QueueLength; i++){
		V[i] = new VBlock(i);//create a VBlock object, assign it to a pointer 
	}
	
	//std::queue<VBlock*> ThreadPool;//create a normal queue to hold VBlock pointers
        typedef ConcurrentPsi::ThreadSafeQueue<VBlock*> ThreadSafeQueueType;
        ThreadSafeQueueType ThreadPool;//create a ThreadSafe queue to hold VBlock pointers 
	//push it, control of the dynamic memory is now the responsibility of the queue
	for (SizeType i=0; i<QueueLength; i++)
		ThreadPool.push(V[i]); 

/* implement pthreads to execute the following in multiple threads parallelly.
	while(!ThreadPool.empty()){ //continue if there are additional items left
		VBlock* tempVBlockHandle = NULL; //see below for usage of this handle
                if( ThreadPool.can_pop(tempVBlockHandle) ) {
			std::cout << tempVBlockHandle->getVIndex() << std::endl; 
			delete tempVBlockHandle; //delete it (since we allocated with new)
		}
	}
*/

	pthread_t threadId[nPthreads];
	int params[nPthreads];
        for (SizeType i=0; i<nPthreads; i++){
		std::cout<<"launch pthreads here. i="<<i<<"\n";
		int ret=0;
		params[i]=0;
		if ( (ret=pthread_create(&threadId[i], NULL, manipulate, &params[i])) )
			std::cerr<<"Thread creation failed: "<<ret<<"\n"; 
	}  
	for (SizeType i=0; i<nPthreads; i++)pthread_join(threadId[i],0);
	return 0;
}

void *manipulate(void *p) {pthread_exit(NULL);}


/*
void WorkFromThreadPool(ThreadSafeQueueType& tp){
	typedef ConcurrentPsi::ThreadSafeQueue<VBlock*> ThreadSafeQueueType;

        while(!tp.empty()){ //continue if there are additional items left
                VBlock* tempVBlockHandle = NULL; //see below for usage of this handle
                if( tp.can_pop(tempVBlockHandle) ) {
                        std::cout << tempVBlockHandle->getVIndex() << std::endl; 
                        delete tempVBlockHandle; //delete it (since we allocated with new)
                }


}
*/
