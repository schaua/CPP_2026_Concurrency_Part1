#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <future>
#include <exception>
#include <helpers.h>

std::mutex balance_mutex;
double balance = 1000;

void deposit_thread(double amount)
{
    const std::lock_guard<std::mutex> lock_deposit{balance_mutex};
    std::cout << "The balance is " << balance << std::endl;
    balance += amount;
    std::cout << "After deposit the balance is " << balance << std::endl;
}

void withdraw_thread(double amount)
{
    const std::lock_guard<std::mutex> lock_withdraw{balance_mutex};
    std::cout << "The balance is " << balance << std::endl;
    balance -= amount;
    std::cout << "After withdraw the balance is " << balance << std::endl;
}

int main()
{
    std::vector<std::thread> threads{};

    threads.emplace_back(std::thread (deposit_thread, 100));
    threads.emplace_back(std::thread (withdraw_thread, 200));

    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    threads.emplace_back(std::thread(computeValue, std::move(prom), -5));
    
    std::cout << "Main thread working..." << std::endl;

    try{
    std::cout << "The promised provided " << fut.get() << std::endl;
    }
    catch (std::invalid_argument){
        std::cout << "Ooops" << std::endl;
    }

    for (auto& t : threads)
    {
        t.join();
    }
    
    std::cout << "The final balance is " << balance << std::endl;
}