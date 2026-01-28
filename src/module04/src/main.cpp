#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>

void do_thread_work(std::promise<int> prom, int ndx) {
    std::cout << "The worker thread has started" << std::endl;
    std::cout << "and then might throw an exception" << std::endl;
    if (ndx <= 10) prom.set_value(ndx++);
    else try
    {
        throw std::invalid_argument("The ndx is out of range");
    }
    catch(...)
    {
        prom.set_exception(std::current_exception());
    }
    
}

int main() {
    std::vector<std::thread> threads;
    std::vector<std::future<int>> futures;
    std::promise<int> pm1;
    std::promise<int> pm2;
    futures.emplace_back(pm1.get_future());
    futures.emplace_back(pm2.get_future());
    try {
        threads.emplace_back(do_thread_work, std::move(pm1), 9);
        threads.emplace_back(do_thread_work, std::move(pm2), 42);
        for (auto& f : futures)
        {
            f.get();
        }
    }
    catch (std::exception ex)
    {
        std::cout << "The main thread has caught the exception thrown by the worker thread." << std::endl;
        std::cout << ex.what() << std::endl;
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "Main thread has finished it's work" << std::endl;
    return 0;
}