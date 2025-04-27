#pragma once

#include <future>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool 
{
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable cond_;
    bool stop_;

public:
    explicit ThreadPool(size_t num_threads);

    template <typename F>
    std::future<void> enqueue(F&& f) 
    {
        auto task = std::make_shared<std::packaged_task<void()>>(std::forward<F>(f));
        std::future<void> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (stop_) 
                throw std::runtime_error("Пул остановлен");
            tasks_.emplace([task]() { (*task)(); });
        }
        cond_.notify_one();
        return res;
    }

    ~ThreadPool();
};