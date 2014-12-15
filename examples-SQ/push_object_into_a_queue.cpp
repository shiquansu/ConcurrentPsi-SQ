#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>

#include "../src-SQ_c++11/ThreadSafeQueue.h"
#include "../src-SQ/CriticalStorage.h"

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

	if (argc < 2) {
		std::cerr<<"USAGE "<<argv[0]<<"queue_length ";
		std::cerr<<"queue_length\n";
		return 1;
	}
	SizeType QueueLength = atoi(argv[1]);
	SizeType SpareTire = (argc == 3) ? atoi(argv[2]) : 1;
	std::cout<<"push_object_into_a_queue example \n";
	std::cout<<"input: "<<QueueLength<<" "<<SpareTire<<"\n";

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

	while(!ThreadPool.empty()){
                // ThreadSafe queue example. front and pop are combined in can_pop method
		VBlock* tempVBlockHandle = NULL; //see below for usage of this handle
                if( ThreadPool.can_pop(tempVBlockHandle) ) {
			std::cout << tempVBlockHandle->getVIndex() << std::endl; 
			delete tempVBlockHandle; //delete it (since we allocated with new)
		}
		//continue if there are additional items left
	}
	return 0;
}

