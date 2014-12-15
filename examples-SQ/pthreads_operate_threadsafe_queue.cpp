#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include <pthread.h> //for pthread_t 

#include "../src-SQ_c++11/ThreadSafeQueue.h"
#include "../src-SQ/CriticalStorage.h"

class VBlock{
	int VIndex;
public:
	VBlock(int index): VIndex(index) {};
	~VBlock() {};
	int getVIndex()	{
		unsigned int microseconds=1000000;
                usleep(microseconds);
		return VIndex;
	};
};

typedef ConcurrentPsi::ThreadSafeQueue<VBlock*> ThreadSafeQueueType;

struct WorkerThreadStruct {
        WorkerThreadStruct()
                : done(true), threadNum(0)//, tp()
        {}
        bool done;
	int threadNum;
        ThreadSafeQueueType* tp;
};

void *workFromQueue(void *p);

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
	for (SizeType i=0; i<QueueLength; i++){
		V[i] = new VBlock(i);//create a VBlock object, assign it to a pointer 
	}
	
        ThreadSafeQueueType ThreadPool;//create a ThreadSafe queue to hold VBlock pointers 
	//push it, control of the dynamic memory is now the responsibility of the queue
	for (SizeType i=0; i<QueueLength; i++)
		ThreadPool.push(V[i]); 

	pthread_t threadId[nPthreads];
	struct WorkerThreadStruct wtparams[nPthreads]; 
	for (SizeType i=0; i<nPthreads; i++){
		wtparams[i].done = true;	
		wtparams[i].threadNum = i;	
		wtparams[i].tp = &ThreadPool;	
                std::cout<<"launch pthreads here. i="<<i<<"\n";
                int ret=0;
                if ( (ret=pthread_create(&threadId[i], NULL, workFromQueue, &wtparams[i])) )
                        std::cerr<<"Thread creation failed: "<<ret<<"\n";
        }
        for (SizeType i=0; i<nPthreads; i++)pthread_join(threadId[i],0);
	return 0;
}

void *workFromQueue(void *p)
{
	WorkerThreadStruct *wtparams = (WorkerThreadStruct*)p; 	
        bool done = wtparams->done;
        int tn = wtparams->threadNum;
	ThreadSafeQueueType* tp = wtparams->tp;	
        while(!tp->empty()){ //continue if there are additional items left
                VBlock* tempVBlockHandle = NULL;
                if( tp->can_pop(tempVBlockHandle) ) {
                        std::cout<<"I am VBlock with VIndex="<<tempVBlockHandle->getVIndex()<<" using threadNum="<<tn<<" done="<<done<<std::endl;
			delete tempVBlockHandle; //delete it (since we allocated with new)
		}
	}
	pthread_exit(NULL);	
};
