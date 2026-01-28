#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>

std::mutex cout_mutex;

void concurrent_cout (const std::string& message)
{
    std::lock_guard cout_guard(cout_mutex);
    std::cout << message;
}

class PhotoBooth
{
private:
    std::counting_semaphore<3> semaphore{3}; // Allows up to 3 concurrent accesses

public:
    PhotoBooth() {};
    void accessResource(int id) {
        semaphore.acquire(); // Acquire a permit
        // std::cout << "Thread " << id << " is releasing the resource\n";
        concurrent_cout(std::format("Thread {} is accessing the resource.\n", id));
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        // std::cout << "Thread " << id << " is acquiring the resource\n";
        concurrent_cout(std::format("Thread {} is releasing the resource.\n", id));
        semaphore.release(); // Release the permit
    }
};

int main() {
    PhotoBooth booth;
    std::thread threads[6];
    for (int i = 0; i < 6; ++i) {
        threads[i] = std::thread(&PhotoBooth::accessResource, &booth, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}