#include <queue>
//#include <mutex> //require -std=c++11                                           
//#include <condition_variable> //require -std=c++11     
//#include <thread>
#include "Pthreads.h"
#include <memory>

namespace ConcurrentPsi {

template<typename T>
class WorkStealingQ
{
private:

  //mutable std::mutex mut;                                                         
   pthread_mutex_t mutexq;

    std::deque<T> job_deque;

public:
    WorkStealingQ()
    {}

    WorkStealingQ(const WorkStealingQ& other)=delete;
    WorkStealingQ& operator=(const WorkStealingQ& other)=delete;

    void push(T new_job){
      //std::lock_guard<std::mutex> lk(mut);                                        
        pthread_mutex_lock (&mutexq);
        std::cout<<"In WorkStealingQ.h, before job_deque.push_front.\n";
        new_job->executeTask();
        job_deque.push_front(new_job);
        //job_deque.push_front(std::move(new_job));                                 
        std::cout<<"In WorkStealingQ.h, after job_deque.push_front.\n";
        pthread_mutex_unlock (&mutexq);
    }

    bool empty() const{
      //std::lock_guard<std::mutex> lk(mut);                                        
      bool t;
      pthread_mutex_lock (&mutexq);
      t = job_deque.empty();
      pthread_mutex_unlock (&mutexq);
      return t;
    }

    bool can_pop(T& res){
      //std::lock_guard<std::mutex> lk(mut);                                        
        pthread_mutex_lock (&mutexq);
        if(job_deque.empty()){
          pthread_mutex_unlock (&mutexq);
          return false;
        }
        res=std::move(job_deque.front());
        job_deque.pop_front();
        pthread_mutex_unlock (&mutexq);
        return true;
    }

    bool can_steal(T& res){
       pthread_mutex_lock (&mutexq);
       //std::lock_guard<std::mutex> lk(mut);                                       
       if(job_deque.empty()){
         pthread_mutex_unlock (&mutexq);
          return false;
       }
        res=std::move(job_deque.back());
        job_deque.pop_back();
        pthread_mutex_unlock (&mutexq);
        return true;
    }


};

} //namespace ConcurrentPsi
