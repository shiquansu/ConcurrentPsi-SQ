// the stall and enable features have not been implemented suscessfully.

#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include <pthread.h> //for pthread_t 

#include "../src-SQ_c++11/ThreadSafeQueueWithDependency.h"
#include "../src-SQ/CriticalStorage.h"

class TDVB{
	int VIndex;
	bool done;
public:
	TDVB(int index, SizeType argv[]): VIndex(index),done(false) {
        	if (argv[2] < 0) {
                	std::cerr<<"USAGE "<<argv[0]<<" "<<index;
                	std::cerr<<"#_of_in V_in...\n";
                	std::cerr<<"#_of_out V_out...\n";
        	}	
        	SizeType NumIn = argv[0];
		SizeType VBInD[NumIn];
        	SizeType NumOut = argv[NumIn+1];
		SizeType VBOutD[NumOut];
		std::cout<<"\n TDVB in:";
		for ( SizeType i=0; i< NumIn; i++){
			VBInD[i]=argv[i+1];
			std::cout<<" "<<VBInD[i];
		}
		std::cout<<" TDVB out:";
		for ( SizeType i=0; i< NumOut; i++){
			VBOutD[i]=argv[i+1+NumIn+1];
			std::cout<<" "<<VBOutD[i];
		}
	};

        ~TDVB() {};
        int getVIndex() {
                return VIndex;
        };
        void execute() {
                unsigned int microseconds=1000000;
                usleep(microseconds);
		done = true;
        };
 
};

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

typedef ConcurrentPsi::ThreadSafeQueue<TDVB*> ThreadSafeQueueType;

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

	//generate all Tasks Dependent V Blocks (TDVB). 
	TDVB* V[QueueLength];
	for (SizeType i=0; i<QueueLength; i++){
		SizeType aa[]={1, i, 1, i+1};
		V[i] = new TDVB(i,aa);//create a VBlock object, assign it to a pointer 
	}
	
	//push to the queue
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
                TDVB* tempVBlockHandle = NULL;
                if( tp->can_pop(tempVBlockHandle) ) {
			tempVBlockHandle->execute();	
                        std::cout<<"I am VBlock with VIndex="<<tempVBlockHandle->getVIndex()<<" using threadNum="<<tn<<" done="<<done<<std::endl;
			delete tempVBlockHandle; //delete it (since we allocated with new)
		}
	}
	pthread_exit(NULL);	
};
