#include <iostream>
#include <thread>
#include <helpers.h>


int main() 
{

    {
        thread_pool tpool;

        tpool.submit([](){std::cout << "Task 1 is running\n";});
        tpool.submit([](){std::cout << "Task 2 is running\n";});
        tpool.submit([](){std::cout << "Task 3 is running\n";});
        tpool.submit([](){std::cout << "Task 4 is running\n";});
        tpool.submit([](){std::cout << "Task 5 is running\n";});
        tpool.submit([](){std::cout << "Task 6 is running\n";});

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }    
    std::cout << "The application is done." << std::endl;
    return 0;
}