#ifndef thread_pool_h
#define thread_pool_h

#include "locker.h"

#include <exception>
#include <list>
#include <pthread.h>

template <typename T>
class ThreadPool {

public:
    ThreadPool(int thread_number = 8, int max_requests = 10000);
    ~ThreadPool();

    bool append(T* request);

private:
    static void* worker(void* arg);
    void run();

private:
    int           thread_number_; // 线程池中线程数
    int           max_requests_; // 请求队列中允许的最大请求数
    pthread_t*    threads_; // 描述线程池的数组，大小为 thread_number_
    std::list<T*> workqueue_; // 请求队列
    Locker        queuelocker_; // 保护请求队列的互斥锁
    Sem           queuestat_; // 是否有任务需要处理
    bool          stop_; // 是否结束线程
};

template <typename T>
ThreadPool<T>::ThreadPool(int thread_number, int max_requests)
    : thread_number_(thread_number)
    , max_requests_(max_requests)
    , threads_(nullptr)
    , stop_(false)
{
    if (thread_number <= 0 || max_requests <= 0) {
        throw std::exception();
    }
    threads_ = new pthread_t[thread_number];
    if (!threads_) {
        throw std::exception();
    }
    for (int i = 0; i < thread_number; i++) {
        if (pthread_create(threads_ + i, nullptr, worker, this) != 0) {
            delete[] threads_;
            throw std::exception();
        }
        if (pthread_detach(threads_[i]) != 0) {
            delete[] threads_;
            throw std::exception();
        }
    }
}

template <typename T>
ThreadPool<T>::~ThreadPool()
{
    delete[] threads_;
    stop_ = true;
}

template <typename T>
bool ThreadPool<T>::append(T* request)
{
    queuelocker_.lock();
    if (workqueue_.size() > max_requests_) {
        queuelocker_.unlock();
        return false;
    }
    workqueue_.push_back(request);
    queuelocker_.unlock();
    queuestat_.post();
    return true;
}

template <typename T>
void* ThreadPool<T>::worker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    pool->run();
    return pool;
}

template <typename T>
void ThreadPool<T>::run()
{
    while (!stop_) {
        queuestat_.wait();
        queuelocker_.lock();
        if (workqueue_.empty()) {
            queuelocker_.unlock();
            continue;
        }
        T* request = workqueue_.front();
        workqueue_.pop_front();
        queuelocker_.unlock();
        if (!request) {
            continue;
        }
        request->process();
    }
}

#endif
