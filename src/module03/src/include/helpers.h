#pragma once

// Global function to display the message
void displayMessage(std::string message) {
    std::cout << message << std::endl;
}

void computeValue(std::promise<int> prom, int x){
    try {
        if (x < 0) throw std::invalid_argument("Negative input not allowed");
        prom.set_value(x*x);
    }
    catch(...) {
        prom.set_exception(std::current_exception());
    }
}
// Optionally can use old style
//#ifndef HELPERS_H
//#define HELPERS_H
// <move code here>
//#endif

