#include <iostream>
#include <thread>
#include <latch>
#include <vector>
#include <chrono>

std::mutex cout_mutex;

void concurrent_cout (const std::string& message)
{
    std::lock_guard cout_guard(cout_mutex);
    std::cout << message;
}


void workerTask(int id, std::latch& main_latch) {
    std::cout << "Worker " << id << " is starting work.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
    std::cout << "Worker " << id << " has completed part of the work.\n";
    main_latch.count_down(); // Signal that this thread has completed work
}

int main() {
    const int numThreads = 5;
    std::latch local_latch(numThreads); // Initialize latch with count equal to the number of threads

    // Launch worker threads
    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(workerTask, i, std::ref(local_latch));
    }

    // Wait for all worker threads to finish their work
    local_latch.wait();
    std::cout << "All workers have completed their tasks. Main thread proceeding.\n";

    // Join threads
    for (auto& worker : workers) {
        worker.join();
    }

    return 0;
}