// generic driver for all different threadpool
//
#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include "Pthreads.h" //for pthread_t 
//#include <atomic>   //require c++11
//#include <chrono>   //require c++11
#include <string>

#include "../src-SQ/WorkStealingObjThreadPool.h"
//#include "../src-SQ_c++11/WorkStealingObjThreadPool.h"
#include "../src-SQ/CriticalStorage.h"

class VBlock{
        int VIndex;
public:
        VBlock(int index): VIndex(index) {};
        ~VBlock() {};

        void executeTask() {
                unsigned int microseconds=1000000;
                usleep(microseconds+VIndex*10000);
		std::string str="in VBlock with VIndex= "+std::to_string(VIndex)+"\n";
		std::cout<<str;
        };
};

class DBlock{
        int DIndex;
public:
        DBlock(int index): DIndex(index) {};
        ~DBlock() {};

        void executeTask() {
                unsigned int microseconds=1000000;
                usleep(microseconds+10000);
                std::string str="in a dump DBlock which can not print DIndex\n";
                std::cout<<str;
        };
};


void a() { printf("a\n"); }

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

	//std::atomic_bool done;
	ConcurrentPsi::WorkStealingObjThreadPool<DBlock*> testPool; 

	// generate a lot of VBlock tasks
	DBlock* B[job_num];
	for (SizeType i=0; i<job_num; i++){
		B[i] = new DBlock(i);//create a VBlock object, assign it to a pointer 
	}	
	std::cout<<"In test_threadpool.cpp line 52, after initialize all the VBlocks.\n"; 
        for (SizeType i=0; i<job_num; i++){
	std::cout<<"In test_threadpool.cpp line 54, pushing "<<i<<"th VBlock pointer to testpool.\n"; 
		testPool.submit( B[i] );
        }
	printf("submit finish\n");
	testPool.submissionDone = true;
	//while(testPool.jobDone == false) std::this_thread::sleep_for(std::chrono::milliseconds(20));
	while(testPool.jobDone == false) sleep(1);
        return 0;
}
