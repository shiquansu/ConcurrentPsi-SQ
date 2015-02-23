// generic driver for all different threadpool
//
#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include <pthread.h> //for pthread_t 
//#include <atomic>
#include <chrono>
#include <string>

#include "../src-SQ/WorkStealingObjThreadPool.h"
#include "../src-SQ/CriticalStorage.h"

class VBlock{
        int VIndex;
public:
        VBlock(int index): VIndex(index) {};
        ~VBlock() {};

        void executeTask() {
                unsigned int microseconds=1000000;
                usleep(microseconds+10000);
                //usleep(microseconds+VIndex*10000);
		std::string str="in VBlock with VIndex= "+std::to_string(microseconds)+"\n";
		//std::string str="in VBlock with VIndex= "+std::to_string(VIndex)+"\n";
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

	std::atomic_bool done;
	ConcurrentPsi::WorkStealingObjThreadPool<VBlock*> testPool; 

	// generate a lot of VBlock tasks
	VBlock* V[job_num];
	for (SizeType i=0; i<job_num; i++){
		V[i] = new VBlock(i);//create a VBlock object, assign it to a pointer 
	}	

        for (SizeType i=0; i<job_num; i++){
		testPool.submit( V[i] );
        }
	printf("submit finish\n");
	testPool.submissionDone = true;
	while(testPool.jobDone == false) std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return 0;
}
