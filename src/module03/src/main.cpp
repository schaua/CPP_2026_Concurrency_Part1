#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> dataQueue;
bool done = false;

// Producer function
void producer() {
    for (int i = 1; i <= 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            dataQueue.push(i);
            std::cout << "Produced: " << i << std::endl;
        }
        cv.notify_one(); // Notify one waiting consumer
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true; // Signal that production is finished
    }
    cv.notify_all(); // Notify all consumers to stop waiting
}

// Consumer function
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !dataQueue.empty() || done; }); // Wait for data or done signal

        while (!dataQueue.empty()) {
            int value = dataQueue.front();
            dataQueue.pop();
            std::cout << "Consumed: " << value << std::endl;
        }

        // Exit if done and no more items in the queue
        if (done && dataQueue.empty()) {
            break;
        }
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}