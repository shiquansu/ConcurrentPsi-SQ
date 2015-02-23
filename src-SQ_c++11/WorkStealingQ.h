#include <queue>
#include <mutex> //require -std=c++11
#include <condition_variable> //require -std=c++11
#include <thread>
#include <memory>

namespace ConcurrentPsi {

template<typename T>
class WorkStealingQ
{
private:
    mutable std::mutex mut;
    std::deque<T> job_deque;
    std::condition_variable job_cond;
public:
    WorkStealingQ()
    {}

    WorkStealingQ(const WorkStealingQ& other)=delete;
    WorkStealingQ& operator=(const WorkStealingQ& other)=delete;

    void push(T new_job){
        std::lock_guard<std::mutex> lk(mut);
	std::cout<<"In WorkStealingQ.h, before job_deque.push_front.\n"; 
	new_job->executeTask(); 
        job_deque.push_front(new_job);
        //job_deque.push_front(std::move(new_job));
	std::cout<<"In WorkStealingQ.h, after job_deque.push_front.\n"; 
    }

    bool empty() const{
        std::lock_guard<std::mutex> lk(mut);
        return job_deque.empty();
    }

    bool can_pop(T& res){
        std::lock_guard<std::mutex> lk(mut);
        if(job_deque.empty()) return false;
        res=std::move(job_deque.front());
        job_deque.pop_front();
        return true;
    }

    bool can_steal(T& res){
        std::lock_guard<std::mutex> lk(mut);
        if(job_deque.empty()) return false;
        res=std::move(job_deque.back());
        job_deque.pop_back();
        return true;
    }


};


} //namespace ConcurrentPsi
