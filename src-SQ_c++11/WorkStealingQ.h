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

    //orktealing_queue(const work_stealing_queue& other)=delete;
    //orktealing_queue& operator=(const work_stealing_queue& other)=delete;

    void push(T new_job){
        std::lock_guard<std::mutex> lk(mut);
        job_deque.push_front(std::move(new_job));
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
