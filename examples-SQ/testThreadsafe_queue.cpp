#include <iostream>

#include <vector>
#include <algorithm>
#include <functional>
#include <thread>

#include "../src-SQ/ThreadSafeQueue.h"
#include "../src-SQ/CriticalStorage.h"

class KernelNoOutput{

public:

	typedef double RealType;

	KernelNoOutput(SizeType total)
	    : total1_(total)
	{}

	std::function<void()> operator()( SizeType index )
	//void operator()( SizeType index ) const
	{
		int totalKernel=300;
		int bigNumber = 1e6;

		RealType sum = 0.0;
		for (int j = 0; j < totalKernel; ++j) {
			for (int mm = 0; mm < bigNumber; mm++)
				sum += (index + 1.0)*1e-6;
		}
		std::cout<<"end kernel, with index= "<<index<<"\n";
                return 0;
	}

        std::function<void()> operator2()
	{
		RealType sum=0.0;
        	for (int j=0; j< 100000; ++j) {
			sum += 0.0;       
		}
		std::cout<<" in operator2\n";
		return 0;
	}

	SizeType size() const { return total1_; }

private:

	SizeType total1_;
};


int main(int argc, char* argv[])
{
        typedef ConcurrentPsi::ThreadSafeQueue< std::function<void()> > ThreadSafeQueueType;

	if (argc < 3) {
		std::cerr<<"USAGE "<<argv[0]<<"nthreads1, ntasks1 ";
		std::cerr<<"[nthreads2]\n";
		return 1;
	}
	SizeType nthreads1 = atoi(argv[1]);
	SizeType ntasks1 = atoi(argv[2]);
	SizeType nthreads2 = (argc == 4) ? atoi(argv[3]) : 1;
	std::cout<<"threadsafe_queue example \n";
	std::cout<<"input: "<<nthreads1<<" "<<nthreads2<<"\n";

        ThreadSafeQueueType work_queue;
        //std::atomic_bool done;
        std::vector<std::thread> threads;

        KernelNoOutput kernel1(ntasks1);
        // call kernel1 with index=123
        kernel1.operator()(123);
      
        std::function<void()> f;
        f();
        //work_queue.push(std::function<void()>(f));
        //work_queue.push(std::function<void()>(f));
        
        kernel1.operator()(126);

        //work_queue.push(void( kernel1.operator()(124) ));
        //work_queue.push(std::function<void()>( kernel1.operator()(128) ));
        work_queue.push(std::function<void()>( kernel1.operator2() ));
        work_queue.push(std::function<void()>( kernel1.operator2() ));

        std::function<void()> task1;
        std::function<void()> task2;

        if( work_queue.can_pop(task1) ) std::cout<<"I am still OK line 85 \n";
        if( work_queue.can_pop(task2) ){
           //task = work_queue.try_pop();
           task2();
        }
       
}

std::function<void()> f()
{
   std::cout<<"I am in void f() \n";
   return 0;
}

