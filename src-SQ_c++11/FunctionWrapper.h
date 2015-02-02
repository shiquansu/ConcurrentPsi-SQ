#include <queue>
#include <mutex> //require -std=c++11
#include <condition_variable> //require -std=c++11
#include <memory>

namespace ConcurrentPsi {

class FunctionWrapper {
	struct ImplBase {
		virtual void call()=0;
		virtual ~ImplBase() {}
	};
	std::unique_ptr<ImplBase> impl;
	template<typename F>
	struct ImplType: ImplBase {
		F f;
		ImplType(F&& f_): f(std::move(f_)) {}
		void call() { f(); }
	};
public:
	template<typename F>
	FunctionWrapper(F&& f): impl(new ImplType<F>(std::move(f))) {}

	void call() { impl->call(); }  // from source code list 9.2

	void operator()() {impl->call();} // from book page 277 listing 9.2
	FunctionWrapper()=default;        // from book page 277 listing 9.2

	FunctionWrapper(FunctionWrapper&& inFunc): impl(std::move(inFunc.impl)) {}

	FunctionWrapper& operator=(FunctionWrapper&& inFunc){
		impl=std::move(inFunc.impl);
		return *this;
	};

	FunctionWrapper(const FunctionWrapper&)=delete; //disallow copying
	FunctionWrapper(FunctionWrapper&)=delete;      //disallow lvalue reference moving
	FunctionWrapper& operator=(const FunctionWrapper&)=delete; //disallow copying
}; //class FunctionWrapper

} //namespace ConcurrentPsi
