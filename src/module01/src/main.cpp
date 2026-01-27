#include <iostream>
#include <thread>
#include <message.h>

// Sample app just to make sure the environment is setup 

int main() {

    std::thread not_thread{displayMessage, "Hola, Mondo"};
    if (not_thread.joinable()){
        std::cout << "Is joinable" << std::endl;
    }
    else 
        std::cout << "Not joinable" << std::endl;
    not_thread.join();
    return 0;
}