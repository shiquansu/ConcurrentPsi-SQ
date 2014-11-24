#include <iostream>
#include "../src/ParallelizerAsync.h"
//#include "../src/Parallelizer.h"
#include "../src/CriticalStorage.h"
#include "../src/KernelCompositor.h"


class KernelLong{

public:

	typedef double RealType;

	KernelLong(SizeType total)
	    : total1_(total)
	{}

	template<typename SomeCriticalStorageType>
	void operator()(SizeType index,
	                SizeType threadNum,
	                SomeCriticalStorageType& cs) const
	{
		int totalKernel=300;
		int bigNumber = 1e6;

		RealType sum = 0.0;
		for (int j = 0; j < totalKernel; ++j) {
			for (int mm = 0; mm < bigNumber; mm++)
				sum += (index + 1.0)*1e-6;
		}

		cs.value(0,threadNum) += sum;
	}

	SizeType size() const { return total1_; }

private:

	SizeType total1_;
};

class KernelShort {

public:

	typedef double RealType;

	KernelShort(SizeType total)
	    : total2_(total)
	{}

	template<typename SomeCriticalStorageType>
	void operator()(SizeType index,
	                SizeType threadNum,
	                SomeCriticalStorageType& cs) const
	{
		int totalKernel=50;
		int bigNumber = 1e6;
		RealType sum = 0.0;
		for (int j = 0; j < totalKernel; ++j) {
			for (int mm = 0; mm < bigNumber; mm++)
				sum += (index + 1.0)*1e-6;
		}

		cs.value(0,threadNum) += sum;
	}

	SizeType size() const { return total2_; }

private:

	SizeType total2_;
};

int main(int argc, char* argv[])
{
//	typedef ConcurrentPsi::Parallelizer<ConcurrentPsi::TYPE_PTHREADS,
//	        Kernel1And2Type> ParallelizerType;
	typedef ConcurrentPsi::ParallelizerAsync<ConcurrentPsi::TYPE_PTHREADS,
	        KernelLong> ParallelizerLongType;
	typedef ConcurrentPsi::ParallelizerAsync<ConcurrentPsi::TYPE_PTHREADS,
	        KernelShort> ParallelizerShortType;
	typedef KernelLong::RealType RealType;
	typedef ParallelizerLongType::CriticalStorageType StorageType;
	typedef StorageType::ConcurrentPatternType ConcurrentPatternType;

	if (argc < 5) {
		std::cerr<<"USAGE "<<argv[0]<<" total1 total2 total3 nthreads1 ";
		std::cerr<<"[nthreads2]\n";
		return 1;
	}

	SizeType total1 = atoi(argv[1]);
	SizeType total2 = atoi(argv[2]);
	SizeType total3 = atoi(argv[3]);
	SizeType nthreads1 = atoi(argv[4]);
	SizeType nthreads2 = (argc == 6) ? atoi(argv[5]) : 1;
	std::cout<<"two loops in-tandem example \n";

	//Kernel1And2Type kernel1and2(total1,total2,1);
	KernelLong kernel1(total1);
	KernelShort kernel2(total2);
	KernelLong kernel3(total3);
	ParallelizerLongType p1(kernel1,nthreads1 + nthreads2,&argc,&argv);
	ParallelizerShortType p2(kernel2,nthreads1 + nthreads2,&argc,&argv);
	ParallelizerLongType p3(kernel3,nthreads1 + nthreads2,&argc,&argv);
	StorageType cs;
	ConcurrentPatternType pattern1(ConcurrentPatternType::PATTERN_REDUCE,
	                               ConcurrentPatternType::PATTERN_SUM);
	RealType tmp1 = 0;
	cs.push(&tmp1, pattern1);

	ConcurrentPatternType pattern2(ConcurrentPatternType::PATTERN_REDUCE,
	                               ConcurrentPatternType::PATTERN_SUM);
	RealType tmp2 = 0;
	cs.push(&tmp2, pattern2);

        ConcurrentPatternType pattern3(ConcurrentPatternType::PATTERN_REDUCE,
                                       ConcurrentPatternType::PATTERN_SUM);
        RealType tmp3 = 0;
        cs.push(&tmp3, pattern3);

	p1.launch(cs);
	p2.launch(cs);
	p3.launch(cs);

	cs.sync();

	if (ParallelizerType::canPrint())
		std::cout<<argv[0]<<" Result1 is "<<tmp1<<"\n";

	if (ParallelizerType::canPrint())
		std::cout<<argv[0]<<" Result2 is "<<tmp2<<"\n";

	if (ParallelizerType::canPrint())
		std::cout<<argv[0]<<" Result3 is "<<tmp3<<"\n";
}

