#pragma once
#include <thread>
#include <queue>
#include <vector>
#include <functional>

class thread_pool
{
    bool done;
    std::queue<std::function<void()>> pending_queue;
    std::vector<std::thread> threads;
    std::mutex queue_mutex;

    void worker_thread()
    {
        while (!done)
        {
            std::function<void()> task = nullptr;
            {
                std::lock_guard queue_guard(queue_mutex);
                if (!pending_queue.empty())
                {
                    task = pending_queue.front();
                    pending_queue.pop();
                }
                else
                {
                    std::this_thread::yield();
                }
            }
            if (task != nullptr)
            {
                task();
            }
        }
    }

public:
    thread_pool() : done(false)
    {
        unsigned const thread_count =
            std::thread::hardware_concurrency();
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.emplace_back(
                    std::thread(&thread_pool::worker_thread, this));
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }
    ~thread_pool()
    {
        {
            std::lock_guard done_guard(queue_mutex);
            done = true;
        }
        for (auto& t : threads){
            t.join();
        }
        
    }
    template <typename FunctionType>
    void submit(FunctionType f)
    {
        {
            std::lock_guard queue_guard(queue_mutex);
            pending_queue.push(std::function<void()>(f));
        }
    }
};
// Optionally can use old style
// #ifndef HELPERS_H
// #define HELPERS_H
// <move code here>
// #endif
