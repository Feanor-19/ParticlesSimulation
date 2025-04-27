#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t num_threads)
    : stop_(false) 
{
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    cond_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) 
                        return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    cond_.notify_all();
    for (auto& worker : workers_) 
        worker.join();
}
