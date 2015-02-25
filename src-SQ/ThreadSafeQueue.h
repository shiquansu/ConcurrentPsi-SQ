// Shiquan Su adds a JoinThreads class at the end, the code comes from "http://stackoverflow.com/questions/7858866/why-not-run-task-after-pop-and-return-true"

#include <queue>
//#include <mutex> //require -std=c++11
//#include <condition_variable> //require -std=c++11
//#include <thread>
#include "Pthreads.h"
#include <memory>

namespace ConcurrentPsi {

template<typename T>
class ThreadSafeQueue
{
private:
    //mutable std::mutex mut;
    pthread_mutex_t mut;
    std::queue<T> data_queue;
    //std::condition_variable data_cond;
    pthread_cond_t data_cond;
public:
    ThreadSafeQueue()
    {}

    void push(T new_value)
    {
        //std::lock_guard<std::mutex> lk(mut);
        pthread_mutex_lock(&mut);
        data_queue.push(std::move(new_value));
        //data_cond.notify_one();
	pthread_cond_signal(&data_cond);
        pthread_mutex_unlock(&mut);
    }

    void wait_and_pop(T& value)
    {
        //std::unique_lock<std::mutex> lk(mut);
        pthread_mutex_lock(&mut);
        //data_cond.wait(lk,[this]{return !data_queue.empty();});
	pthread_cond_wait(&data_cond,&mut);
        value=std::move(data_queue.front());
        data_queue.pop();
        pthread_mutex_unlock(&mut);
    }

    std::shared_ptr<T> wait_and_pop()
    {
        //std::unique_lock<std::mutex> lk(mut);
        pthread_mutex_lock(&mut);
        //data_cond.wait(lk,[this]{return !data_queue.empty();});
	pthread_cond_wait(&data_cond,&mut);
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        pthread_mutex_unlock(&mut);
        return res;
    }

    bool can_pop(T& value)
    {
        //std::unique_lock<std::mutex> lk(mut);
        pthread_mutex_lock(&mut);
        if(data_queue.empty())
            return false;
        value=std::move(data_queue.front());
        data_queue.pop();
        pthread_mutex_unlock(&mut);
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        //std::unique_lock<std::mutex> lk(mut);
        pthread_mutex_lock(&mut);
        if(data_queue.empty()){
        	return std::shared_ptr<T>();
        	pthread_mutex_unlock(&mut);
	}
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        pthread_mutex_unlock(&mut);
        return res;
    }

    bool empty() const
    {
        //std::unique_lock<std::mutex> lk(mut);
        pthread_mutex_lock(&mut);
	bool t;
	t = data_queue.empty();
        pthread_mutex_unlock(&mut);
        return t;
    }
};

/*
class JoinThreads
{
    std::vector<std::thread>& threads;
public:
    explicit JoinThreads(std::vector<std::thread>& threads_):threads(threads_)
    {}
    ~JoinThreads()
    {
        for(unsigned long i=0;i<threads.size();++i)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
};
*/

} //namespace ConcurrentPsi
