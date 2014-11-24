/*
Only have  pthread implementation currently.
*/
#ifndef PARALLELIZERASYNC_H
#define PARALLELIZERASYNC_H
#include "Vector.h"
#include "Pthreads.h"
#include <cassert>
#include "CriticalStorage.h"
#include "TypeToString.h"
#include "MpiHolder.h"

namespace ConcurrentPsi {

template<ParallelTypeEnum type,typename KernelType>
class ParallelizerBase {

protected:

	typedef typename KernelType::RealType RealType;

public:

	typedef CriticalStorage<type,RealType> CriticalStorageType;

	static bool canPrint() { return true; }
};

template<typename KernelType>
class ParallelizerBase<TYPE_MPI, KernelType> : public MpiHolder {

protected:

	typedef typename KernelType::RealType RealType;

	ParallelizerBase(int* argcPtr, char*** argvPtr, SizeType mpiSizeArg)
	    : MpiHolder(argcPtr,argvPtr,mpiSizeArg)
	{}

public:

	typedef CriticalStorage<TYPE_MPI,RealType> CriticalStorageType;

};


template<typename KernelType>
class ParallelizerAsync<TYPE_PTHREADS,KernelType>
        : public ParallelizerBase<TYPE_PTHREADS,KernelType> {

	typedef ParallelizerBase<TYPE_PTHREADS,KernelType> BaseType;
	typedef typename BaseType::CriticalStorageType CriticalStorageType_;

	struct PthreadFunctionStruct {
		PthreadFunctionStruct()
		    :  kernel(0), blockSize(0), total(0), threadNum(0), criticalStorage(0)
		{}

		KernelType* kernel;
		SizeType blockSize;
		SizeType total;
		int threadNum;
		CriticalStorageType_* criticalStorage;
	};

public:

	typedef CriticalStorageType_ CriticalStorageType;

	ParallelizerAsync(KernelType& kernel,
	             SizeType nPthreads,
	             int* = 0,
	             char*** = 0)
	    : kernel_(kernel),nthreads_(nPthreads)
	{}

	void launch(CriticalStorageType& cs)
	{
		PthreadFunctionStruct pfs[nthreads_];
		Pthreads::PthreadType threadId[nthreads_];

		SizeType total = kernel_.size();

		cs.prepare(nthreads_);

                // wait until(availCE >= nthreads)

		for (SizeType j=0; j < nthreads_; j++) {
			int ret=0;
			pfs[j].threadNum = j;
			pfs[j].kernel = &kernel_;
			pfs[j].total = total;
			pfs[j].blockSize = total/nthreads_;
			if (total%nthreads_!=0) pfs[j].blockSize++;
			pfs[j].criticalStorage = &cs;
			if ((ret=Pthreads::create(&threadId[j],0,threadFunctionWrapper,&pfs[j])))
				std::cerr<<"Thread creation failed: "<<ret<<"\n";
		}

//		for (SizeType j=0; j < nthreads_; j++) Pthreads::join(threadId[j], 0);
                // return the thread to the pool, availCE++
	}

private:

	static void *threadFunctionWrapper(void *dummyPtr)
	{
		PthreadFunctionStruct* pfs = (PthreadFunctionStruct*) dummyPtr;

		KernelType *kernel = pfs->kernel;

		SizeType blockSize = pfs->blockSize;

		for (SizeType i = 0; i < blockSize; ++i) {
			SizeType index = i + pfs->threadNum*blockSize;
			if (index >= pfs->total) continue;
			kernel->operator()(index,
			                   pfs->threadNum,
			                   *pfs->criticalStorage);
		}
        // return the thread to the pool, availCE++
		return 0;
	}

	KernelType& kernel_;
	SizeType nthreads_;
}; // class ParallelizerAsync


} // namespace ConcurrentPsi

#endif // PARALLELIZERASYNC_H

