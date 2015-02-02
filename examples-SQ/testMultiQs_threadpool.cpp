#include <unistd.h> // for usleep(microseconds) call
#include <vector>
#include <queue>
#include <iostream>
#include <pthread.h> //for pthread_t 
#include <atomic>
#include <chrono>
#include <string>

#include "../src-SQ_c++11/MultiQsThreadPool.h"
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

//template<typename T> //template me if needed
class TBlock{
        int TIndex;
public:
        TBlock(int index): TIndex(index) {};
        ~TBlock() {};

	void operator()() {
                unsigned int microseconds=1000000;
                usleep(microseconds+TIndex*10000);
                std::string str="in TBlock with TIndex= "+std::to_string(TIndex)+"\n";
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

	std::vector< std::future<std::result_of<TBlock()>> > futureT[job_num];
	//std::atomic_bool done;
	ConcurrentPsi::MultiQsThreadPool testPool;  // implement function wrapper in the threadpool submit method, so no longer need to pass the task type to the thread pool, the thread pool get the template type by FunctionType().

// *********** need to follow 9.3 to write the driver, since it needs a std::vector< std::future<T> > to hold the return of the submit

	// generate a lot of TBlock tasks
	TBlock* T[job_num];
	for (SizeType i=0; i<job_num; i++){
		T[i] = new TBlock(i);//create a TBlock object, assign it to a pointer 
	}	

        for (SizeType i=0; i<job_num; i++){
                futureT[i]=testPool.submit( T[i]->operator() );
        }
	printf("submit finish\n");
	testPool.submissionDone = true;
	while(testPool.jobDone == false) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        return 0;
}
