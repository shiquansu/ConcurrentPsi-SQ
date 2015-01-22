#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include <pthread.h> //for pthread_t 
#include <atomic>
#include <chrono>

#include "../src-SQ_c++11/SimpleObjThreadPool.h"
#include "../src-SQ/CriticalStorage.h"

class VBlock{
        int VIndex;
public:
        VBlock(int index): VIndex(index) {};

        ~VBlock() {};

        int getVIndex() {
                unsigned int microseconds=1000000;
                usleep(microseconds+VIndex*10000);
                return VIndex;
        };

        void executeTask() {
                unsigned int microseconds=1000000;
                usleep(microseconds);
                std::cout<<"in VBlock with VIndex="<<VIndex<<"\n";
        };
};

void a()
{
    //while(true)
    {
        printf("a\n");
    }
}

int main(int argc, char* argv[]){

        if (argc < 3) {
                std::cerr<<"USAGE "<<argv[0]<<" job_number nPthreads";
                std::cerr<<"SpareTire\n";
                return 1;
        }
        SizeType job_num = atoi(argv[1]);
        SizeType nPthreads = atoi(argv[2]);
        SizeType SpareTire = (argc == 4) ? atoi(argv[3]) : 1;
        std::cout<<"simple object thread pool example (job_num~100, nPthreads~8)\n";
        std::cout<<"input: "<<job_num<<" "<<nPthreads<<" "<<SpareTire<<"\n";

	std::atomic_bool done;
	ConcurrentPsi::SimpleObjThreadPool<VBlock*> testPool;
	//ConcurrentPsi::SimpleObjThreadPool<VBlock> testPool;

	// generate a lot of VBlock tasks
	VBlock* V[job_num];
	for (SizeType i=0; i<job_num; i++){
		V[i] = new VBlock(i);//create a VBlock object, assign it to a pointer 
	}	
/*  The following section tries to push the member function to the queue, but fails. We need to come back later.
	submit VBlock method printVIndex as function to the SimpleThreadPool, SimpleThreadPool use a thread safe workQueue to hold these functions and then the thread work from the queue.
	using VBPtrType = void(VBlock::*)();
	using VBPtrType = void(VBlock::*)(); //pointer-to-member, in this case, VBlock class. Shall I use VBlock class object V[i]-> ?
	VBPtrType printVIndexPtr = &V[1]->printVIndex;
	VBPtrType printVIndexPtr = &VBlock::printVIndex;
	printVIndexPtr = printVIndexPtr;
	for (SizeType i=0; i<job_num; i++){
		//testThreadPool.submit( V[i]->*printVIndex );
		testThreadPool.submit( V[i] );
		//testThreadPool.submit(*a);
	}	
*/

        for (SizeType i=0; i<job_num; i++){
                testPool.submit( V[i] );
        }
	printf("submit finish\n");
//	while(testpool->done == false) std::this_thread::sleep(std::milliseconds(100));
        return 0;
}
